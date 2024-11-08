#ifndef _SCREEN_CAPTURE_H_
#define _SCREEN_CAPTURE_H_

#include <X11/Xlib.h>

#include <string>

class ScreenCapture {
  public:
    ScreenCapture() = default;
    ~ScreenCapture();

    bool capture(int x, int y, int w, int h);
    XImage* xImg();

    void writePPM(const std::string& path);

  private:
    Display* _display = nullptr;
    int _screen = 0;
    Window _rootWindow = 0;
    XImage* _lastCapImg = nullptr;
    int _w = 0;
    int _h = 0;
};

#endif