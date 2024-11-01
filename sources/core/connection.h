#ifndef _CORE_CONNECTION_H_
#define _CORE_CONNECTION_H_

#include <uv.h>

#include <string>
#include <vector>
#include <functional>

class Connection {
    friend class BaseServices;

  public:
    Connection() = default;

    void setStream(uv_stream_t* stream);

    void send(const std::string& data) const;

    void connect(uv_loop_t* loop, const std::string& sockPath, std::function<void(int)> cb);

    void onMessage(std::function<void(const std::string& mesg)> cb);
    void onClose(std::function<void()> cb);

  private:
    std::vector<std::string> pushData(const std::string& data);

    static void connectCb(uv_connect_t* req, int status);
    static void newData(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf);

  private:
    uv_loop_t* _loop = nullptr;
    uv_connect_t* _connect = nullptr;
    uv_stream_t* _stream = nullptr;
    std::string _mesgBuff;
    std::function<void(int)> _cbConnect = nullptr;
    std::function<void(const std::string& mesg)> _cbMessage = nullptr;
    std::function<void()> _cbClose = nullptr;
};

#endif