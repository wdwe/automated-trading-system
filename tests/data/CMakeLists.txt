add_executable(test_data
    test_data.cpp
)

target_link_libraries(test_data
        PUBLIC cli
)

target_link_directories(test_data
        PUBLIC ${CMAKE_BINARY_DIR}/lib
)

target_include_directories(test_data
        PUBLIC ${CMAKE_SOURCE_DIR}/lib
)