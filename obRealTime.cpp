#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <curl/curl.h>

// OrderBook class definition with real-time data fetching
class OrderBook {
public:
    enum class OrderType { Market, Limit, Stop, GoodTillCanceled, FillOrKill_Limit };
    enum class Side { Buy, Sell };

    class Order {
    public:
        Order(int id, OrderType type, Side side, double price, int quantity)
            : id(id), type(type), side(side), price(price), quantity(quantity) {}

        int getId() const { return id; }
        OrderType getType() const { return type; }
        Side getSide() const { return side; }
        double getPrice() const { return price; }
        int getQuantity() const { return quantity; }

        void setQuantity(int new_quantity) { quantity = new_quantity; }
        void setPrice(double new_price) { price = new_price; } // Update price with real-time data

    private:
        int id;
        OrderType type;
        Side side;
        double price;
        int quantity;
    };

    // Method to add an order to the order book
    void addOrder(const Order& order) {
        orders.push_back(order);
    }

    // Method to match orders in the order book
    void matchOrders() {
        // Handle Market orders first
        for (auto it = orders.begin(); it != orders.end();) {
            if (it->getType() == OrderType::Market) {
                auto matchIt = findMatch(it);
                if (matchIt != orders.end()) {
                    executeOrder(it, matchIt);
                    it = orders.erase(it);
                } else {
                    ++it;
                }
            } else {
                ++it;
            }
        }
    }

    // Print all orders in the order book
    void printOrders() const {
        for (const auto& order : orders) {
            printOrder(order);
        }
    }

private:
    std::vector<Order> orders; // Vector to store orders

    // Helper method to find a match for an order
    std::vector<Order>::iterator findMatch(std::vector<Order>::iterator orderIt) {
        for (auto it = orders.begin(); it != orders.end(); ++it) {
            if (it->getSide() != orderIt->getSide() &&
                ((orderIt->getSide() == Side::Buy && it->getPrice() <= orderIt->getPrice()) ||
                 (orderIt->getSide() == Side::Sell && it->getPrice() >= orderIt->getPrice()))) {
                return it;
            }
        }
        return orders.end();
    }

    // Helper method to execute an order
    void executeOrder(std::vector<Order>::iterator& orderIt, std::vector<Order>::iterator& matchIt) {
        double fillPrice = matchIt->getPrice();
        std::cout << "Matched Order ID: " << orderIt->getId() << " with Order ID: " << matchIt->getId() << " at Price: " << fillPrice << std::endl;
        matchIt->setQuantity(matchIt->getQuantity() - orderIt->getQuantity());
        if (matchIt->getQuantity() == 0) {
            orders.erase(matchIt);
        }
    }

    // Helper method to print an individual order
    void printOrder(const Order& order) const {
        std::cout << "Order ID: " << order.getId() 
                  << ", Type: " << static_cast<int>(order.getType()) 
                  << ", Side: " << (order.getSide() == Side::Buy ? "Buy" : "Sell")
                  << ", Price: " << order.getPrice() 
                  << ", Quantity: " << order.getQuantity() << std::endl;
    }
};

// Class to fetch real-time data using cURL
class RealTimeDataFetcher {
public:
    RealTimeDataFetcher(const std::string& api_key) : api_key(api_key) {}

    // Fetch real-time price for a given stock symbol
    double fetchRealTimePrice(const std::string& symbol) {
        CURL* curl;
        CURLcode res;
        std::string readBuffer;

        curl = curl_easy_init();
        if (curl) {
            std::string url = "https://www.alphavantage.co/query?function=GLOBAL_QUOTE&symbol=" + symbol + "&apikey=" + api_key;

            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

            // Perform the request
            res = curl_easy_perform(curl);

            // Check for errors
            if (res != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            }

            // Clean up
            curl_easy_cleanup(curl);
        }

        // Parse the real-time price from the fetched data (simplified for demo purposes)
        double price = extractPriceFromJson(readBuffer);
        return price;
    }

private:
    std::string api_key;

    // Callback to capture the HTTP response
    static size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
        s->append((char*)contents, size * nmemb);
        return size * nmemb;
    }

    // Simple JSON parsing to extract the price from the response (you can use a JSON library for real use cases)
    double extractPriceFromJson(const std::string& json) {
        // This is a simple string search for demonstration. You should use a JSON library like nlohmann/json for more robust parsing.
        std::string key = "\"05. price\": \"";
        size_t start = json.find(key) + key.length();
        size_t end = json.find("\"", start);
        std::string priceStr = json.substr(start, end - start);
        return std::stod(priceStr);
    }
};

// Main program
int main() {
    std::string api_key = "your_api_key_here"; // Replace with your actual API key
    RealTimeDataFetcher dataFetcher(api_key);

    OrderBook orderBook;

    // Create sample orders
    OrderBook::Order order1(1, OrderBook::OrderType::Market, OrderBook::Side::Buy, 0, 10);
    OrderBook::Order order2(2, OrderBook::OrderType::Limit, OrderBook::Side::Sell, 100.0, 20);

    // Add orders to the order book
    orderBook.addOrder(order1);
    orderBook.addOrder(order2);

    // Fetch real-time price for a stock (e.g., AAPL)
    double realTimePrice = dataFetcher.fetchRealTimePrice("AAPL");

    // Update the order book with the real-time price
    order1.setPrice(realTimePrice);  // Set price for the Market order
    orderBook.addOrder(order1);

    // Print the updated order book
    std::cout << "Order Book before matching:" << std::endl;
    orderBook.printOrders();

    // Match the orders
    orderBook.matchOrders();

    // Print the order book after matching
    std::cout << "Order Book after matching:" << std::endl;
    orderBook.printOrders();

    return 0;
}
