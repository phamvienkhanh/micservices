
#include <GLFW/glfw3.h>

#include <thread>

#include "core/window_manager.h"
#include "services/endpoint.h"
#include "uv.h"

int main(int argc, char const* argv[]) {
    if (!glfwInit()) {
        return -1;
    }

    uv_loop_t* loop = uv_default_loop();

    Endpoint s;
    s.setSockPath("/tmp/mics_services.sock");
    s.run(loop);

    while (1) {
        _winsMgr->shouldDestroy();

        uv_run(loop, UV_RUN_NOWAIT);

        glfwPollEvents();

        _winsMgr->renderAllWindow();
        _winsMgr->shouldCloseWindow();

        if (!_winsMgr->countWindows()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }

    uv_loop_close(loop);

    glfwTerminate();

    return 0;
}
