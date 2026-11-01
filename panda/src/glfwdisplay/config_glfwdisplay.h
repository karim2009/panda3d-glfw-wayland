#ifndef CONFIG_GL FWDISPLAY_H
#define CONFIG_GL FWDISPLAY_H

#include "pandabase.h"
#include "notifyCategoryProxy.h"

NotifyCategoryDecl(glfwdisplay, EXPCL_PANDAGL, EXPTP_PANDAGL);

extern EXPCL_PANDAGL void init_libglfwdisplay();

#endif
