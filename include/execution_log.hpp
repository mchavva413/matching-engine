#pragma once
#include <cstdint>
#include <vector>
#include <string>

struct TradeEvent {
    uint64_t ts;      // timestamp
    uint64_t buy_id;  // buy order id
    uint64_t sell_id; // sell order id
    int64_t price;    // trade price
    uint32_t qty;     // traded quantity
};

class ExecutionLog {
public:
    ExecutionLog(size_t capacity = 1<<20);

    // Add a trade (timestamp + meta)
    void add_trade(uint64_t ts,
                   uint64_t buy_id,
                   uint64_t sell_id,
                   int64_t  price,
                   uint32_t qty);

    // Return and clear the current buffer (used by server to fetch new trades)
    std::vector<TradeEvent> drain_trades();

    // Print to stdout (keeps existing contents)
    void dump_to_stdout() const;

    // Write contents to CSV file (keeps existing contents)
    void dump_to_file(const std::string& path) const;

private:
    std::vector<TradeEvent> buf_;
    size_t head_;
    size_t size_;
    size_t capacity_;
};
