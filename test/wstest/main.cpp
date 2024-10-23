#include <iostream>

void start_server(int port); // websocket.cpp.o --> importing function from object file

int main() {
    std::cout << "Hello, World!" << std::endl;

    int port = 9002;
    start_server(port);

    return 0;
}