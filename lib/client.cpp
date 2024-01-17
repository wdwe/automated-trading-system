#include "client.h"
#include "EClientSocket.h"
#include "contracts.h"
#include <iostream>
#include <ctime>
#include <memory>

Client::Client():
    m_osSignal(2000), //2-seconds timeout
    m_pClient(new EClientSocket(this, &m_osSignal)),
    m_sleepDeadline(0),
    m_orderId(0),
    m_extraAuth(false) {

}

Client::~Client() {
    if (m_pReader)
        m_pReader.reset();

    delete m_pClient;
}

bool Client::connect(const char *host, int port, int clientId)
{
    // trying to connect
    printf( "Connecting to %s:%d clientId:%d\n", !( host && *host) ? "127.0.0.1" : host, port, clientId);

    //! [connect]
    bool bRes = m_pClient->eConnect( host, port, clientId, m_extraAuth);
    //! [connect]

    if (bRes) {
        printf( "Connected to %s:%d clientId:%d\n", m_pClient->host().c_str(), m_pClient->port(), clientId);
        //! [ereader]
        m_pReader = std::make_unique<EReader>( m_pClient, &m_osSignal );
        m_pReader->start();
        //! [ereader]
    }
    else
        printf( "Cannot connect to %s:%d clientId:%d\n", m_pClient->host().c_str(), m_pClient->port(), clientId);

    return bRes;
}

void Client::disconnect() const
{
    m_pClient->eDisconnect();

    printf ( "Disconnected\n");
}

bool Client::is_connected() const
{
    return m_pClient->isConnected();
}

void Client::process_messages() {
    m_osSignal.waitForSignal();
    errno = 0;
    m_pReader->processMsgs();
}

void Client::nextValidId(OrderId order_id) {
    m_orderId = order_id;
    std::cout << order_id << std::endl;
}


void Client::error(int id, int errorCode, const std::string &errorString, const std::string &advancedOrderRejectJson) {
    std::cout << "id:" << id << " errorCode: " << errorCode << " errorMsg: " << errorString;
}

void Client::get_tick_data(int req_id, bool delayed) {
    if (delayed)
        m_pClient->reqMarketDataType(4);
    m_pClient->reqTickByTickData(req_id, TSEJStockContracts::get_sony(), "AllLast", 10, false);
}

void Client::cancel_tick_data(int req_id) {
    m_pClient->cancelTickByTickData(req_id);
}

void Client::tickByTickAllLast(int reqId, int tickType, time_t time, double price, Decimal size,
                               const TickAttribLast &tickAttribLast, const std::string &exchange,
                               const std::string &specialConditions) {
    std::cout << "all last" << std::endl;
    char time_str[100];
    std::time_t now = std::time(nullptr);
    std::tm * ptm = std::localtime(&now);
    std::cout << "reqId: " << reqId << " tickType: " << tickType << " time: " << std::strftime(time_str, 100, "%d/%m/%Y %T", ptm)
    << " price: " << price << " size: " << size << " exchange: " << exchange << std::endl;
}





