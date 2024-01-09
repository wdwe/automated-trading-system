cmake_minimum_required(VERSION 3.27)
project(trading)

set(CMAKE_CXX_STANDARD 20)
#enable_testing()

add_executable(${PROJECT_NAME} main.cpp)
add_subdirectory(tws/client)
add_subdirectory(tests)