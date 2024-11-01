
#include <iostream>

#include "services/endpoint.h"
#include "services/totp.h"
#include "uv.h"

int main(int argc, char const* argv[]) {
    uv_loop_t* loop = uv_default_loop();

    Endpoint s;
    s.setSockPath("/tmp/mics_services.sock");
    s.run(loop);

    uv_run(loop, UV_RUN_DEFAULT);

    return 0;
}
