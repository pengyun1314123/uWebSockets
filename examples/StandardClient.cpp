// This example is broken and doesn't do anything. It is a potential interface for a future potential client.
// There is no client support implemented in the library. This example is a placeholder.

#include "ClientApp.h"
#include <iostream>

int main() {
    uWS::WebSocketClientBehavior b;
    b.open = [](/*auto *ws*/) {
        std::cout << "Hello and welcome to client" << std::endl;
        };
    b.message = [](/*auto *ws, auto message*/) {
        std::cout << "got message" << std::endl;
        };
    b.close = [](/*auto *ws*/) {
        std::cout << "bye" << std::endl;
        };
    uWS::ClientApp app(std::move(b));

    //app.connect("ws://localhost:9000", "protocol");
    app.connect("ws://localhost:9000");

    app.run();
}
