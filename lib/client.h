#pragma once
#include "DefaultEWrapper.h"
#include "EReaderOSSignal.h"
#include "EReader.h"

#include <memory>

class Client: public DefaultEWrapper {
public:
    Client();
    ~Client();

    // connection
    void process_messages();
    bool connect(const char * host, int port, int client_id = 0);
    void disconnect() const;
    bool is_connected() const;

    void init();

    // base class
//    #include "EWrapper_prototypes.h"
    void nextValidId(OrderId order_id);
    void tickByTickAllLast(int reqId, int tickType, time_t time, double price, Decimal size,
                           const TickAttribLast &tickAttribLast, const std::string &exchange,
                           const std::string &specialConditions) override;
    void error(int id, int errorCode, const std::string& errorString, const std::string& advancedOrderRejectJson) override;
    // operation
    // tick data
    void get_tick_data(int req_id, bool delayed=false);
    void cancel_tick_data(int req_id);
    // market data
    void get_market_data(int req_id, bool delayed);
    void cancel_market_data(int req_id);
    void tickSize(TickerId tickerId, TickType field, Decimal size) override;
    void tickPrice(TickerId tickerId, TickType field, double price, const TickAttrib& attribs) override;
    void tickString(TickerId tickerId, TickType tickType, const std::string& value) override;

    // place order
    void place_order();
    void cancel_order(int order_id);
    void openOrder(OrderId orderId, const Contract &, const Order &, const OrderState &) override;

    // req historical tick
    void set_hist_start_end(const std::string& start = "20200101 00:00:00", const std::string& end = "20200101 00:00:00");
    void req_historical_tick(int req_id);
    void historicalTicksLast(int reqId, const std::vector<HistoricalTickLast>& ticks, bool done) override;
private:
    // members
    EReaderOSSignal m_osSignal;
    EClientSocket * const m_pClient;

    time_t m_sleepDeadline;
    OrderId m_orderId;
    std::unique_ptr<EReader> m_pReader;
    bool m_extraAuth;

    bool ready = false;

    std::string m_bboExchange;
    std::time_t hist_data_last_date = 0;
    bool hist_data_finished = true;
    std::time_t start_time;
    std::time_t end_time;
};

