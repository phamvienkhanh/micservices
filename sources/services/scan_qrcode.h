#ifndef _SCAN_QRCODE_H_
#define _SCAN_QRCODE_H_

#include <imgui.h>

#include <string>

#include "core/base_window.h"
#include "core/connection.h"
#include "core/screen_capture.h"
#include "core/window_manager.h"
#include "services/qrcode.h"

class ScanQrcodeWindow : public BaseWindow {
  public:
    ScanQrcodeWindow() = default;
    void onClose() override { _winsMgr->destroyLater(this); }

    void onInit() override {}

    void onRender() override {        
        ImGui::Begin("scan qrcode", nullptr,
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
        {
            int w = 200;
            Size windSize = getSize();
            ImGui::SetWindowSize(ImVec2(w, windSize.h));
            ImGui::SetWindowPos(ImVec2(windSize.w - w, 0));
            ImVec2 fullSize = ImGui::GetContentRegionAvail();
            ImGui::TextWrapped("%s", lastResult.c_str());

            if(lastResult.length()) {
                ImVec2 btnSize(70, 30);
                ImGui::SetCursorPos(ImVec2(fullSize.x - btnSize.x, fullSize.y - btnSize.y));
                if(ImGui::Button("copy", btnSize)) {
                    ImGui::SetClipboardText(lastResult.c_str());
                }
            }
        }
        ImGui::End();
    }

    void onMove() override {
        uint64_t curTime = currentMs();
        if(curTime - lastTimeUpdate < 250) {
            return;
        }
        lastTimeUpdate = currentMs();
        
        auto pos = getPos();
        auto size = getSize();

        cap.capture(pos.x, pos.y, size.w, size.h);
        auto ximg = cap.xImg();
        QRcode qrcode;
        auto codes = qrcode.read((uint8_t*)ximg->data, ximg->width, ximg->height, ximg->depth / 8);

        lastResult = "";
        for (auto it : codes) {
            lastResult = lastResult + it + "\n";
        }
    }

    void onResize() override {}

    ScreenCapture cap;
    std::string lastResult;
    uint64_t lastTimeUpdate = 0;
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
        params.flags = {{GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE}};
        window->init(params);
        window->show();

        con.send("OK");
    }
};

#endif