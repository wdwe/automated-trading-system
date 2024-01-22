#include <iostream>
#include "historical_data.h"
#include "contracts.h"


int main() {
    DataClient client(TSEJStockContracts::get_one(), "20230601-01:00:00", "20240120-00:00:00", "sony.csv");
    const char* host = "";
    int port = 7497;
    client.connect(host, port, 0);
    client.earliest_time_stamp();

    while(client.is_connected()) {
        client.req_historical_tick();
        client.process_messages();
        if (client.is_finished()) client.disconnect();
    }

    return 0;
}
