#ifndef _SCAN_QRCODE_H_
#define _SCAN_QRCODE_H_

#include <string>

#include "core/base_window.h"
#include "core/connection.h"
#include "core/window_manager.h"

class ScanQrcodeWindow : public BaseWindow {
  public:
    ScanQrcodeWindow() = default;
    void onClose() override { _winsMgr->destroyLater(this); }

    void onRender() override {}
};

class ScanQrcode {
  public:
    ScanQrcode() = default;

    void handle(const std::string& data, const Connection& con) {
        ScanQrcodeWindow* window = new ScanQrcodeWindow;
        BaseWindow::InitParams params;
        params.width = 480;
        params.height = 320;
        params.title = "Scan Qrcode";
        window->init(params);
        window->show();
    }
};

#endif