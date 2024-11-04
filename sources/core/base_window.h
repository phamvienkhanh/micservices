#ifndef _BASE_WINDOW_H_
#define _BASE_WINDOW_H_

#include <GLFW/glfw3.h>

#include <string>

class BaseWindow {
  public:
    struct InitParams {
        std::string title;
        int width = 0;
        int height = 0;
    };

  public:
    BaseWindow() = default;
    virtual ~BaseWindow();

    virtual void onRender() = 0;
    virtual void onClose() = 0;

    void render();
    void init(const InitParams& params);
    void show();
    bool shouldClose();

  private:
    GLFWwindow* _window = nullptr;
};

#endif