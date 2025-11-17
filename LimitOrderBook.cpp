#include "LimitOrderBook.h"
#include <iostream>
#include <iomanip>
#include <vector>
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
    // ANSI colors
    const char* GREEN = "\033[32m";
    const char* RED   = "\033[31m";
    const char* CYAN  = "\033[36m";
    const char* RESET = "\033[0m";

    // Copy queues to avoid mutating the original order book
    auto buys = buy_orders;
    auto sells = sell_orders;

    // Collect top N levels (rows) from each side
    const int ROWS = 10;
    vector<Order> topBuys; topBuys.reserve(ROWS);
    vector<Order> topSells; topSells.reserve(ROWS);

    for (int i = 0; i < ROWS && !buys.empty(); ++i) {
        topBuys.push_back(buys.top());
        buys.pop();
    }
    for (int i = 0; i < ROWS && !sells.empty(); ++i) {
        topSells.push_back(sells.top());
        sells.pop();
    }

    // Header
    cout << CYAN
        << left << setw(24) << "BIDS (Buy Orders)"
        << " | "
        << left << setw(24) << "ASKS (Sell Orders)" << RESET << "\n";

    cout << left << setw(12) << "Price"
        << left << setw(12) << "Qty"
        << " | "
        << left << setw(12) << "Price"
        << left << setw(12) << "Qty" << "\n";

    cout << string(24, '-') << "-+-" << string(24, '-') << "\n";

    // Rows
    for (int i = 0; i < ROWS; ++i) {
        // Left side (Bids)
        if (i < static_cast<int>(topBuys.size())) {
            const auto& ob = topBuys[i];
            cout << GREEN
                 << left << setw(12) << fixed << setprecision(2) << ob.price
                 << left << setw(12) << ob.shares
                 << RESET;
        } else {
            cout << left << setw(12) << "-"
                 << left << setw(12) << "-";
        }

        cout << " | ";

        // Right side (Asks)
        if (i < static_cast<int>(topSells.size())) {
            const auto& oa = topSells[i];
            cout << RED
                 << left << setw(12) << fixed << setprecision(2) << oa.price
                 << left << setw(12) << oa.shares
                 << RESET;
        } else {
            cout << left << setw(12) << "-"
                 << left << setw(12) << "-";
        }

        cout << "\n";
    }
}