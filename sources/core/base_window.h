#ifndef _BASE_WINDOW_H_
#define _BASE_WINDOW_H_

#include <GLFW/glfw3.h>

#include <string>
#include <vector>

#include "core/types.h"

class BaseWindow {
  public:
    struct InitParams {
        std::string title;
        int width = 0;
        int height = 0;
        std::vector<std::pair<int, int>> flags;
    };

  public:
    BaseWindow() = default;
    virtual ~BaseWindow();

    virtual void onRender() = 0;
    virtual void onClose() = 0;
    virtual void onInit() = 0;
    virtual void onMove() = 0;
    virtual void onResize() = 0;

    void render();
    void init(const InitParams& params);
    void show();
    bool shouldClose();

    Position getPos();
    Size getSize();

  private:
    static void resizeCallback(GLFWwindow* window, int w, int h);
    static void moveCallback(GLFWwindow* window, int x, int y);

  protected:
    GLFWwindow* _window = nullptr;
};

#endif