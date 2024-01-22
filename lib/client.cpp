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
#include <chrono>
#include <thread>
#include <fstream>


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

void Client::init() {
    m_pClient->reqIds(0);
    while (!ready) {
        process_messages();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void Client::process_messages() {
    m_osSignal.waitForSignal();
    errno = 0;
    m_pReader->processMsgs();
}

void Client::nextValidId(OrderId order_id) {
    ready = true;
    m_orderId = order_id;
    std::cout << order_id << std::endl;
}


void Client::error(int id, int errorCode, const std::string &errorString, const std::string &advancedOrderRejectJson) {
    std::cout << "id:" << id << " errorCode: " << errorCode << " errorMsg: " << errorString << " advancedOrderRejectJson: " << advancedOrderRejectJson << std::endl;
}

void Client::get_market_data(int req_id, bool delayed) {
    auto contract = TSEJStockContracts::get_xiaomi();
    if (delayed)
        m_pClient->reqMarketDataType(4);
    else
        m_pClient->reqMarketDataType(1);
    m_pClient->reqMarketDataType(4); // send delayed-frozen (4) market data type
    m_pClient->reqMktData(req_id, contract, "", false, false, TagValueListSPtr());
}

void Client::cancel_market_data(int req_id) {
    m_pClient->cancelMktData(req_id);
}


void Client::get_tick_data(int req_id, bool delayed) {
    auto contract = TSEJStockContracts::get_sony();
    if (delayed)
        m_pClient->reqMarketDataType(4);
    else
        m_pClient->reqMarketDataType(1);

    m_pClient->reqTickByTickData(req_id, contract, "Last", 100, false);

}

void Client::cancel_tick_data(int req_id) {
    m_pClient->cancelTickByTickData(req_id);
}

void Client::tickByTickAllLast(int reqId, int tickType, time_t time, double price, Decimal size,
                               const TickAttribLast &tickAttribLast, const std::string &exchange,
                               const std::string &specialConditions) {
    char time_str[std::size("yyyy-mm-ddThh:mm:ssZ")];
//    std::time_t now = std::time(&time);
    std::tm * ptm = std::gmtime(&time);
    std::strftime(time_str, std::size(time_str), "%FT%TZ", ptm);
    std::cout << "reqId: " << reqId << " tickType: " << tickType << " time: " << time_str << " price: " << price
              << " size: " << decimalStringToDisplay(size) << " exchange: " << exchange.c_str() << std::endl;
}

void Client::tickPrice(TickerId tickerId, TickType field, double price, const TickAttrib &attribs) {
    std::cout << "Tick Price - " << "Ticker Id:" << tickerId << " Field:" <<  field << " Price:" << Utils::doubleMaxString(price).c_str()
    << std::boolalpha << " CanAutoExecute: " << attribs.canAutoExecute << " PastLimit: " << attribs.pastLimit << " PreOpen: " << attribs.preOpen << std::endl;
//    printf( "Tick Price. Ticker Id: %ld, Field: %d, Price: %s, CanAutoExecute: %d, PastLimit: %d, PreOpen: %d\n", tickerId, (int)field, Utils::doubleMaxString(price).c_str(), attribs.canAutoExecute, attribs.pastLimit, attribs.preOpen);
}

void Client::tickString(TickerId tickerId, TickType tickType, const std::string& value) {
    std::cout << "Tick String - Ticker Id:" << tickerId << " Type:" << tickType << " value:" << value << std::endl;
}

void Client::tickSize(TickerId tickerId, TickType field, Decimal size) {
    std::cout << "Tick Size - Ticker Id:" << tickerId << " Type " << field << " Size: " << decimalStringToDisplay(size) << std::endl;
}

void Client::place_order() {
    std::cout << "m_orderId is:" << m_orderId << std::endl;
    m_pClient->placeOrder(m_orderId++, TSEJStockContracts::get_sony(), OrderSamples::LimitOrder("BUY", stringToDecimal("100"), 10.4));
    // OpenOrder. PermId: 741901835, ClientId: 0, OrderId: 3, Account: DU8320128, Symbol: 6758, SecType: STK, Exchange: SMART:, Action: BUY, OrderType:LMT, TotalQty: 100, CashQty: 0, LmtPrice: 10.4, AuxPrice: 0, Status: Submitted, MinTradeQty: 0, MinCompeteSize: 100, CompeteAgainstBestOffset: 0.02, MidOffsetAtWhole: 0, MidOffsetAtHalf: 0, FAGroup: , FAMethod:
}

void Client::openOrder( OrderId orderId, const Contract& contract, const Order& order, const OrderState& orderState) {
    printf( "OpenOrder. PermId: %s, ClientId: %s, OrderId: %s, Account: %s, Symbol: %s, SecType: %s, Exchange: %s:, Action: %s, OrderType:%s, TotalQty: %s, CashQty: %s, "
            "LmtPrice: %s, AuxPrice: %s, Status: %s, MinTradeQty: %s, MinCompeteSize: %s, CompeteAgainstBestOffset: %s, MidOffsetAtWhole: %s, MidOffsetAtHalf: %s, "
            "FAGroup: %s, FAMethod: %s\n",
            Utils::intMaxString(order.permId).c_str(), Utils::longMaxString(order.clientId).c_str(), Utils::longMaxString(orderId).c_str(), order.account.c_str(), contract.symbol.c_str(),
            contract.secType.c_str(), contract.exchange.c_str(), order.action.c_str(), order.orderType.c_str(), decimalStringToDisplay(order.totalQuantity).c_str(),
            Utils::doubleMaxString(order.cashQty).c_str(), Utils::doubleMaxString(order.lmtPrice).c_str(), Utils::doubleMaxString(order.auxPrice).c_str(), orderState.status.c_str(),
            Utils::intMaxString(order.minTradeQty).c_str(), Utils::intMaxString(order.minCompeteSize).c_str(),
            order.competeAgainstBestOffset == COMPETE_AGAINST_BEST_OFFSET_UP_TO_MID ? "UpToMid" : Utils::doubleMaxString(order.competeAgainstBestOffset).c_str(),
            Utils::doubleMaxString(order.midOffsetAtWhole).c_str(), Utils::doubleMaxString(order.midOffsetAtHalf).c_str(),
            order.faGroup.c_str(), order.faMethod.c_str());
}

void Client::cancel_order(int order_id) {
    m_pClient->cancelOrder(order_id, "");

}

void Client::req_historical_tick(int req_id) {
    if (hist_data_finished) {
        hist_data_finished = false; // FIXME: can have a race condition with historicalTicksLast
        time_t next_time = std::max(start_time, hist_data_last_date + 1);
        std::cout << next_time << std::endl;
        char time_str[std::size("yyyymmdd-HH:MM:SSz")]; //std::size("yyymmdd-HH:MM:SS")
        std::tm * ptm = std::gmtime(&next_time);
        strftime(time_str, std::size(time_str), "%Y%m%d-%H:%M:%S", ptm);
        std::cout << time_str << std::endl;
        if (next_time < end_time) {
            m_pClient->reqHistoricalTicks(req_id, TSEJStockContracts::get_sony(), time_str, "", 1000, "TRADES", 1, true, nullptr);
        }
    }

}

void Client::historicalTicksLast(int reqId, const std::vector<HistoricalTickLast> &ticks, bool done) {
    std::ofstream file;
    file.open("data.csv", std::ios_base::out | std::ios_base::app);

    for (auto &tick: ticks) {
        if (tick.time > end_time) continue;
        char time_str[std::size("yyyymmdd-hh:mm:ss")];
        auto time = static_cast<std::time_t>(tick.time);
        auto ptm = std::localtime(&time);
        std::strftime(time_str, std::size(time_str), "%Y%m%d-%H:%M:%S", ptm);
        file << reqId << ", " << tick.time << ", " << time_str << ", " << tick.price << ", " << decimalStringToDisplay(tick.size) <<
            ", " << tick.exchange << ", " << tick.specialConditions << std::endl;
        std::cout << reqId << ", " << tick.time << ", " << time_str << ", " << tick.price << ", " << decimalStringToDisplay(tick.size) <<
                     ", " << tick.exchange << ", " << tick.specialConditions << std::endl;
    }
    file.close();
    if (done && !ticks.empty()) {
        hist_data_last_date = ticks.back().time;
        hist_data_finished = true;
    }
}

void Client::set_hist_start_end(const std::string& start, const std::string& end) {
    std::tm tm{};
    strptime(start.c_str(), "%Y%m%d %H:%M:%S", &tm);
    start_time = mktime(&tm);
    strptime(end.c_str(), "%Y%m%d %H:%M:%S", &tm);
    end_time = mktime(&tm);
    std::cout << "start: " << start_time << std::endl;
}







