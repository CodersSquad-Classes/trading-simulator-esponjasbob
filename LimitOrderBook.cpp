#include "LimitOrderBook.h"
#include <iostream>
using namespace std;

void LimitOrderBook::addBuy(double price, int quantity) {
    buy_orders.emplace(price, quantity); // Dynamically construct object and sort automatically within the queue
    match();
}

void LimitOrderBook::addSell(double price, int quantity) {
    sell_orders.emplace(price, quantity); // Dynamically construct object and sort automatically within the queue
    match();
}

void LimitOrderBook::match() {
    while (!buy_orders.empty() && !sell_orders.empty()) {
        Order buy = buy_orders.top();
        Order sell = sell_orders.top();

        // No matches possible
        if (buy.price < sell.price) {
            return; // Matches only occur when buy.price >= sell.price
        }

        // Pop from both queues
        buy_orders.pop();
        sell_orders.pop();

        // Calculate the traded shares and trade price
        int tradedShares = min(buy.shares, sell.shares); // Whichever is smaller
        double tradePrice = sell.price; // Price is set by the seller side

        // Print match details
        cout << "TRADE: " << tradedShares 
             << " @ " << tradePrice 
             << " (Buy seq " << buy.seq 
             << ", Sell seq " << sell.seq << ")\n";

        // Reinsert remaning shares to the order book
        buy.shares -= tradedShares;
        sell.shares -= tradedShares;

        if (buy.shares > 0) {
            buy_orders.push(buy);
        }

        if (sell.shares > 0) {
            sell_orders.push(sell);
        }
    }
}

void LimitOrderBook::printBook() const {
    // Insert code here...
}