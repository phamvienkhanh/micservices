cmake_minimum_required(VERSION 3.21)
project(app_server LANGUAGES CXX)

set(SOURCES 
    sources/main.cpp    
    3rd/stb/stb_image.cpp
    sources/core/base_services.cpp
    sources/core/window_manager.cpp
    sources/core/base_window.cpp
    sources/core/screen_capture.cpp
    sources/core/utils.cpp
    sources/core/connection.cpp
    sources/services/router.cpp
    sources/services/endpoint.cpp

    3rd/imgui/imgui_draw.cpp
    3rd/imgui/imgui.cpp
    3rd/imgui/backends/imgui_impl_opengl3.cpp
    3rd/imgui/backends/imgui_impl_glfw.cpp
    3rd/imgui/imgui_tables.cpp
    3rd/imgui/imgui_widgets.cpp
)

add_executable(app_server ${SOURCES})
target_link_directories(app_server PRIVATE 3rd/libuv/lib 3rd/zxing/lib)
target_include_directories(app_server 
    PRIVATE 
        3rd/libuv/include
        3rd/zxing/include
        3rd/imgui
        3rd/imgui/backends
        3rd/stb
        sources
)
target_link_libraries(app_server PRIVATE -luv -lcrypto -lZXing -lglfw -lGL -lX11)

add_executable(client 
    sources/client.cpp 
    sources/core/utils.cpp
    sources/core/connection.cpp
)
target_link_directories(client PRIVATE 3rd/libuv/lib)
target_include_directories(client 
    PRIVATE 
        3rd/libuv/include
        sources
)
target_link_libraries(client PRIVATE -luv)
