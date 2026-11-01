#ifndef GLFWCALLBACKWINDOW_H
#define GLFWCALLBACKWINDOW_H

#include "callbackGraphicsWindow.h"
#include "callbackObject.h"
#include "callbackData.h"

#include <GLFW/glfw3.h>

class glfwRenderCallback : public CallbackObject {
private:
  GLFWwindow *_window;

public:
  explicit glfwRenderCallback(GLFWwindow *window) : _window(window) {}

  virtual void do_callback(CallbackData *cbdata) {
    auto *data =
      static_cast<CallbackGraphicsWindow::RenderCallbackData *>(cbdata);

    switch (data->get_callback_type()) {
      case CallbackGraphicsWindow::RCT_begin_frame:
        glfwMakeContextCurrent(_window);
        data->upcall();
        break;

      case CallbackGraphicsWindow::RCT_end_frame:
        data->upcall();
        break;

      case CallbackGraphicsWindow::RCT_begin_flip:
        data->upcall();
        break;

      case CallbackGraphicsWindow::RCT_end_flip:
        glfwSwapBuffers(_window);
        data->upcall();
        break;
    }
  }
};

class glfwCallbackWindow : public CallbackGraphicsWindow {
private:
  GLFWwindow *_window;

public:
  glfwCallbackWindow(
    GraphicsEngine *engine,
    GraphicsPipe *pipe,
    const std::string &name,
    const FrameBufferProperties &fb_prop,
    const WindowProperties &win_prop,
    int flags,
    GraphicsStateGuardian *gsg,
    GLFWwindow *window
  ) :
    CallbackGraphicsWindow(
      engine, pipe, name, fb_prop, win_prop, flags, gsg
    ),
    _window(window)
  {
    set_render_callback(new glfwRenderCallback(_window));

    WindowProperties properties;
    properties.set_origin(0, 0);
    properties.set_size(
      glfwGetWindowSizeWidth(),
      glfwGetWindowSizeHeight()
    );
    system_changed_properties(properties);
  }

  virtual ~glfwCallbackWindow() {
    if (_window) {
      glfwDestroyWindow(_window);
      _window = nullptr;
    }
  }

  virtual void process_events() {
    glfwPollEvents();

    if (_window && glfwWindowShouldClose(_window)) {
      request_close();
    }

    CallbackGraphicsWindow::process_events();
  }

  int glfwGetWindowSizeWidth() const {
    int width = 0;
    int height = 0;
    glfwGetWindowSize(_window, &width, &height);
    return width;
  }

  int glfwGetWindowSizeHeight() const {
    int width = 0;
    int height = 0;
    glfwGetWindowSize(_window, &width, &height);
    return height;
  }
};

#endif
