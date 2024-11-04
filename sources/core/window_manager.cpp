#include "window_manager.h"

#include <algorithm>

#include "core/base_window.h"

WindowManager* WindowManager::_inst = nullptr;

void WindowManager::addWindow(BaseWindow* window) { _windows.push_back(window); }

void WindowManager::removeWindow(BaseWindow* window) { auto _ = std::remove(_windows.begin(), _windows.end(), window); }

void WindowManager::renderAllWindow() {
    for (const auto& it : _windows) {
        it->render();
    }
}

void WindowManager::shouldCloseWindow() {
    for (auto it = _windows.begin(); it != _windows.end();) {
        if ((*it)->shouldClose()) {
            it = _windows.erase(it);
            (*it)->onClose();
        } else {
            ++it;
        }
    }
}

int WindowManager::countWindows() { return _windows.size(); }

void WindowManager::shouldDestroy() {
    for (BaseWindow* it : _waitDestroy) {
        delete it;
    }

    _waitDestroy.clear();
}

void WindowManager::destroyLater(BaseWindow* window) { _waitDestroy.push_back(window); }

WindowManager* WindowManager::inst() {
    if (!_inst) _inst = new WindowManager;
    return _inst;
}
