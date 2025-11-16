#ifndef LIMITORDERBOOK_H
#define LIMITORDERBOOK_H

#include <queue>
#include <vector>
#include "Order.h"

class LimitOrderBook {
    private:
    std::priority_queue<Order, std::vector<Order>, BuyComparator> buy_orders;
    std::priority_queue<Order, std::vector<Order>, SellComparator> sell_orders;

    public:
    void addBuy(double price, int quantity);
    void addSell(double price, int quantity);
    void match(); // Automatically matches after inserting

    void printBook() const;
};

#endif // LIMITORDERBOOK_H