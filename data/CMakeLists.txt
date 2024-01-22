add_executable(acquire_data acquire_data.cpp)

target_link_libraries(acquire_data
        PUBLIC cli
)

target_link_directories(acquire_data
        PUBLIC ${CMAKE_BINARY_DIR}/lib
)

target_include_directories(acquire_data
        PUBLIC ${CMAKE_SOURCE_DIR}/lib
)
