#include "App.h"
#include <thread>
#include <algorithm>

int main() {
    /* ws->getUserData returns one of these */
    struct PerSocketData {

    };
    
    /* Simple echo websocket server, using multiple threads */
    std::vector<std::thread *> threads(std::thread::hardware_concurrency());

    std::transform(threads.begin(), threads.end(), threads.begin(), [](std::thread */*t*/) {
        return new std::thread([]() {
            uWS::App::WebSocketBehavior<PerSocketData> sb;
            /* Settings */
            sb.compression = uWS::SHARED_COMPRESSOR;
            sb.maxPayloadLength = 16 * 1024;
            sb.idleTimeout = 10;
            sb.maxBackpressure = 1 * 1024 * 1024;
            /* Handlers */
            sb.upgrade = nullptr;
            sb.open = [](auto*/*ws*/) {

                };
            sb.message = [](auto* ws, std::string_view message, uWS::OpCode opCode) {
                ws->send(message, opCode);
                };
            sb.drain = [](auto*/*ws*/) {
                /* Check getBufferedAmount here */
                };
            sb.ping = [](auto*/*ws*/, std::string_view) {

                };
            sb.pong = [](auto*/*ws*/, std::string_view) {

                };
            sb.close = [](auto*/*ws*/, int /*code*/, std::string_view /*message*/) {

                };
            /* Very simple WebSocket echo server */
            uWS::App().ws<PerSocketData>("/*", std::move(sb)).listen(9001, [](auto *listen_socket) {
                if (listen_socket) {
                    std::cout << "Thread " << std::this_thread::get_id() << " listening on port " << 9001 << std::endl;
                } else {
                    std::cout << "Thread " << std::this_thread::get_id() << " failed to listen on port 9001" << std::endl;
                }
            }).run();

        });
    });

    std::for_each(threads.begin(), threads.end(), [](std::thread *t) {
        t->join();
    });
}
