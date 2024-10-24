#include "websocket.h"

void start_server(
    int port,
    std::function<void(websocketpp::connection_hdl, websocketpp::server<websocketpp::config::asio>::message_ptr)> message_handler) {

    websocketpp::server<websocketpp::config::asio> wsserver;
    wsserver.set_message_handler(message_handler);

    wsserver.init_asio();
    wsserver.listen(port);
    wsserver.start_accept();
    wsserver.run();
}