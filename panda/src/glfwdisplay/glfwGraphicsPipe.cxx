#include "glfwGraphicsPipe.h"
#include "glfwGraphicsWindow.h"
#include "config_glfwdisplay.h"

#include <GLFW/glfw3.h>
#include <cstdio>

TypeHandle glfwGraphicsPipe::_type_handle;

glfwGraphicsPipe::glfwGraphicsPipe() {
  glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);

  glfwSetErrorCallback([](int code, const char *description) {
    fprintf(stderr, "GLFW ERROR %d: %s\n", code, description);
  });

  _is_valid = glfwInit() == GLFW_TRUE;
  _supported_types = OT_window;

  if (!_is_valid) {
    fprintf(stderr, "GLFW: initialization failed\n");
  }
}

glfwGraphicsPipe::~glfwGraphicsPipe() {
  if (_is_valid) {
    glfwTerminate();
  }
}

std::string glfwGraphicsPipe::get_interface_name() const {
  return "GLFW3 Wayland";
}

PT(GraphicsPipe) glfwGraphicsPipe::pipe_constructor() {
  return new glfwGraphicsPipe;
}

PT(GraphicsStateGuardian)
glfwGraphicsPipe::make_callback_gsg(GraphicsEngine *engine) {
  // GLFW creates and owns its context through glfwGraphicsWindow.
  return nullptr;
}

PT(GraphicsOutput) glfwGraphicsPipe::make_output(
  std::string_view name,
  const FrameBufferProperties &fb_prop,
  const WindowProperties &win_prop,
  int flags,
  GraphicsEngine *engine,
  GraphicsStateGuardian *gsg,
  GraphicsOutput *host,
  int retry,
  bool &precertify
) {
  precertify = false;

  if (!_is_valid || retry != 0) {
    return nullptr;
  }

  if (flags & BF_refuse_window) {
    return nullptr;
  }

  return new glfwGraphicsWindow(
    engine,
    this,
    std::string(name),
    fb_prop,
    win_prop,
    flags,
    gsg,
    host
  );
}
