#include "router.h"

#include "core/utils.h"
#include "services/qrcode.h"
#include "services/scan_qrcode.h"
#include "services/totp.h"

void Router::handle(const std::string& path, const std::string& data, const Connection& con) {
    auto parts = strSplit(path, '/');

    if (parts.empty()) {
        return;
    }

    auto servicesName = toLower(parts[0]);

    if (servicesName == "totp") {
        TOTP().handle(data, con);
    } else if (servicesName == "qrcode") {
        QRcode().handle(data, con);
    } else if (servicesName == "scan") {
        ScanQrcode().handle(data, con);
    }
}