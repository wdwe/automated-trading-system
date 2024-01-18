add_library(cli
        client.cpp
        contracts.cpp
)

target_link_libraries(cli
        PUBLIC tws
        PUBLIC twsutils
)

target_link_directories(cli
        PUBLIC ${CMAKE_BINARY_DIR}/tws/client
        PUBLIC ${CMAKE_BINARY_DIR}/tws/utils
)

target_include_directories(cli
        PUBLIC ${CMAKE_SOURCE_DIR}/tws/client
        PUBLIC ${CMAKE_SOURCE_DIR}/tws/utils
)