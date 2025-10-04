/* We simply call the root header file "App.h", giving you uWS::App and uWS::SSLApp */
#include "App.h"
#include <time.h>
#include <iostream>

/* This is a simple WebSocket echo server example.
 * You may compile it with "WITH_OPENSSL=1 make" or with "make" */

uWS::SSLApp *globalApp;

int main() {
    /* ws->getUserData returns one of these */
    struct PerSocketData {
        /* Fill with user data */
    };

    uWS::SocketContextOptions ctx;
    ctx.key_file_name = "misc/key.pem";
    ctx.cert_file_name = "misc/cert.pem";
    ctx.passphrase = "1234";
    ctx.dh_params_file_name = "misc/dhparams.pem";
    uWS::SSLApp::WebSocketBehavior<PerSocketData> sb;
	sb.compression = uWS::SHARED_COMPRESSOR;
    sb.compression = uWS::SHARED_COMPRESSOR;
    sb.maxPayloadLength = 16 * 1024 * 1024;
    sb.idleTimeout = 16;
    sb.maxBackpressure = 1 * 1024 * 1024;
    sb.closeOnBackpressureLimit = false;
    sb.resetIdleTimeoutOnSend = false;
    sb.sendPingsAutomatically = true;
        /* Handlers */
    sb.upgrade = nullptr;
    sb.open = [](auto* ws) {
        /* Open event here, you may access ws->getUserData() which points to a PerSocketData struct */
        ws->subscribe("broadcast");
        };
    sb.message = [](auto*/*ws*/, std::string_view /*message*/, uWS::OpCode /*opCode*/) {

        };
    sb.drain = [](auto*/*ws*/) {
        /* Check ws->getBufferedAmount() here */
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
    uWS::SSLApp app = uWS::SSLApp(ctx);
    app.ws<PerSocketData>("/*", std::move(sb));
    app.listen(9001, [](auto* listen_socket) {
        if (listen_socket) {
            std::cout << "Listening on port " << 9001 << std::endl;
        }
    });

    struct us_loop_t *loop = (struct us_loop_t *) uWS::Loop::get();
    struct us_timer_t *delayTimer = us_create_timer(loop, 0, 0);

    // broadcast the unix time as millis every 8 millis
    us_timer_set(delayTimer, [](struct us_timer_t */*t*/) {

        struct timespec ts;
        timespec_get(&ts, TIME_UTC);

        int64_t millis = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;

        //std::cout << "Broadcasting timestamp: " << millis << std::endl;

        globalApp->publish("broadcast", std::string_view((char *) &millis, sizeof(millis)), uWS::OpCode::BINARY, false);

    }, 8, 8);

    globalApp = &app;

    app.run();
}
