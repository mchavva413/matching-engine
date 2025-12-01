📈 Ultra–Low-Latency C++ Order Matching Engine

A production-style, single-threaded matching engine written in modern C++ and designed to simulate real exchange behavior. Built with low-latency techniques used in professional market-making and high-frequency trading systems.

🚀 Features
🏛 Core Exchange Mechanics

Limit Orders

Market Orders

Order Cancelation

Order Modification

Bid/Ask Order Book

Price–Time Priority (FIFO)

Partial Fill Support

⚡ Low-Latency Engineering

Single-threaded, lock-free architecture

Intrusive linked lists per price level

Cache-aware data layout

Custom memory pool (optional)

Nanosecond-resolution latency measurements

Lightweight execution logging

🔬 Benchmarking

Includes a benchmark driver that measures:

Throughput (orders/sec)

Latency distribution (avg / p50 / p90 / p99)

Impact of order type mixes

CPU performance under synthetic order flow

📂 Project Structure
matching-engine/
│
├── include/            # Headers (engine, order book, pools)
├── src/                # Matching engine implementation
├── bench/              # Benchmark driver
├── server.cpp          # HTTP server to submit orders (optional)
├── messages.csv        # Sample test orders
├── modify_test.csv     # Modify/cancel tests
├── CMakeLists.txt      # Build configuration
└── .gitignore

🛠 Build Instructions (macOS / Linux)
Install dependencies

You only need CMake and a C++20 compiler.

Build:
mkdir build && cd build
cmake ..
make -j

▶️ Run Engine Demo
./engine

📊 Run Benchmark
./bench


Produces:

latencies.csv

throughput logs

fill logs
Use Python / Excel / Pandas to analyze (optional).

📦 Order Flow Diagram
Order → Pre-Validation → Match Attempt →  
  ├─ Fully Matched → Generate Fills → Log → Done  
  ├─ Partially Matched → Reduce Quantity → Add to Book  
  └─ Not Matched → Insert into Book (FIFO)

🧠 Design Overview
Data Structures

std::map for price levels

intrusive FIFO linked lists for orders

unordered_map for O(1) order-id lookup

vector for log output and latency storage

Why this design?

Deterministic ordering

Minimal allocations

High throughput

Easy to extend (order types, IOC, FOK, routing)

⚡ Performance Notes

This architecture supports:

~500k to 2M messages/sec on laptop hardware

Sub-microsecond average latency

Stable p99 latency due to single-threaded, predictable memory access patterns

(Real numbers depend on your CPU — include your benchmark results for interviews.)

🧭 Future Enhancements

These are excellent talking points during interviews:

Replace std::map with flat hash map or skiplist

Add object pool allocator for orders

Implement I/O batching / kernel bypass (e.g., DPDK or io_uring)

Extend order types (IOC, FOK, Post-only)

Add replay engine for historical market data

💼 Why This Project Matters (Recruiter Pitch)

This project demonstrates:

Low-latency C++ engineering

Understanding of exchange microstructure

Memory-efficient data structures

Real-time system design

Algorithmic problem solving

Performance measurement

Clean code organization
