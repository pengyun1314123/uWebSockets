#include "App.h"

struct us_listen_socket_t *global_listen_socket;

int main() {

    /* ws->getUserData returns one of these */
    struct PerSocketData {
        /* Fill with user data */
        std::vector<std::string> topics;
        int nr = 0;
    };
    uWS::SocketContextOptions ctx;
    ctx.key_file_name = "misc/key.pem";
    ctx.cert_file_name = "misc/cert.pem";
    ctx.passphrase = "1234";
    ctx.dh_params_file_name = "misc/dhparams.pem";

    uWS::SSLApp* app = new uWS::SSLApp(ctx);

    uWS::SSLApp::WebSocketBehavior<PerSocketData> sb;
    sb.compression = uWS::DISABLED;
    sb.maxPayloadLength = 16 * 1024 * 1024;
    sb.idleTimeout = 60;
    sb.maxBackpressure = 16 * 1024 * 1024;
    sb.closeOnBackpressureLimit = false;
    sb.resetIdleTimeoutOnSend = true;
    sb.sendPingsAutomatically = false;
        /* Handlers */
    sb.upgrade = nullptr;
    sb.open = [](auto* ws) {
        /* Open event here, you may access ws->getUserData() which points to a PerSocketData struct */

        PerSocketData* perSocketData = (PerSocketData*)ws->getUserData();

        for (int i = 0; i < 32; i++) {
            std::string topic = std::to_string((uintptr_t)ws) + "-" + std::to_string(i);
            perSocketData->topics.push_back(topic);
            ws->subscribe(topic);
        }
        };
    sb.message = [&app](auto* ws, std::string_view message, uWS::OpCode opCode) {
        PerSocketData* perSocketData = (PerSocketData*)ws->getUserData();

        app->publish(perSocketData->topics[(size_t)(++perSocketData->nr % 32)], message, opCode);
        ws->publish(perSocketData->topics[(size_t)(++perSocketData->nr % 32)], message, opCode);
        };
    sb.drain = [](auto*/*ws*/) {
        /* Check ws->getBufferedAmount() here */
        //std::cout << "drain" << std::endl;
        };
    sb.ping = [](auto*/*ws*/, std::string_view) {
        /* Not implemented yet */
        };
    sb.pong = [](auto*/*ws*/, std::string_view) {
        /* Not implemented yet */
        };
    sb.close = [](auto*/*ws*/, int /*code*/, std::string_view /*message*/) {
        /* You may access ws->getUserData() here */
        };
    /* Keep in mind that uWS::SSLApp({options}) is the same as uWS::App() when compiled without SSL support.
     * You may swap to using uWS:App() if you don't need SSL */
    
    app->ws<PerSocketData>("/*", std::move(sb));
    app->listen(9001, [](auto* listen_s) {
        if (listen_s) {
            std::cout << "Listening on port " << 9001 << std::endl;
            //listen_socket = listen_s;
        }
    });
    
    app->run();

    delete app;

    uWS::Loop::get()->free();
}
