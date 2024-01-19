add_executable(quick_test
        quick_test.cpp
)

target_link_libraries(quick_test
        PUBLIC tws
)

target_link_directories(quick_test
        PUBLIC ${CMAKE_BINARY_DIR}/external/tws/client
        PUBLIC ${CMAKE_BINARY_DIR}/lib
)

target_include_directories(quick_test
        PUBLIC ${CMAKE_SOURCE_DIR}/external/tws/client
        PUBLIC ${CMAKE_SOURCE_DIR}/external/tests/TestCppClient
        PUBLIC ${CMAKE_SOURCE_DIR}/external/lib
)