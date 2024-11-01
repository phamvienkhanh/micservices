cmake_minimum_required(VERSION 3.21)
project(app_server LANGUAGES CXX)

set(SOURCES 
    sources/main.cpp    
    sources/core/base_services.cpp
    sources/core/utils.cpp
    sources/core/connection.cpp
    sources/services/router.cpp
    sources/services/endpoint.cpp
)

add_executable(app_server ${SOURCES})
target_link_directories(app_server PRIVATE 3rd/libuv/lib)
target_include_directories(app_server 
    PRIVATE 
        3rd/libuv/include
        sources
)
target_link_libraries(app_server PRIVATE -luv -lcrypto)

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
