# Project Documentation

## Introduction

...

## Research on Limit Book Orders

...

## Implementation

### Order class

- Contains public members such as 'price', 'shares', sequence (which is used to determine the earliest order) and a global 'counter' (used to define the sequence for each newly created order)
- The global counter starts at 0 everytime the project is run
- In the header file are also included the comparators for the buy and sell queues. The only difference is that buy order are sorted by highest price (then earliest order), while sell order are sorted by lowest price (then earliest order)

### LimitOrderBook class

- Includes the buy and sell priority queues, as well as public member functions 'addBuy()', 'addSell()', 'match()', and 'printBook()'
- The add functions dynamically create new Order objects in the sense that the queue grows at runtime, memory is allocated at the heap, and objects disappear once popped
- The 'match()' function is run everytime a new order is added. It automatically finds matches and deletes the matching orders from the queues, while displaying the match details


### Real-time Command Loop

- The program presents a menu for as long as the user decides to run the project
- Once entered command 'b', the code will stop running
- The new orders are inserted through the menu

## Conclusions

- Ricardo: 
- Isaac: