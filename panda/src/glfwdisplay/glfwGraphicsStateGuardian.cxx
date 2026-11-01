#include "glfwGraphicsStateGuardian.h"
#include <cstdio>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GLFW/glfw3.h>

glfwGraphicsStateGuardian::glfwGraphicsStateGuardian(
  GraphicsEngine *engine,
  GraphicsPipe *pipe
) :
  GLGraphicsStateGuardian(engine, pipe) {
}

glfwGraphicsStateGuardian::~glfwGraphicsStateGuardian() {
}

void *glfwGraphicsStateGuardian::do_get_extension_func(
  const char *name
) {
  if (name == nullptr) {
    return nullptr;
  }

  return (void *)glfwGetProcAddress(name);
}

const FrameBufferProperties &
glfwGraphicsStateGuardian::get_fb_properties() const {
  return _fbprops;
}

void glfwGraphicsStateGuardian::update_fb_properties() {
  GLint depth = 0;
  GLint stencil = 0;
  GLboolean double_buffer = GL_FALSE;
  GLint samples = 0;
  GLint sample_buffers = 0;
  GLint red = 0, green = 0, blue = 0, alpha = 0;

  auto get_attachment = reinterpret_cast<PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC>(
    glfwGetProcAddress("glGetFramebufferAttachmentParameteriv"));

  if (get_attachment != nullptr) {
    get_attachment(GL_FRAMEBUFFER, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE, &red);
    get_attachment(GL_FRAMEBUFFER, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE, &green);
    get_attachment(GL_FRAMEBUFFER, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE, &blue);
    get_attachment(GL_FRAMEBUFFER, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE, &alpha);
    get_attachment(GL_FRAMEBUFFER, GL_DEPTH, GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE, &depth);
    get_attachment(GL_FRAMEBUFFER, GL_STENCIL, GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE, &stencil);
  }

  glGetBooleanv(GL_DOUBLEBUFFER, &double_buffer);
  glGetIntegerv(GL_SAMPLE_BUFFERS, &sample_buffers);
  glGetIntegerv(GL_SAMPLES, &samples);

  _fbprops.clear();
  _fbprops.set_rgb_color(true);
  _fbprops.set_rgba_bits(red, green, blue, alpha);
  _fbprops.set_depth_bits(depth);
  _fbprops.set_stencil_bits(stencil);
  _fbprops.set_multisamples(sample_buffers ? samples : 0);
  _fbprops.set_back_buffers(1);
  _fbprops.set_force_hardware(true);
  _fbprops.set_force_software(false);
}
