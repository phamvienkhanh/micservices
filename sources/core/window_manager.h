#ifndef _WINDOW_MANAGER_H_
#define _WINDOW_MANAGER_H_

#include <vector>

#include "core/base_window.h"

class WindowManager {
  public:
    void addWindow(BaseWindow* window);
    void removeWindow(BaseWindow* window);
    void renderAllWindow();
    void shouldCloseWindow();
    int countWindows();

    void shouldDestroy();
    void destroyLater(BaseWindow* window);

    static WindowManager* inst();

  private:
    WindowManager() = default;

  private:
    std::vector<BaseWindow*> _windows;
    std::vector<BaseWindow*> _waitDestroy;
    static WindowManager* _inst;
};

#define _winsMgr WindowManager::inst()

#endif