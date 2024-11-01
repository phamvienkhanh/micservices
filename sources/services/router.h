#ifndef _ROUTER_H_
#define _ROUTER_H_

#include <string>

#include "core/connection.h"

class Router {
  public:
    Router() = default;

    void handle(const std::string& path, const std::string& data, const Connection& con);
};

#endif