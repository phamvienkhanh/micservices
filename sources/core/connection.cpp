
#include "connection.h"

#include "log.h"
#include "types.h"
#include "utils.h"

void Connection::send(const std::string& data) const {
    if (!_stream) {
        return;
    }

    int mesgSize = data.size();
    std::string mesgFrame;
    mesgFrame.append((char*)&mesgSize, 4);
    mesgFrame.append(data);

    char* prtMesgFrame = new char[mesgFrame.size()];
    mesgFrame.copy(prtMesgFrame, mesgFrame.size());

    WriteReq* req = (WriteReq*)malloc(sizeof(WriteReq));
    req->buff = uv_buf_init(prtMesgFrame, mesgFrame.size());
    uv_write((uv_write_t*)req, _stream, &req->buff, 1, freeWriteReqCb);
}

void Connection::setStream(uv_stream_t* stream) {
    _stream = stream;
    _stream->data = this;
    uv_read_start(_stream, allocBuffer, newData);
}

void Connection::connect(uv_loop_t* loop, const std::string& sockPath, std::function<void(int)> cb) {
    _loop = loop;
    _cbConnect = cb;

    uv_connect_t* connect = (uv_connect_t*)malloc(sizeof(uv_connect_t));
    _connect = connect;
    _connect->data = this;

    uv_pipe_t* pipe = (uv_pipe_t*)malloc(sizeof(uv_pipe_t));
    _stream = (uv_stream_t*)pipe;
    _stream->data = this;

    uv_pipe_init(_loop, pipe, 0);
    uv_pipe_connect(_connect, pipe, sockPath.c_str(), connectCb);
}

void Connection::onMessage(std::function<void(const std::string& mesg)> cb) { _cbMessage = cb; }

void Connection::onClose(std::function<void()> cb) { _cbClose = cb; }

void Connection::connectCb(uv_connect_t* req, int status) {
    Connection* self = (Connection*)req->data;
    if (self) {
        if (self) {
            self->_cbConnect(status);
            uv_read_start(req->handle, allocBuffer, newData);
        }
    }
}

void Connection::newData(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf) {
    Connection* self = (Connection*)client->data;
    if (nread > 0) {
        std::string data(buf->base, nread);
        std::vector<std::string> listMesg = self->pushData(data);
        for (auto const& it : listMesg) {
            if (self->_cbMessage) {
                self->_cbMessage(it);
            }
        }

        free(buf->base);
        return;
    }

    if (nread < 0) {
        if (nread != UV_EOF) {
            fprintf(stderr, "Read error %s\n", uv_err_name(nread));
        }

        if (self->_cbClose) {
            self->_cbClose();
        }

        uv_close((uv_handle_t*)client, NULL);
        fprintf(stdout, "close connect\n");

        free(client);
        if (self->_connect) {
            free(self->_connect);
        }
    }

    free(buf->base);
}

std::vector<std::string> Connection::pushData(const std::string& data) {
    std::vector<std::string> listMesgFrame;

    _mesgBuff.append(data);

    while (1) {
        if (_mesgBuff.size() < 4) {
            break;
        }

        char* bytesBuff = _mesgBuff.data();
        int* frameSize = (int*)bytesBuff;

        if (*frameSize <= 0) {
            slog("invalid message size");
            exit(1);
        }

        if (*frameSize > _mesgBuff.size() - 4) {
            break;
        }

        std::string mesgFrame = std::string(_mesgBuff.begin() + 4, _mesgBuff.begin() + *frameSize + 4);
        listMesgFrame.push_back(mesgFrame);

        _mesgBuff.erase(_mesgBuff.begin(), _mesgBuff.begin() + *frameSize + 4);
    }

    return listMesgFrame;
}
