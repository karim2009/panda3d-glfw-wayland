#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <iostream>

int main()
{
    if (!glfwInit()) {
        std::cerr << "GLFW initialization failed\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(
        1280, 720, "Panda3D GLFW Wayland", nullptr, nullptr);

    if (!window) {
        std::cerr << "GLFW window creation failed\n";
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    std::cout << "GL_VENDOR: "
              << glGetString(GL_VENDOR) << '\n';
    std::cout << "GL_RENDERER: "
              << glGetString(GL_RENDERER) << '\n';
    std::cout << "GL_VERSION: "
              << glGetString(GL_VERSION) << '\n';

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0.15f, 0.25f, 0.45f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
