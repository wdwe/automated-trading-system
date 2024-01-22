#include "client.h"

int main() {
    Client client;
    const char* host = "";
    int port = 7497;
    client.connect(host, port, 0);
    client.init();
    int req_id = 100001;

    client.set_hist_start_end("20240118 09:00:00", "20240118 19:51:00");
    client.req_historical_tick(req_id++);
    while (client.is_connected()) {
        client.process_messages();
        client.req_historical_tick(req_id++);
    }
    return 0;
}