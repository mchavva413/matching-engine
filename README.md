# ⚡ Ultra–Low-Latency C++ Matching Engine

High-performance C++ matching engine implementing **bid/ask order books**, **FIFO queues**, **partial fills**, **cancel/modify operations**, and **real-time execution logging** — built to mirror exchange behavior used in electronic trading and market-making systems.

---

## 📘 Project Overview  

This project simulates how real exchanges match buy/sell orders with microsecond-level response times.

### It includes:
- **Limit & Market Order Processing**
- **Price–Time Priority (FIFO)**
- **Partial Fill Logic**
- **Order Cancel & Modify**
- **Bid/Ask Order Book**
- **Execution Logging**
- **Latency Tracking (nanoseconds)**
- **Benchmarking Tools**

---

## 🔁 Workflow  

```
Incoming Order → Match Engine →
   ├─ Full Match → Generate Fills → Log
   ├─ Partial Match → Reduce Quantity → Insert into Book
   └─ No Match → Insert into Book
```

---

## 🧰 Tech Stack  

| Component | Description |
|----------|-------------|
| **Language** | C++20 |
| **Build System** | CMake |
| **Data Structures** | Intrusive FIFO queues, price-level maps |
| **Logging** | CSV execution logs |
| **Latency** | Nanosecond timing |
| **Benchmarking** | Synthetic order flow |
| **OS** | macOS / Linux |

---

## 📂 Project Structure  

```
matching-engine/
├── bench/              # Benchmarks
├── include/            # Headers
├── src/                # Engine implementation
├── server.cpp          # Optional HTTP server
├── messages.csv        # Sample input
├── modify_test.csv     # Modify order testing
├── CMakeLists.txt
└── .gitignore
```

---

## 🛠️ Setup Instructions  

### 1️⃣ Clone the Repository  
```bash
git clone https://github.com/mchavva413/matching-engine.git
cd matching-engine
```

### 2️⃣ Build  
```bash
mkdir build && cd build
cmake ..
make -j
```

### 3️⃣ Run Engine  
```bash
./engine
```

### 4️⃣ Run Benchmarks  
```bash
./bench
```

---

## 📊 Benchmark Example  

| Metric | Value |
|--------|--------|
| **Throughput** | ~1,200,000 orders/sec |
| **Avg Latency** | ~120ns |
| **p99 Latency** | ~410ns |
| **Max Depth** | 50 levels |

---

## 🚀 Key Features  

### ✔ Price–Time Priority  
FIFO ordering inside each price level using an intrusive linked list.

### ✔ O(1) Cancel/Modify  
Orders stored in a hash table for instant lookup.

### ✔ Realistic Matching  
- Multiple fill iterations  
- Partial fills  
- Price crossing  

### ✔ Low-Latency Optimizations  
- `-O3 -march=native`  
- Cache-friendly structures  
- Minimal allocations  
- Single-threaded deterministic core  

---

## 🔮 Future Improvements  
- Replace `std::map` with skiplist / flat_hash_map  
- Add order pool to eliminate heap allocations  
- Add IOC, FOK order types  
- Build market data replay system  
- Add WebSocket dashboard  

---

## 👤 Author  

**Akshit Chavva**  
Low-Latency Systems • C++ • Algorithmic Trading • Market Microstructure  
