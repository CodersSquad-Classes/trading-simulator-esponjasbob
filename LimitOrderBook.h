#ifndef LIMITORDERBOOK_H
#define LIMITORDERBOOK_H

#include <queue>
#include <string>
#include "Order.h"
#include <vector>

class LimitOrderBook {

private:
    std::priority_queue<Order, std::vector<Order>, BuyComparator> buy_orders;
    std::priority_queue<Order, std::vector<Order>, SellComparator> sell_orders;

    double lastPrice = 0;
    int totalVolume = 0;
    int totalTrades = 0;

    // simple tape storage (max 5 trades)
    std::string tape[5];
    int tapeCount = 0;

public:
    void addBuy(double price, int quantity);
    void addSell(double price, int quantity);

    void match();

    void printBook() const;
    void printTape() const;

    double getLastPrice() const { return lastPrice; }
    int getTrades() const { return totalTrades; }
    int getVolume() const { return totalVolume; }

    double getSpread() const;
};

#endif
