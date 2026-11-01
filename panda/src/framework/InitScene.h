#pragma once

#include "pandaFramework.h"
#include "windowFramework.h"

#include <string>

WindowFramework* InitScene(
    int x,
    int y,
    const std::string& title,
    bool fullscreen = false
);

PandaFramework& GetSceneFramework();
