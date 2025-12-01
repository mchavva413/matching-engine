#pragma once
#include "order.hpp"
#include <vector>

class OrderPool {
public:
    explicit OrderPool(size_t cap = 1 << 20);
    int alloc(const Order& o);    // returns index allocated, -1 on failure
    void free(int idx);
    Order* get(int idx);
    size_t capacity() const;

private:
    std::vector<Order> pool_;
    std::vector<int> free_idx_;
};

