#ifndef GLFWGRAPHICSWINDOW_H
#define GLFWGRAPHICSWINDOW_H

#include "pandabase.h"
#include "graphicsWindow.h"
#include <GLFW/glfw3.h>

class glfwGraphicsWindow : public GraphicsWindow {
public:
  glfwGraphicsWindow(
    GraphicsEngine *engine,
    GraphicsPipe *pipe,
    std::string name,
    const FrameBufferProperties &fb_prop,
    const WindowProperties &win_prop,
    int flags,
    GraphicsStateGuardian *gsg,
    GraphicsOutput *host
  );

  virtual ~glfwGraphicsWindow();

  virtual bool begin_frame(FrameMode mode, Thread *current_thread);
  virtual void end_frame(FrameMode mode, Thread *current_thread);
  virtual void end_flip();
  virtual void process_events();

protected:
  virtual bool open_window();
  virtual void close_window();

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }

  static void init_type() {
    GraphicsWindow::init_type();
    register_type(_type_handle, "glfwGraphicsWindow",
                  GraphicsWindow::get_class_type());
  }

  virtual TypeHandle get_type() const {
    return get_class_type();
  }

  virtual TypeHandle force_init_type() {
    init_type();
    return get_class_type();
  }

private:
  GLFWwindow *_window = nullptr;
  static TypeHandle _type_handle;
};

#endif
