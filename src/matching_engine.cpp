#include "matching_engine.hpp"
#include <algorithm>

MatchingEngine::MatchingEngine() {}

ExecutionLog& MatchingEngine::get_exec_log() {
    return exec_log_;
}

void ExecutionLog::add_trade(uint64_t buy_id, uint64_t sell_id, double price, uint64_t qty) {
    buf_.push_back({buy_id, sell_id, price, qty});
}

std::vector<TradeEvent> ExecutionLog::drain_trades() {
    auto out = buf_;
    buf_.clear();
    return out;
}

// ------------------- MATCHING LOGIC -------------------

void MatchingEngine::add_limit_buy(uint64_t id, double price, uint64_t qty) {
    while (qty > 0 && !asks_.empty() && asks_.front().price <= price) {
        auto &top = asks_.front();
        uint64_t trade_qty = std::min(qty, top.qty);

        exec_log_.add_trade(id, top.id, top.price, trade_qty);

        qty -= trade_qty;
        top.qty -= trade_qty;
        if (top.qty == 0) asks_.pop_front();
    }
    if (qty > 0) bids_.push_back({id, price, qty});
}

void MatchingEngine::add_limit_sell(uint64_t id, double price, uint64_t qty) {
    while (qty > 0 && !bids_.empty() && bids_.front().price >= price) {
        auto &top = bids_.front();
        uint64_t trade_qty = std::min(qty, top.qty);

        exec_log_.add_trade(top.id, id, top.price, trade_qty);

        qty -= trade_qty;
        top.qty -= trade_qty;
        if (top.qty == 0) bids_.pop_front();
    }
    if (qty > 0) asks_.push_back({id, price, qty});
}

// ------------------- BOOK SNAPSHOT -------------------

MatchingEngine::BookSnapshot MatchingEngine::snapshot_book() {
    BookSnapshot snap;

    for (auto &b : bids_)
        snap.bids.push_back({b.price, b.qty});
    for (auto &a : asks_)
        snap.asks.push_back({a.price, a.qty});

    return snap;
}

