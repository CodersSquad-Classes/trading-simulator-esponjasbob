#ifndef ORDER_H
#define ORDER_H

class Order {
public:
    int shares;
    double price;
    int seq;
    static int counter;

    Order(double p = 0, int q = 0) {
        price = p;
        shares = q;
        seq = counter++;
    }
};

struct BuyComparator {
    bool operator()(const Order& a, const Order& b) const {
        if (a.price != b.price)
            return a.price < b.price;
        return a.seq > b.seq;
    }
};

struct SellComparator {
    bool operator()(const Order& a, const Order& b) const {
        if (a.price != b.price)
            return a.price > b.price;
        return a.seq > b.seq;
    }
};

#endif
