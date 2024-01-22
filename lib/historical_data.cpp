#include "historical_data.h"
#include "client.h"
#include "EClientSocket.h"
#include "contracts.h"
#include <iostream>
#include <ctime>
#include <memory>
#include "Utils.h"
#include <ios>
#include "Decimal.h"
#include "OrderSamples.h"
#include "Order.h"
#include "OrderState.h"
#include <thread>
#include <utility>

DataClient::DataClient(Contract contract, const std::string& st_str, const std::string& end_str, const std::string& output_file):
        m_osSignal(2000), //2-seconds timeout
        m_pClient(new EClientSocket(this, &m_osSignal)),
        contract(std::move(contract)) {
    std::tm tm{};
    strptime(st_str.c_str(), "%Y%m%d-%H:%M:%S", &tm);
    start = std::mktime(&tm); // TODO: consider time zone
    req_time = start;
    last_time = req_time - 1;
    strptime(end_str.c_str(), "%Y%m%d-%H:%M:%S", &tm);
    end = std::mktime(&tm);
    file.open(output_file, std::ios::out | std::ios::app);
}

DataClient::~DataClient() {
    if (m_pReader)
        m_pReader.reset();

    delete m_pClient;
}

/*
 * Logging
 */
void DataClient::error(int id, int errorCode, const std::string &errorString, const std::string &advancedOrderRejectJson) {
    std::cout << "[id]:" << id << " [errorCode]:" << errorCode << " [errorMsg]:" << errorString << " [advancedOrderRejectJson]:" << advancedOrderRejectJson << std::endl;
}

/*
 * connection
 */
bool DataClient::connect(const char *host, int port, int clientId)
{
    // trying to connect
    printf( "Connecting to %s:%d clientId:%d\n", !( host && *host) ? "127.0.0.1" : host, port, clientId);

    bool bRes = m_pClient->eConnect( host, port, clientId, m_extraAuth);

    if (bRes) {
        printf( "Connected to %s:%d clientId:%d\n", m_pClient->host().c_str(), m_pClient->port(), clientId);
        m_pReader = std::make_unique<EReader>( m_pClient, &m_osSignal );
        m_pReader->start();
    }
    else
        printf( "Cannot connect to %s:%d clientId:%d\n", m_pClient->host().c_str(), m_pClient->port(), clientId);

    return bRes;
}

void DataClient::disconnect() const
{
    m_pClient->eDisconnect();

    printf ( "Disconnected\n");
}

bool DataClient::is_connected() const
{
    return m_pClient->isConnected();
}

/*
 * Data acquisition
 */
void DataClient::earliest_time_stamp() {
    m_pClient->reqHeadTimestamp(req_id++, contract, "TRADES", 1, 1);
}

void DataClient::headTimestamp(int reqId, const std::string& headTimestamp) {
    std::cout << "headTimestamp for " << contract.symbol << " is " << headTimestamp << std::endl;
}


void DataClient::req_historical_tick() {
    if (!ready) return;
    if (last_time == req_time) {
        std::cout << "no more data available" << std::endl;
        finished = true;
    }
    if (req_time > end) {
        std::cout << "completed data range" << std::endl;
        finished = true;
    }
    if (finished) return;

    last_time = req_time;
    data_ready_mutex.lock();
    ready = false;
    data_ready_mutex.unlock();

    char time_str[std::size("yyyymmdd-hh::mm::ss")];
    std::tm tm = *std::gmtime(&req_time);
    strftime(time_str, std::size(time_str), "%Y%m%d-%H:%M:%S", &tm);
    std::cout << time_str << std::endl;
    m_pClient->reqHistoricalTicks(req_id++, contract, time_str, "", 1000, "TRADES", 1, true, nullptr);

}

void DataClient::historicalTicksLast(int reqId, const std::vector<HistoricalTickLast>& ticks, bool done) {
    for (auto &tick: ticks) {
        if (tick.time > end) continue;
        char time_str[std::size("yyyymmdd-hh:mm:ss")];
        auto time = static_cast<std::time_t>(tick.time);
        auto ptm = std::localtime(&time); // TODO: time zone
        std::strftime(time_str, std::size(time_str), "%Y%m%d-%H:%M:%S", ptm);
        file << reqId << "," << tick.time << "," << time_str << "," << tick.price << "," << decimalStringToDisplay(tick.size) <<
             "," << tick.exchange << "," << tick.specialConditions << std::endl;
        std::cout << reqId << ", " << tick.time << ", " << time_str << ", " << tick.price << ", " << decimalStringToDisplay(tick.size) <<
                  ", " << tick.exchange << ", " << tick.specialConditions << std::endl;
    }
    if (!ticks.empty()) {
        time_mutex.lock();
        req_time = ticks.back().time + 1;
        time_mutex.unlock();
    }
    if (done) {
        data_ready_mutex.lock();
        ready = true;
        data_ready_mutex.unlock();
//        std::cout << "empty? " << std::boolalpha << !ticks.empty() << std::endl;
    }
}

void DataClient::process_messages() {
    m_osSignal.waitForSignal();
    errno = 0;
    m_pReader->processMsgs();
}

bool DataClient::is_finished() {
    return finished;
}




