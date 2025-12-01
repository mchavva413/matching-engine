#include <chrono>
#include <random>
#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include "matching_engine.hpp"

// Simple helper to compute percentiles (assumes sorted)
uint64_t percentile(const std::vector<uint64_t>& data, double p) {
    if (data.empty()) return 0;
    double idx = (p/100.0) * (data.size() - 1);
    size_t lo = (size_t)std::floor(idx);
    size_t hi = (size_t)std::ceil(idx);
    if (lo == hi) return data[lo];
    double frac = idx - lo;
    return (uint64_t)((1.0 - frac)*data[lo] + frac*data[hi]);
}

int main(int argc, char** argv) {
    // Config
    uint64_t N = 200000;           // number of orders to send
    double buy_ratio = 0.5;        // fraction that are buys
    uint64_t warmup = 20000;       // warmup orders not measured
    int64_t mid_price = 100000;    // base price in ticks
    int price_spread = 100;        // +/- spread around mid

    if (argc > 1) N = std::stoull(argv[1]);
    if (argc > 2) buy_ratio = std::stod(argv[2]);

    std::cout << "Benchmark params: N=" << N << " buy_ratio=" << buy_ratio
              << " warmup=" << warmup << " mid_price=" << mid_price << "\n";

    MatchingEngine engine;

    // RNG
    std::mt19937_64 rng(123456789);
    std::uniform_real_distribution<double> u01(0.0, 1.0);
    std::uniform_int_distribution<int> spread_dist(-price_spread, price_spread);
    std::uniform_int_distribution<int> qty_dist(1, 100);

    std::vector<uint64_t> latencies;
    latencies.reserve(N);

    uint64_t order_id = 1000000;

    // Warmup
    for (uint64_t i = 0; i < warmup; ++i) {
        NewOrderMsg m;
        m.order_id = order_id++;
        m.side = (u01(rng) < buy_ratio) ? Side::BUY : Side::SELL;
        m.price = mid_price + spread_dist(rng);
        m.qty = qty_dist(rng);
        engine.process_limit(m);
    }

    // Timed run
    auto run_start = std::chrono::high_resolution_clock::now();
    for (uint64_t i = 0; i < N; ++i) {
        NewOrderMsg m;
        m.order_id = order_id++;
        m.side = (u01(rng) < buy_ratio) ? Side::BUY : Side::SELL;
        // occasionally generate a market-like aggressive order
        if ((i % 1000) == 0) m.price = (m.side==Side::BUY) ? mid_price + price_spread*10 : mid_price - price_spread*10;
        else m.price = mid_price + spread_dist(rng);

        m.qty = qty_dist(rng);

        auto t0 = std::chrono::high_resolution_clock::now();
        engine.process_limit(m);
        auto t1 = std::chrono::high_resolution_clock::now();
        uint64_t ns = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
        latencies.push_back(ns);
    }
    auto run_end = std::chrono::high_resolution_clock::now();

    // Basic throughput
    double elapsed_s = std::chrono::duration_cast<std::chrono::duration<double>>(run_end - run_start).count();
    double ops_per_sec = (double)N / elapsed_s;

    // Percentiles
    std::sort(latencies.begin(), latencies.end());
    uint64_t med = percentile(latencies, 50.0);
    uint64_t p90 = percentile(latencies, 90.0);
    uint64_t p99 = percentile(latencies, 99.0);
    uint64_t p999 = percentile(latencies, 99.9);

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\nBenchmark results:\n";
    std::cout << "  Total orders: " << N << "\n";
    std::cout << "  Time (s): " << elapsed_s << "\n";
    std::cout << "  Ops/sec: " << ops_per_sec << "\n";
    std::cout << "  Latency ns (median): " << med << "\n";
    std::cout << "  Latency ns (p90):     " << p90 << "\n";
    std::cout << "  Latency ns (p99):     " << p99 << "\n";
    std::cout << "  Latency ns (p99.9):   " << p999 << "\n";

    return 0;
}
