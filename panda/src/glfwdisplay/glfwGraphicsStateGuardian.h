#ifndef GLFWGRAPHICSSTATEGUARDIAN_H
#define GLFWGRAPHICSSTATEGUARDIAN_H

#include "glgsg.h"
#include "frameBufferProperties.h"

class glfwGraphicsStateGuardian : public GLGraphicsStateGuardian {
public:
  glfwGraphicsStateGuardian(GraphicsEngine *engine, GraphicsPipe *pipe);
  virtual ~glfwGraphicsStateGuardian();

  const FrameBufferProperties &get_fb_properties() const;
  void update_fb_properties();

protected:
  virtual void *do_get_extension_func(const char *name) override;

private:
  FrameBufferProperties _fbprops;
};

#endif
