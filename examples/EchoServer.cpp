/* We simply call the root header file "App.h", giving you uWS::App and uWS::SSLApp */
#include "App.h"

/* This is a simple WebSocket echo server example.
 * You may compile it with "WITH_OPENSSL=1 make" or with "make" */

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
    uWS::App::WebSocketBehavior<PerSocketData> sb;

    sb.compression = uWS::CompressOptions(uWS::DEDICATED_COMPRESSOR | uWS::DEDICATED_DECOMPRESSOR);
    sb.maxPayloadLength = 100 * 1024 * 1024;
    sb.idleTimeout = 16;
    sb.maxBackpressure = 100 * 1024 * 1024;
    sb.closeOnBackpressureLimit = false;
    sb.resetIdleTimeoutOnSend = false;
    sb.sendPingsAutomatically = true;
        /* Handlers */
    sb.upgrade = nullptr;
    sb.open = [](auto*/*ws*/) {
        /* Open event here, you may access ws->getUserData() which points to a PerSocketData struct */

        };
    sb.message = [](auto* ws, std::string_view message, uWS::OpCode opCode) {
        /* This is the opposite of what you probably want; compress if message is LARGER than 16 kb
         * the reason we do the opposite here; compress if SMALLER than 16 kb is to allow for
         * benchmarking of large message sending without compression */

         /* Never mind, it changed back to never compressing for now */
        ws->send(message, opCode, false);
        };
    sb.dropped = [](auto*/*ws*/, std::string_view /*message*/, uWS::OpCode /*opCode*/) {
        /* A message was dropped due to set maxBackpressure and closeOnBackpressureLimit limit */
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
    uWS::App(ctx).ws<PerSocketData>("/*", std::move(sb)).listen(9001, [](auto *listen_socket) {
        if (listen_socket) {
            std::cout << "Listening on port " << 9001 << std::endl;
        }
    }).run();
}
