#include <iostream>
#include <limits> // For numeric_limits
#include "LimitOrderBook.h"

using namespace std;

int main() {

    LimitOrderBook island; // Initialize LimitOrderBook object
    char cmd; // Define command variable

    // While loop
    while (true) {
        // Clear screen and print dashboard
        cout << "\033[2J\033[H"; // ANSI clear screen & move cursor home
        cout << "--- LIMIT ORDER BOOK ---\n\n";
        island.printBook();
        
        // Display menu and command prompt
        cout << "\nCommands:\n";
        cout << "b: Add Buy Order\n";
        cout << "s: Add Sell Order\n";
        cout << "p: Print Book (snapshot)\n";
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
            // Print a snapshot without clearing dashboard
            cout << "\n";
            island.printBook();
            cout << "\nPress Enter to continue...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
    }
    
    return 0;
}