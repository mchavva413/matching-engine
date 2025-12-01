#pragma once
#include <cstdint>

enum class Side : uint8_t { BUY=0, SELL=1 };

struct Order {
    uint64_t order_id = 0;
    Side side = Side::BUY;
    int64_t price = 0;
    uint32_t qty = 0;
    uint64_t ts = 0;
    int32_t prev = -1;
    int32_t next = -1;
    bool in_book = false;
};
