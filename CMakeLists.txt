cmake_minimum_required(VERSION 3.16)
project(iLinkView_v1_0_0 C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)
add_subdirectory(deps/civetweb)
add_subdirectory(deps/net-snmp)

add_library(core
    src/server.c
    src/snmp_manager.c
    src/worker_pool.c
    src/rate_limiter.c
)

target_link_libraries(core civetweb net-snmp pthread)

add_executable(app app/main.cpp app/business_logic.cpp)
target_link_libraries(app core pthread)