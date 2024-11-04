#include "base_window.h"

#include <GLFW/glfw3.h>

#include "core/log.h"
#include "core/window_manager.h"

BaseWindow::~BaseWindow() {
    glfwDestroyWindow(_window);
    _window = nullptr;
}

void BaseWindow::render() {
    glfwMakeContextCurrent(_window);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Red background
    glClear(GL_COLOR_BUFFER_BIT);

    onRender();

    glfwSwapBuffers(_window);
}

void BaseWindow::init(const InitParams& params) {
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
    _window = glfwCreateWindow(params.width, params.height, params.title.c_str(), NULL, NULL);
}

void BaseWindow::show() { _winsMgr->addWindow(this); }

bool BaseWindow::shouldClose() { return glfwWindowShouldClose(_window); }
