#ifndef WEBSOCKET_H
#define WEBSOCKET_H
#define ASIO_STANDALONE

#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <nlohmann/json.hpp>
#include <chrono>
#include <iostream>

using websocketpp::connection_hdl;

void on_message(websocketpp::server<websocketpp::config::asio>& server, connection_hdl hdl, websocketpp::server<websocketpp::config::asio>::message_ptr msg);
void start_server(int port);

#endif // WEBSOCKET_H