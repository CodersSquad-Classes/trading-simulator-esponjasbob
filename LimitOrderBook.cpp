#include "LimitOrderBook.h"
#include <iostream>
#include <iomanip>
using namespace std;

void LimitOrderBook::addBuy(double price, int quantity) {
    buy_orders.emplace(price, quantity);
    match();
}

void LimitOrderBook::addSell(double price, int quantity) {
    sell_orders.emplace(price, quantity);
    match();
}

double LimitOrderBook::getSpread() const {
    if (buy_orders.empty() || sell_orders.empty()) return 0;
    return sell_orders.top().price - buy_orders.top().price;
}

void LimitOrderBook::match() {
    while (!buy_orders.empty() && !sell_orders.empty()) {

        Order buy = buy_orders.top();
        Order sell = sell_orders.top();

        if (buy.price < sell.price) return;

        buy_orders.pop();
        sell_orders.pop();

        int traded = min(buy.shares, sell.shares);
        double price = sell.price;

        lastPrice = price;
        totalVolume += traded;
        totalTrades++;

        // simple tape (only 5 trades)
        string t = to_string(traded) + " @ " + to_string(price);

        if (tapeCount < 5) {
            tape[tapeCount] = t;
            tapeCount++;
        } else {
            for (int i = 4; i > 0; i--)
                tape[i] = tape[i - 1];
            tape[0] = t;
        }

        buy.shares -= traded;
        sell.shares -= traded;

        if (buy.shares > 0) buy_orders.push(buy);
        if (sell.shares > 0) sell_orders.push(sell);
    }
}

void LimitOrderBook::printTape() const {
    if (tapeCount == 0) {
        cout << "  No trades yet.\n";
        return;
    }

    for (int i = 0; i < tapeCount; i++)
        cout << "  " << tape[i] << "\n";
}

void LimitOrderBook::printBook() const {
    auto buys = buy_orders;
    auto sells = sell_orders;

    cout << left << setw(12) << "BUY Qty" 
         << setw(12) << "BUY Px"
         << " | "
         << setw(12) << "SELL Qty"
         << setw(12) << "SELL Px" << "\n";

    cout << string(28, '-') << "+-" << string(28, '-') << "\n";

    for (int i = 0; i < 10; i++) {

        if (!buys.empty()) {
            Order b = buys.top(); buys.pop();
            cout << setw(12) << b.shares
                 << setw(12) << b.price;
        } else {
            cout << setw(12) << "-"
                 << setw(12) << "-";
        }

        cout << " | ";

        if (!sells.empty()) {
            Order s = sells.top(); sells.pop();
            cout << setw(12) << s.shares
                 << setw(12) << s.price;
        } else {
            cout << setw(12) << "-"
                 << setw(12) << "-";
        }

        cout << "\n";
    }
}
