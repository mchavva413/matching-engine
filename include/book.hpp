#pragma once
#include <map>
#include <vector>
#include <iostream>
#include "order_pool.hpp"

struct PriceLevel {
    int head = -1;
    int tail = -1;
    uint32_t total_qty = 0;
};

class OrderBook {
public:
    OrderBook(OrderPool* pool);

    void add_to_book(int order_idx);
    void remove_from_book(int order_idx, std::vector<int>& freed_resting);
    uint32_t match(int order_idx, std::vector<int>& freed_resting);

    // FIFO-friendly qty modify (adjust quantity but keep FIFO)
    void adjust_qty_in_place(int64_t price, int idx, uint32_t old_qty, uint32_t new_qty);

    // textual dump to stdout (keeps existing)
    void dump(std::ostream& os = std::cout) const;

    // -------------------------
    // New: safe read-only accessors for UI / server
    // -------------------------
    // Return const references to the internal maps (read-only)
    const std::map<int64_t, PriceLevel, std::greater<int64_t>>& get_bids() const { return bids_; }
    const std::map<int64_t, PriceLevel>& get_asks() const { return asks_; }

    // Return pointer to pool (non-owning) so server can walk orders in a safe read-only way.
    // NOTE: this returns non-const pointer to allow reading orders via pool->get(idx).
    OrderPool* get_pool() const { return pool_; }

    // JSON-serialiser (easy JSON for UI)
    // Produces: {"bids":[{"price":P,"qty":Q,"orders":N},...],"asks":[...]}
    std::string serialize_json() const;

private:
    OrderPool* pool_;

    // buy side - price descending
    std::map<int64_t, PriceLevel, std::greater<int64_t>> bids_;
    // sell side - price ascending
    std::map<int64_t, PriceLevel> asks_;

    template<typename MapType>
    void add_to_level(MapType& side, int64_t price, int idx);

    template<typename MapType>
    void remove_from_level(MapType& side, int64_t price, int idx);
};

