#ifndef _ENDPOINT_H_
#define _ENDPOINT_H_

#include "core/base_services.h"
#include "services/router.h"

class Endpoint : public BaseServices {
  public:
    Endpoint() = default;

    void onMessage(const Connection& connection, const std::string& message) override;

  private:
    Router _router;
};

#endif