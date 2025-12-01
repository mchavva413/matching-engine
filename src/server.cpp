#include "matching_engine.hpp"
#include <httplib.h>
#include <mutex>
#include <sstream>

static std::mutex g_mtx;
static MatchingEngine engine;

bool to_uint64(const std::string& s, uint64_t& out) {
    try { out = std::stoull(s); return true; }
    catch (...) { return false; }
}

bool to_int64(const std::string& s, int64_t& out) {
    try { out = std::stoll(s); return true; }
    catch (...) { return false; }
}

bool to_uint32(const std::string& s, uint32_t& out) {
    try { out = std::stoul(s); return true; }
    catch (...) { return false; }
}

int main() {
    httplib::Server svr;

    svr.Get("/book", [](const httplib::Request&, httplib::Response& res) {
        std::lock_guard<std::mutex> lock(g_mtx);
        std::string json = engine.get_book().serialize_json();
        res.set_content(json, "application/json");
    });

    svr.Get("/trades", [](const httplib::Request&, httplib::Response& res) {
        std::lock_guard<std::mutex> lock(g_mtx);
        auto trades = engine.get_exec_log().drain_trades();

        std::ostringstream out;
        out << "[";
        bool first = true;

        for (auto& t : trades) {
            if (!first) out << ",";
            first = false;
            out << "{"
                << "\"ts\":" << t.ts
                << ",\"buy\":" << t.buy_id
                << ",\"sell\":" << t.sell_id
                << ",\"price\":" << t.price
                << ",\"qty\":" << t.qty
                << "}";
        }
        out << "]";

        res.set_content(out.str(), "application/json");
    });

    svr.Post("/limit", [](const httplib::Request& req, httplib::Response& res) {
        std::lock_guard<std::mutex> lock(g_mtx);

        uint64_t id; int64_t price; uint32_t qty;
        std::string side_s;

        if (!req.has_param("id") || !req.has_param("side")
         || !req.has_param("price") || !req.has_param("qty")) {
            res.set_content("{\"error\":\"missing params\"}", "application/json");
            return;
        }

        to_uint64(req.get_param_value("id"), id);
        to_int64(req.get_param_value("price"), price);
        to_uint32(req.get_param_value("qty"), qty);
        side_s = req.get_param_value("side");

        Side side = (side_s == "B") ? Side::BUY : Side::SELL;

        engine.process_limit({id, side, price, qty});
        res.set_content("{\"status\":\"ok\"}", "application/json");
    });

    svr.Post("/cancel", [](const httplib::Request& req, httplib::Response& res) {
        std::lock_guard<std::mutex> lock(g_mtx);

        uint64_t id;
        to_uint64(req.get_param_value("id"), id);
        engine.process_cancel(id);

        res.set_content("{\"status\":\"ok\"}", "application/json");
    });

    svr.Post("/modify", [](const httplib::Request& req, httplib::Response& res) {
        std::lock_guard<std::mutex> lock(g_mtx);

        uint64_t id; int64_t price; uint32_t qty;
        to_uint64(req.get_param_value("id"), id);
        to_int64(req.get_param_value("price"), price);
        to_uint32(req.get_param_value("qty"), qty);

        engine.process_modify(id, price, qty);

        res.set_content("{\"status\":\"ok\"}", "application/json");
    });

    std::cout << "SERVER running on http://localhost:8080\n";
    svr.listen("0.0.0.0", 8080);
    return 0;
}
