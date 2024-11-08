#include "base_window.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <GLFW/glfw3.h>

#include "core/window_manager.h"

BaseWindow::~BaseWindow() {
    glfwDestroyWindow(_window);
    _window = nullptr;
}

void BaseWindow::render() {
    glfwMakeContextCurrent(_window);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwMakeContextCurrent(_window);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    onRender();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(_window);
}

void BaseWindow::init(const InitParams& params) {
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    for (auto it : params.flags) {
        glfwWindowHint(it.first, it.second);
    }

    _window = glfwCreateWindow(params.width, params.height, params.title.c_str(), NULL, NULL);

    glfwSetWindowUserPointer(_window, this);

    glfwSetWindowPosCallback(_window, moveCallback);
    glfwSetWindowSizeCallback(_window, resizeCallback);

    glfwMakeContextCurrent(_window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& _ = ImGui::GetIO();

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(_window, true);
    ImGui_ImplOpenGL3_Init("#version 130");     


    onInit();
}

void BaseWindow::show() { _winsMgr->addWindow(this); }

bool BaseWindow::shouldClose() { return glfwWindowShouldClose(_window); }

Position BaseWindow::getPos() {
    Position pos;
    glfwGetWindowPos(_window, &pos.x, &pos.y);
    return pos;
}

Size BaseWindow::getSize() {
    Size size;
    glfwGetWindowSize(_window, &size.w, &size.h);
    return size;
}

void BaseWindow::resizeCallback(GLFWwindow* window, int w, int h) {
    BaseWindow* self = (BaseWindow*)glfwGetWindowUserPointer(window);
    if (self) {
        self->onResize();
    }
}

void BaseWindow::moveCallback(GLFWwindow* window, int x, int y) {
    BaseWindow* self = (BaseWindow*)glfwGetWindowUserPointer(window);
    if (self) {
        self->onMove();
    }
}
