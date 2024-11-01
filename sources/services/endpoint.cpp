#include "endpoint.h"

#include "core/log.h"
#include "core/utils.h"

void Endpoint::onMessage(const Connection& connection, const std::string& message) {
    slog("UtilsServices from : " << (Connection*)&connection << " : " << message);

    auto inputs = strSplitFirts(message, ' ');
    if (inputs.empty()) {
        return;
    }

    auto path = inputs[0];
    auto data = inputs.size() > 1 ? inputs[1] : "";

    _router.handle(path, data, connection);
}
