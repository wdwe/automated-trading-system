add_library(cli
        client.cpp
        contracts.cpp
        historical_data.cpp
)

target_link_libraries(cli
        PUBLIC tws
        PUBLIC twsutils
)

target_link_directories(cli
        PUBLIC ${CMAKE_BINARY_DIR}/external/tws/client
        PUBLIC ${CMAKE_BINARY_DIR}/external/tws/utils
)

target_include_directories(cli
        PUBLIC ${CMAKE_SOURCE_DIR}/external/tws/client
        PUBLIC ${CMAKE_SOURCE_DIR}/external/tws/utils
)