#ifndef WEBSOCKET_H
#define WEBSOCKET_H
#define ASIO_STANDALONE

#include <functional>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

void start_server(int port, std::function<void(websocketpp::connection_hdl)> http_handler,
                  std::function<void(websocketpp::connection_hdl, websocketpp::server<websocketpp::config::asio>::message_ptr)> message_handler);

#endif // WEBSOCKET_H