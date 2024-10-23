#include "websocket.h"
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <nlohmann/json.hpp>
#include <map>
#include <chrono>
#include <iostream>

const int TIME_WINDOW = 60; // Example value
const int RATE_LIMIT = 100; // Example value

struct ClientInfo {
    std::chrono::steady_clock::time_point start_time;
    int message_count;
};

std::map<websocketpp::connection_hdl, ClientInfo, std::owner_less<websocketpp::connection_hdl>> clients;

void on_message(websocketpp::server<websocketpp::config::asio>& ws_server, websocketpp::connection_hdl hdl, websocketpp::server<websocketpp::config::asio>::message_ptr msg) {
    auto now = std::chrono::steady_clock::now();
    auto& client_info = clients[hdl];

    if (client_info.message_count == 0) {
        client_info.start_time = now;
    }

    auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(now - client_info.start_time).count();

    if (elapsed_time > TIME_WINDOW) {
        client_info.message_count = 0;
        client_info.start_time = now;
    }

    if (client_info.message_count >= RATE_LIMIT) {
        std::cerr << "Rate limit exceeded for client" << std::endl;
        ws_server.close(hdl, websocketpp::close::status::policy_violation, "Rate limit exceeded");
        return;
    }

    client_info.message_count++;

    nlohmann::json received_data;
    try {
        received_data = nlohmann::json::parse(msg->get_payload());
    } catch (nlohmann::json::parse_error& e) {
        std::cout << "Received message: " << msg->get_payload() << std::endl;
        std::cerr << "Failed to parse message as JSON: " << e.what() << std::endl;
        ws_server.close(hdl, websocketpp::close::status::invalid_payload, "Invalid JSON");
        return;
    }

    std::cout << "Received JSON data: " << received_data.dump(4) << std::endl;

    ws_server.send(hdl, msg->get_payload(), msg->get_opcode());
}

void on_open(websocketpp::server<websocketpp::config::asio>& ws_server, websocketpp::connection_hdl hdl) {
    std::cout << "Connection opened" << std::endl;
}

void on_close(websocketpp::server<websocketpp::config::asio>& ws_server, websocketpp::connection_hdl hdl) {
    std::cout << "Connection closed" << std::endl;
    clients.erase(hdl);
}

void on_fail(websocketpp::server<websocketpp::config::asio>& ws_server, websocketpp::connection_hdl hdl) {
    std::cerr << "Connection failed" << std::endl;
    auto con = ws_server.get_con_from_hdl(hdl);
    std::cerr << "Error: " << con->get_ec().message() << std::endl;
}

void start_server(int port) {
    websocketpp::server<websocketpp::config::asio> server;

    server.set_message_handler([&server](websocketpp::connection_hdl hdl, websocketpp::server<websocketpp::config::asio>::message_ptr msg) {
        on_message(server, hdl, msg);
    });

    server.set_open_handler([&server](websocketpp::connection_hdl hdl) {
        on_open(server, hdl);
    });

    server.set_close_handler([&server](websocketpp::connection_hdl hdl) {
        on_close(server, hdl);
    });

    server.set_fail_handler([&server](websocketpp::connection_hdl hdl) {
        on_fail(server, hdl);
    });

    server.init_asio();
    server.listen(port);
    server.start_accept();

    std::cout << "Server started on port " << port << std::endl;

    server.run();
}