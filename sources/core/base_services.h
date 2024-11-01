#ifndef _CORE_BASE_SERVICES_H_
#define _CORE_BASE_SERVICES_H_

#include <uv.h>

#include <string>

#include "connection.h"

class BaseServices {
  public:
    BaseServices(/* args */);
    ~BaseServices();

    void setSockPath(const std::string& sockPath);
    void run(uv_loop_t* loop = nullptr);

    virtual void onMessage(const Connection& connection, const std::string& message) = 0;

  private:
    void onNewConnection(uv_stream_t* server, int status);
    static void newConnection(uv_stream_t* server, int status);

  private:
    uv_pipe_t _serverPipe;
    uv_loop_t* _loop = nullptr;
    std::string _sockPath;

    std::vector<Connection*> _connects;
};

#endif
