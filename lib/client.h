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

    // base class
//    #include "EWrapper_prototypes.h"
    void nextValidId(OrderId order_id);
    void tickByTickAllLast(int reqId, int tickType, time_t time, double price, Decimal size,
                           const TickAttribLast &tickAttribLast, const std::string &exchange,
                           const std::string &specialConditions) override;
    void error(int id, int errorCode, const std::string& errorString, const std::string& advancedOrderRejectJson) override;
    // operation
    void get_tick_data(int req_id, bool delayed=false);
    void cancel_tick_data(int req_id);
private:

    // members
    EReaderOSSignal m_osSignal;
    EClientSocket * const m_pClient;
    time_t m_sleepDeadline;

    OrderId m_orderId;
    std::unique_ptr<EReader> m_pReader;
    bool m_extraAuth;
    std::string m_bboExchange;

};

