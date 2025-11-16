#ifndef ORDER_H
#define ORDER_H

class Order {
    
    public: // Member properties
    int shares;
    double price;
    int seq; // Order's sequence number

    static int counter; // Counter that keeps track of how many orders have been made, shared globally

    // Constructor for Order
    Order(double p, int qty) {
        price = p;
        shares = qty;
        seq = counter++;
    }
};

// --------------- Definition of comparators ---------------

struct BuyComparator {
    bool operator()(const Order& a, const Order&b) const {
        if (a.price != b.price) {
            return a.price < b.price; // Highest price first
        }
        return a.seq > b.seq;         // Earliest order placed second
    }
};

struct SellComparator {
    bool operator()(const Order& a, const Order&b) const {
        if (a.price != b.price) {
            return a.price > b.price; // Lowest price first
        }
        return a.seq > b.seq;         // Earliest order placed second
    }
};

#endif // ORDER_H