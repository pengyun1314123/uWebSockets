/* We simply call the root header file "App.h", giving you uWS::App and uWS::SSLApp */
#include "define.h"
#include "App.h"

int main() {
    /* ws->getUserData returns one of these */


    uWS::SocketContextOptions ctx;
    ctx.key_file_name = "misc/key.pem";
    ctx.cert_file_name = "misc/cert.pem";
    ctx.passphrase = "1234";
    ctx.dh_params_file_name = "misc/dhparams.pem";
    MyBehavior sb;
    sb.compression = uWS::CompressOptions(uWS::DEDICATED_COMPRESSOR | uWS::DEDICATED_DECOMPRESSOR);
    sb.maxPayloadLength = 100 * 1024 * 1024;
    sb.idleTimeout = 16;// idleTimeout
    sb.maxBackpressure = 100 * 1024 * 1024;// maxBackpressure
    sb.closeOnBackpressureLimit = false; // closeOnBackpressureLimit
    sb.resetIdleTimeoutOnSend = false; // resetIdleTimeoutOnSend
    sb.sendPingsAutomatically = true; // sendPingsAutomatically
    sb.upgrade = nullptr; // upgrade
    sb.open =
        /* Handlers */
        [](auto*/*ws*/) {
        /* Open event here, you may access ws->getUserData() which points to a PerSocketData struct */
		std::cout << "A WebSocket connected!" << std::endl;
        }; // open
    sb.message =
        [](auto* ws, std::string_view message, uWS::OpCode opCode) {
        ws->send(message, opCode, false);
        };// message
    sb.dropped =
        [](auto*/*ws*/, std::string_view /*message*/, uWS::OpCode /*opCode*/) {
        /* A message was dropped due to set maxBackpressure and closeOnBackpressureLimit limit */
		std::cout << "A message was dropped due to backpressure!" << std::endl;
        };// dropped
    sb.drain =
        [](auto*/*ws*/) {
        /* Check ws->getBufferedAmount() here */
		std::cout << "Drain event, you can send more data!" << std::endl;
        };// drain
    sb.ping =
        [](auto*/*ws*/, std::string_view) {
		std::cout << "Ping received!" << std::endl;
        /* Not implemented yet */
        }; // ping
    sb.pong =
        [](auto*/*ws*/, std::string_view) {
        /* Not implemented yet */
		std::cout << "Pong received!" << std::endl;
        };// pong
    sb.close =
        [](auto*/*ws*/, int /*code*/, std::string_view /*message*/) {
        /* You may access ws->getUserData() here */
		std::cout << "A WebSocket disconnected!" << std::endl;
        }; // close


    //uWS::ws<PerSocketData> psd( std::move(sb));
    /* If you cannot use C++20 designated initializers, use positional initialization instead */
    /* uWS::App constructor does not take SSL options, so use uWS::SSLApp if you need SSL */
    uWS::App app(ctx);
    app.ws("/*", std::move(sb));
    app.listen(9000, [](auto* listen_socket) {
        if (listen_socket) {
            std::cout << "Listening on port " << 9000 << std::endl;
        }
        });
    app.run();
}


