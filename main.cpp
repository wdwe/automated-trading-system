#include <iostream>
#include "client.h"

int main() {
    Client client;
    const char* host = "";
    int port = 7497;
    client.connect(host, port, 0);
    client.init();
    int req_id = 100001;
//    client.get_tick_data(req_id, true);
//    int cnt = 0;
//    while (client.is_connected()) {
//        std::cout << cnt << std::endl;
//        client.process_messages();
//        ++cnt;
//        if (cnt > 100) break;
//    }
//    client.cancel_tick_data(req_id);
//    client.get_market_data(req_id, false);
//    int cnt = 0;
//    while (client.is_connected()) {
//        std::cout << cnt << std::endl;
//        client.process_messages();
//        ++cnt;
//        if (cnt > 20) break;
//    }

//    client.cancel_order(3);
//    int cnt = 0;
//    while (client.is_connected()) {
//        std::cout << cnt << std::endl;
//        client.process_messages();
//        ++cnt;
// //        if (cnt > 20) break;
//    }

    client.set_hist_start_end("20240118 06:50:00", "20240118 06:51:00");
    client.req_historical_tick(req_id++);
    while (client.is_connected()) {
        client.process_messages();
        client.req_historical_tick(req_id++);
    }
    return 0;
}
