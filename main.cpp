#include <iostream>
#include "LimitOrderBook.h"

using namespace std;

int main() {

    LimitOrderBook island; // Initialize LimitOrderBook object
    char cmd; // Define command variable

    // While loop
    while (true) {
        
        // Display menu and command prompt
        cout << "\n--- LIMIT ORDER BOOK ---\n";
        cout << "b: Add Buy Order\n";
        cout << "s: Add Sell Order\n";
        cout << "p: Print Book\n";
        cout << "q: Quit\n\n";
        cout << "Enter command: ";
        cin >> cmd;

        // Break condition
        if (cmd == 'q') {
            break;
        }

        // Add sell or buy orders
        else if (cmd == 'b' || cmd == 's') {
            double price;
            int qty;

            cout << "Enter price: ";
            cin >> price;
            cout << "Enter quantity: ";
            cin >> qty;

            // Buy orders
            if (cmd == 'b') {
                island.addBuy(price, qty);
            } 
            
            // Sell orders
            else {
                island.addSell(price, qty);
            }


        }

        // Print order book
        else if (cmd == 'p') {
            island.printBook();
        }
    }
    
    return 0;
}