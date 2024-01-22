#include "contracts.h"
#include "Contract.h"

Contract TSEJStockContracts::get_sony() {
    Contract contract;
    contract.symbol = "6758";
    contract.secType = "STK";
    contract.currency = "JPY";
    contract.exchange = "TSEJ";
    return contract;
}

Contract TSEJStockContracts::get_one() {
    Contract contract;
    contract.symbol = "9766";
    contract.secType = "STK";
    contract.currency = "JPY";
    contract.exchange = "TSEJ";
    return contract;
}

Contract TSEJStockContracts::get_hsbc() {
    Contract contract;
    contract.symbol = "HSBA";
    contract.secType = "STK";
    contract.currency = "GBP";
    contract.exchange = "LSE";
    return contract;
}

Contract TSEJStockContracts::get_xiaomi() {
    Contract contract;
    contract.symbol = "1810";
    contract.secType = "STK";
    contract.currency = "HKD";
    contract.exchange = "SEHK";
    return contract;
}
