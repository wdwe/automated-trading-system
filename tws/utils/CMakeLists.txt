add_library(twsutils
        Utils.cpp
        OrderSamples.cpp
)

target_link_libraries(twsutils
        PUBLIC tws
)

target_link_directories(twsutils
        PUBLIC ${CMAKE_BINARY_DIR}/tws/client
)

target_include_directories(twsutils
        PUBLIC ${CMAKE_SOURCE_DIR}/tws/client
)