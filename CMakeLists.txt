cmake_minimum_required(VERSION 3.27)
project(trading)

set(CMAKE_CXX_STANDARD 20)
#enable_testing()

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC cli

)

target_link_directories(${PROJECT_NAME}
        PUBLIC ${CMAKE_BINARY_DIR}/lib
)

target_include_directories(${PROJECT_NAME}
        PUBLIC ${CMAKE_SOURCE_DIR}/lib
)

add_subdirectory(lib)
add_subdirectory(external)
#add_subdirectory(external/tws/utils)
add_subdirectory(tests)