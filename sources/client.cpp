
#include <condition_variable>
#include <mutex>
#include <thread>

#include "core/connection.h"
#include "core/log.h"

int main(int argc, char const* argv[]) {
    uv_loop_t* loop = uv_default_loop();

    std::mutex mutex;
    std::condition_variable cv;
    bool ready = false;

    Connection client;
    client.connect(loop, "/tmp/mics_services.sock", [&](int stt) {
        slog("connect status " << stt);

        std::lock_guard<std::mutex> lock(mutex);
        ready = true;
        cv.notify_one();
    });

    client.onMessage([&mutex, &ready, &cv](const std::string& mesg) {
        std::cout << "resp : " << mesg << "\n";
        std::lock_guard<std::mutex> lock(mutex);
        ready = true;
        cv.notify_one();
    });

    bool exit = false;
    std::thread t([&]() {
        while (!exit) {
            std::unique_lock<std::mutex> lock(mutex);
            ready = false;
            cv.wait(lock, [&ready]() { return ready; });

            std::string mesg;
            std::cout << "send: ";
            std::getline(std::cin, mesg);
            client.send(mesg);
        }
    });

    uv_run(loop, UV_RUN_DEFAULT);
    exit = true;
    t.join();
    return 0;
}
