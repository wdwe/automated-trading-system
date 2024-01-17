#include <iostream>
#include "client.h"

int main() {
    Client client;
    const char* host = "";
    int port = 7497;
    client.connect(host, port, 0);
    client.get_tick_data(10001, true);
    int cnt = 0;
    while (client.is_connected()) {
        std::cout << cnt << std::endl;
        client.process_messages();
        ++cnt;
        if (cnt > 100) break;
    }
    client.cancel_tick_data(10001);
    return 0;
}
