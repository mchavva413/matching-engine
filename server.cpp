#include <iostream>
#include "httplib.h"
#include "json.hpp"
#include "matching_engine.hpp"

using json = nlohmann::json;

int main() {
    MatchingEngine engine;
    httplib::Server svr;

    svr.Get("/book", [&](const httplib::Request&, httplib::Response& res) {
        auto snap = engine.snapshot_book();
        json j;
        j["bids"] = json::array();
        j["asks"] = json::array();

        for (auto &b : snap.bids) j["bids"].push_back({{"price", b.price}, {"qty", b.qty}});
        for (auto &a : snap.asks) j["asks"].push_back({{"price", a.price}, {"qty", a.qty}});

        res.set_content(j.dump(), "application/json");
    });

    svr.Get("/trades", [&](const httplib::Request&, httplib::Response& res) {
        auto trades = engine.get_exec_log().drain_trades();
        json j = json::array();
        for (auto &t : trades)
            j.push_back({{"buy_id", t.buy_id}, {"sell_id", t.sell_id}, {"price", t.price}, {"qty", t.qty}});
        res.set_content(j.dump(), "application/json");
    });

    svr.Post("/limit", [&](const httplib::Request& req, httplib::Response& res) {
        if (!req.has_param("id") || !req.has_param("side")
        || !req.has_param("price") || !req.has_param("qty")) {
            res.set_content("{\"error\":\"missing params\"}", "application/json");
            return;
        }

        uint64_t id = std::stoull(req.get_param_value("id"));
        std::string side = req.get_param_value("side");
        double price = std::stod(req.get_param_value("price"));
        uint64_t qty = std::stoull(req.get_param_value("qty"));

        std::cout << "LIMIT " << side << " id=" << id << " price=" << price << " qty=" << qty << "\n";

        if (side == "BUY") engine.add_limit_buy(id, price, qty);
        else engine.add_limit_sell(id, price, qty);

        res.set_content("{\"status\":\"ok\"}", "application/json");
    });

    std::cout << "SERVER running on http://localhost:8080\n";
    svr.listen("0.0.0.0", 8080);
}

