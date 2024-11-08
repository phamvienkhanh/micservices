#ifndef _CORE_TYPES_H_
#define _CORE_TYPES_H_

#include <uv.h>

#include <iostream>

struct WriteReq {
    uv_write_t req;
    uv_buf_t buff;
};

struct Position {
    int x, y;
};

struct Size {
    int w, h;
};

#endif