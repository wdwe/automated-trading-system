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

Contract TSEJStockContracts::get_hsbc() {
    Contract contract;
    contract.symbol = "HSBA";
    contract.secType = "STK";
    contract.currency = "GBP";
    contract.exchange = "LSE";
    return contract;
}
