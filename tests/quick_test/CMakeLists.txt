add_executable(quick_test
        quick_test.cpp
)

find_package(CURL REQUIRED)

target_link_libraries(quick_test
        PUBLIC tws
        PUBLIC tz
        ${CURL_LIBRARIES}
)

target_link_directories(quick_test
        PUBLIC ${CMAKE_BINARY_DIR}/external/tws/client
        PUBLIC ${CMAKE_BINARY_DIR}/lib
        PUBLIC ${CMAKE_BINARY_DIR}/external/date
)

target_include_directories(quick_test
        PUBLIC ${CMAKE_SOURCE_DIR}/external/tws/client
        PUBLIC ${CMAKE_SOURCE_DIR}/external/tests/TestCppClient
        PUBLIC ${CMAKE_SOURCE_DIR}/external/date/include
        PUBLIC ${CURL_INCLUDE_DIR}
)
