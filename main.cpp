#include <iostream>
#include <random>
#include <thread>
#include <chrono>
#include "LimitOrderBook.h"

using namespace std;

void addRandomOrder(LimitOrderBook& lob) {
    static random_device rd;
    static mt19937 gen(rd());

    uniform_int_distribution<int> type(1, 2);
    uniform_int_distribution<int> qty(1, 6000);
    uniform_real_distribution<double> price(23.0, 25.0);

    int t = type(gen);

    if (t == 1)
        lob.addBuy(price(gen), qty(gen));
    else
        lob.addSell(price(gen), qty(gen));
}

int main() {

    LimitOrderBook island;

    double prevLast = 0;

    while (true) {

        cout << "\033[2J\033[H"; // limpiar pantalla

        cout << "=== REAL-TIME DASHBOARD ===\n\n";

        cout << "Last Price: ";
        double lp = island.getLastPrice();

        if (lp == 0) {
            cout << "N/A\n";
        } else if (lp > prevLast) {
            cout << "\033[32m▲ " << lp << "\033[0m\n";
        } else if (lp < prevLast) {
            cout << "\033[31m▼ " << lp << "\033[0m\n";
        } else {
            cout << lp << "\n";
        }

        prevLast = lp;

        cout << "Trades:  " << island.getTrades() << "\n";
        cout << "Volume:  " << island.getVolume() << "\n";
        cout << "Spread:  " << island.getSpread() << "\n";

        cout << "\nRecent Trades:\n";
        island.printTape();

        cout << "\nOrder Book:\n\n";
        island.printBook();

        addRandomOrder(island);

        this_thread::sleep_for(chrono::seconds(1));
    }

    return 0;
}
