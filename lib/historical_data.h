#pragma once
#include "DefaultEWrapper.h"
#include "EReaderOSSignal.h"
#include "EReader.h"

#include <memory>
#include <fstream>
#include <mutex>

class DataClient: public DefaultEWrapper {
public:
    DataClient(Contract contract, const std::string& st_str, const std::string& end_str, const std::string& output_file);
    ~DataClient() override;

    // connection
    void process_messages();
    bool connect(const char * host, int port, int client_id = 0);
    void disconnect() const;
    bool is_connected() const;

    // logging
    void error(int id, int errorCode, const std::string &errorString, const std::string &advancedOrderRejectJson) override;

    // req historical tick
    void earliest_time_stamp();
    void headTimestamp(int reqId, const std::string& headTimestamp) override;
    void req_historical_tick();
    void historicalTicksLast(int reqId, const std::vector<HistoricalTickLast>& ticks, bool done) override;
    bool is_finished();
private:
    // members
    EReaderOSSignal m_osSignal;
    EClientSocket * const m_pClient;

    OrderId m_orderId{0};
    std::unique_ptr<EReader> m_pReader;
    bool m_extraAuth{false};

    bool ready = true;
    time_t m_sleepDeadline{0};
    time_t start = 0, end = 0, req_time = 0, last_time = 0;
    bool finished = false;
    Contract contract;

    std::string m_bboExchange;
    int req_id = 0;
    std::ofstream file;

    std::mutex data_ready_mutex;
    std::mutex time_mutex;
};