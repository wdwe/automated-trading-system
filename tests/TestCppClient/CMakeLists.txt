add_executable(tws_client_tester
        AccountSummaryTags.cpp
        AvailableAlgoParams.cpp
        ContractSamples.cpp
        Main.cpp
        OrderSamples.cpp
        ScannerSubscriptionSamples.cpp
        StdAfx.cpp
        TestCppClient.cpp
        Utils.cpp
)

target_link_libraries(tws_client_tester
        PUBLIC tws
)

target_link_directories(tws_client_tester
        PUBLIC ${CMAKE_BINARY_DIR}/tws/client
)

target_include_directories(tws_client_tester
        PUBLIC ${CMAKE_SOURCE_DIR}/tws/client
        PUBLIC ${CMAKE_SOURCE_DIR}/tests/TestCppClient
)