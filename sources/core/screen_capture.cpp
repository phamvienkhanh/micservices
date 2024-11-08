#include "screen_capture.h"

#include <X11/Xutil.h>

#include <fstream>

ScreenCapture::~ScreenCapture() {
    if (_lastCapImg) {
        XDestroyImage(_lastCapImg);
        _lastCapImg = nullptr;
    }

    if (_display) {
        XCloseDisplay(_display);
        _display = nullptr;
    }
}

bool ScreenCapture::capture(int x, int y, int w, int h) {
    if (!_display) {
        _display = XOpenDisplay(nullptr);
        if (!_display) {
            return false;
        }

        _screen = DefaultScreen(_display);
        _rootWindow = RootWindow(_display, _screen);
    }

    _lastCapImg = XGetImage(_display, _rootWindow, x, y, w, h, AllPlanes, ZPixmap);
    if (!_lastCapImg) {
        return false;
    }

    _w = w;
    _h = h;

    return true;
}

XImage* ScreenCapture::xImg() { return _lastCapImg; }

void ScreenCapture::writePPM(const std::string& path) {
    std::ofstream ppm_file(path, std::ios::binary);
    if (!ppm_file) {
        return;
    }

    ppm_file << "P6\n";
    ppm_file << _w << " " << _h << "\n";
    ppm_file << "255\n";

    for (int y = 0; y < _h; ++y) {
        for (int x = 0; x < _w; ++x) {
            long pixel = XGetPixel(_lastCapImg, x, y);

            unsigned char r = (pixel & _lastCapImg->red_mask) >> 16;
            unsigned char g = (pixel & _lastCapImg->green_mask) >> 8;
            unsigned char b = (pixel & _lastCapImg->blue_mask);

            ppm_file.put(r);
            ppm_file.put(g);
            ppm_file.put(b);
        }
    }

    ppm_file.close();
}
