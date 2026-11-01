#include "InitScene.h"

#include "load_prc_file.h"
#include "windowProperties.h"

#include <iostream>

static PandaFramework framework;

PandaFramework& GetSceneFramework()
{
    return framework;
}

WindowFramework* InitScene(
    int x,
    int y,
    const std::string& title,
    bool fullscreen
)
{
    load_prc_file_data("", R"(
        load-display p3glfwdisplay
        framebuffer-multisample 0
        sync-video false
    )");

    framework.open_framework();
    framework.set_window_title(title);

    WindowProperties properties;

    framework.get_default_window_props(properties);

    properties.set_size(x, y);
    properties.set_title(title);
    properties.set_fullscreen(fullscreen);
    properties.set_foreground(true);

    WindowFramework* window = framework.open_window(properties, 0);

    if (window == nullptr) {
        std::cerr << "Erreur : impossible de creer la fenetre GLFW3\n";
        framework.close_framework();
        return nullptr;
    }

    return window;
}
