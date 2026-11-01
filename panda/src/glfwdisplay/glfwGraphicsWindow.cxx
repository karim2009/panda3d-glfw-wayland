#include <cstdlib>
#include "glfwGraphicsWindow.h"
#include "glfwGraphicsPipe.h"
#include "config_glfwdisplay.h"
#include "glfwGraphicsStateGuardian.h"

#include "glgsg.h"
#include "pStatTimer.h"

TypeHandle glfwGraphicsWindow::_type_handle;

glfwGraphicsWindow::glfwGraphicsWindow(
  GraphicsEngine *engine,
  GraphicsPipe *pipe,
  std::string name,
  const FrameBufferProperties &fb_prop,
  const WindowProperties &win_prop,
  int flags,
  GraphicsStateGuardian *gsg,
  GraphicsOutput *host
) :
  GraphicsWindow(engine, pipe, std::move(name),
                 fb_prop, win_prop, flags, gsg, host) {
}

glfwGraphicsWindow::~glfwGraphicsWindow() {
  close_window();
}

bool glfwGraphicsWindow::open_window() {
  if (_window != nullptr) {
    return true;
  }

  glfwDefaultWindowHints();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
  glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
  glfwWindowHint(GLFW_RED_BITS, 8);
  glfwWindowHint(GLFW_GREEN_BITS, 8);
  glfwWindowHint(GLFW_BLUE_BITS, 8);
  glfwWindowHint(GLFW_ALPHA_BITS, 8);
  glfwWindowHint(GLFW_DEPTH_BITS, 24);
  glfwWindowHint(GLFW_STENCIL_BITS, 8);

  const WindowProperties &props = get_properties();
  int width = props.get_x_size();
  int height = props.get_y_size();

  if (width <= 0) width = 1280;
  if (height <= 0) height = 720;

  std::string title = props.has_title() ? props.get_title() : get_name();
  GLFWmonitor *monitor = (props.has_fullscreen() && props.get_fullscreen())
    ? glfwGetPrimaryMonitor() : nullptr;

  _window = glfwCreateWindow(
    width, height, title.c_str(), monitor, nullptr
  );

  if (_window == nullptr) {
    glfwdisplay_cat.error() << "glfwCreateWindow failed\n";
    return false;
  }


  glfwMakeContextCurrent(_window);

  glfwGraphicsStateGuardian *glfwgsg;

  if (_gsg == nullptr) {
    glfwgsg = new glfwGraphicsStateGuardian(_engine, _pipe);
    _gsg = glfwgsg;
  } else {
    DCAST_INTO_R(glfwgsg, _gsg, false);
  }

  glfwgsg->reset_if_new();

  if (!glfwgsg->is_valid()) {
    glfwdisplay_cat.error() << "OpenGL GSG initialization failed\n";
    close_window();
    return false;
  }

  glfwgsg->update_fb_properties();

  _fb_properties = glfwgsg->get_fb_properties();
  _fb_properties.set_force_software(false);


  WindowProperties actual;
  actual.set_origin(0, 0);
  actual.set_size(width, height);
  actual.set_title(title);
  actual.set_fullscreen(monitor != nullptr);
  system_changed_properties(actual);

  glfwSwapInterval(1);

  glfwdisplay_cat.info()
    << "GLFW window opened: " << width << "x" << height << "\n";

  return true;
}

void glfwGraphicsWindow::close_window() {
  if (_window != nullptr) {
    glfwMakeContextCurrent(_window);
    _gsg.clear();
    glfwMakeContextCurrent(nullptr);
    glfwDestroyWindow(_window);
    _window = nullptr;
  } else {
    _gsg.clear();
  }

  GraphicsWindow::close_window();
}

bool glfwGraphicsWindow::begin_frame(
  FrameMode mode, Thread *current_thread
) {
  PStatTimer timer(_make_current_pcollector, current_thread);
  begin_frame_spam(mode);

  if (_window == nullptr || _gsg == nullptr) {
    return false;
  }

  glfwMakeContextCurrent(_window);

  GLGraphicsStateGuardian *glgsg;
  DCAST_INTO_R(glgsg, _gsg, false);
  glgsg->reset_if_new();

  if (mode == FM_render) {
    glgsg->push_group_marker(
      std::string("glfwGraphicsWindow ") + get_name()
    );
    clear_cube_map_selection();
  }

  _gsg->set_current_properties(&get_fb_properties());
  return _gsg->begin_frame(current_thread);
}

void glfwGraphicsWindow::end_frame(
  FrameMode mode, Thread *current_thread
) {
  end_frame_spam(mode);

  if (_gsg == nullptr) {
    return;
  }

  if (mode == FM_render) {
    copy_to_textures();
  }

  _gsg->end_frame(current_thread);

  if (mode == FM_render) {
    trigger_flip();
    clear_cube_map_selection();

    GLGraphicsStateGuardian *glgsg;
    DCAST_INTO_V(glgsg, _gsg);
    glgsg->pop_group_marker();
  }
}

void glfwGraphicsWindow::end_flip() {
  if (_window != nullptr && _gsg != nullptr && _flip_ready) {
    if (glfwGetCurrentContext() == _window) {
      glfwSwapBuffers(_window);
    } else {
      glfwdisplay_cat.error()
        << "GLFW context is not current on the draw thread during buffer swap\n";
    }
  }

  GraphicsWindow::end_flip();
}

void glfwGraphicsWindow::process_events() {
  if (_window != nullptr) {
    glfwPollEvents();
    if (glfwWindowShouldClose(_window) ||
        glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      _exit(0);
    }
    int width = 0;
    int height = 0;
    glfwGetWindowSize(_window, &width, &height);
    if (width > 0 && height > 0 &&
        (width != get_x_size() || height != get_y_size())) {
      WindowProperties properties;
      properties.set_size(width, height);
      system_changed_properties(properties);
    }
  }
  GraphicsWindow::process_events();
}
