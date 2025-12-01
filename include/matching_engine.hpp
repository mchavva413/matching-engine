#pragma once
#include <vector>
#include <deque>
#include <cstdint>
#include <limits>

struct Order {
    uint64_t id;
    double price;
    uint64_t qty;
};

struct TradeEvent {
    uint64_t buy_id;
    uint64_t sell_id;
    double price;
    uint64_t qty;
};

class ExecutionLog {
public:
    void add_trade(uint64_t buy_id, uint64_t sell_id, double price, uint64_t qty);
    std::vector<TradeEvent> drain_trades();

private:
    std::vector<TradeEvent> buf_;
};

class MatchingEngine {
public:
    MatchingEngine();

    void add_limit_buy(uint64_t id, double price, uint64_t qty);
    void add_limit_sell(uint64_t id, double price, uint64_t qty);

    struct BookSnapshotSide {
        double price;
        uint64_t qty;
    };

    struct BookSnapshot {
        std::vector<BookSnapshotSide> bids;
        std::vector<BookSnapshotSide> asks;
    };

    BookSnapshot snapshot_book();
    ExecutionLog& get_exec_log();

private:
    std::deque<Order> bids_;
    std::deque<Order> asks_;
    ExecutionLog exec_log_;
};

