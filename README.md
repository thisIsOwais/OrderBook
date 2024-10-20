# Order Book System in C++

## Overview
This project is an implementation of an **Order Book** system in C++ which simulates the trading mechanism typically seen in stock exchanges. The order book is responsible for matching buy and sell orders based on various conditions such as price and quantity.

### Key Features:
- **Order Matching:** Automatically matches buy and sell orders.
- **Order Types:** Supports multiple order types including Market, Limit, Stop, Good-Till-Canceled, and Fill-Or-Kill orders.
- **Order Cancellation:** Allows orders to be canceled by their ID.
- **Execution Simulation:** Simulates execution of orders with print statements to display matches.

## Order Types
The system supports the following types of orders:
1. **Market Order:** An order to buy or sell immediately at the best available current price.
2. **Limit Order:** An order to buy or sell at a specified price or better.
3. **Good-Till-Canceled (GTC):** An order that remains active until it is executed or manually canceled.
4. **Fill-Or-Kill (FOK):** An order that must be executed immediately in its entirety or canceled.

## Code Structure

### 1. `OrderBook` Class
The `OrderBook` class manages the list of orders and handles the logic for adding, canceling, and matching orders.

- **OrderType & Side:** Enumerations that define the type and side (Buy/Sell) of each order.
- **Order Class:** A nested class that defines the attributes of individual orders such as `id`, `type`, `side`, `price`, and `quantity`.
- **addOrder:** Adds an order to the order book.
- **cancelOrder:** Cancels an order by its unique ID.
- **matchOrders:** Matches orders based on their types and sides, prioritizing `Market` and `GTC` orders first, followed by `Limit` and `FOK` orders.
- **printOrders:** Displays all current orders in the order book.

### 2. Order Matching Logic
The `matchOrders` method processes orders in the following steps:
- **Market Orders:** First, it processes market orders by finding a match and executing the order.
- **Good-Till-Canceled Orders:** These orders are matched similarly to market orders, but they persist until they are matched or manually canceled.
- **Fill-Or-Kill Orders:** These are either fully matched or canceled if no full match is found.
- **Limit Orders:** The remaining unmatched limit orders are processed last.

### 3. Main Program
The `main` function demonstrates how the `OrderBook` works by:
- Creating several sample orders of different types.
- Adding them to the order book.
- Displaying the order book before and after matching the orders.

## Example Code Usage

The following sample orders are created and added to the order book:
```cpp
OrderBook::Order order1(1, OrderBook::OrderType::Market, OrderBook::Side::Buy, 0, 10);
OrderBook::Order order2(2, OrderBook::OrderType::Limit, OrderBook::Side::Sell, 101.0, 20);
OrderBook::Order order3(3, OrderBook::OrderType::Limit, OrderBook::Side::Sell, 99.0, 5);
OrderBook::Order order4(4, OrderBook::OrderType::Market, OrderBook::Side::Sell, 0, 15);
OrderBook::Order order5(5, OrderBook::OrderType::GoodTillCanceled, OrderBook::Side::Buy, 102.0, 10);
OrderBook::Order order6(6, OrderBook::OrderType::FillOrKill_Limit, OrderBook::Side::Sell, 100.0, 8);
OrderBook::Order order7(7, OrderBook::OrderType::FillOrKill_Limit, OrderBook::Side::Buy, 99.0, 12);
OrderBook::Order order8(8, OrderBook::OrderType::FillOrKill_Limit, OrderBook::Side::Buy, 101.0, 8);
