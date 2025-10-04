/* We simply call the root header file "App.h", giving you uWS::App and uWS::SSLApp */
#include "App.h"

/* This is a simple WebSocket "sync" upgrade example.
 * You may compile it with "WITH_OPENSSL=1 make" or with "make" */

int main() {
    /* ws->getUserData returns one of these */
    struct PerSocketData {
        /* Define your user data */
        int something;
    };

    uWS::SocketContextOptions ctx;
    ctx.key_file_name = "misc/key.pem";
    ctx.cert_file_name = "misc/cert.pem";
    ctx.passphrase = "1234";
    uWS::SSLApp::WebSocketBehavior<PerSocketData> sb;

    /* Settings */
    sb.compression = uWS::SHARED_COMPRESSOR;
    sb.maxPayloadLength = 16 * 1024;
    sb.idleTimeout = 10;
    sb.maxBackpressure = 1 * 1024 * 1024;
        /* Handlers */
    sb.upgrade = [](auto* res, auto* req, auto* context) {

        /* You may read from req only here, and COPY whatever you need into your PerSocketData.
         * PerSocketData is valid from .open to .close event, accessed with ws->getUserData().
         * HttpRequest (req) is ONLY valid in this very callback, so any data you will need later
         * has to be COPIED into PerSocketData here. */
        PerSocketData psd;
		psd.something = 13;
         /* Immediately upgrading without doing anything "async" before, is simple */
        res->template upgrade<PerSocketData>(std::move(psd), req->getHeader("sec-websocket-key"),
            req->getHeader("sec-websocket-protocol"),
            req->getHeader("sec-websocket-extensions"),
            context);

        /* If you don't want to upgrade you can instead respond with custom HTTP here,
         * such as res->writeStatus(...)->writeHeader(...)->end(...); or similar.*/

         /* Performing async upgrade, such as checking with a database is a little more complex;
          * see UpgradeAsync example instead. */
        };
    sb.open = [](auto* ws) {
        /* Open event here, you may access ws->getUserData() which points to a PerSocketData struct.
         * Here we simply validate that indeed, something == 13 as set in upgrade handler. */
        std::cout << "Something is: " << static_cast<PerSocketData*>(ws->getUserData())->something << std::endl;
        };
    sb.message = [](auto* ws, std::string_view message, uWS::OpCode opCode) {
        /* We simply echo whatever data we get */
        ws->send(message, opCode);
        };
    sb.drain = [](auto*/*ws*/) {
        /* Check ws->getBufferedAmount() here */
        };
    sb.ping = [](auto*/*ws*/, std::string_view) {
        /* You don't need to handle this one, we automatically respond to pings as per standard */
        };
    sb.pong = [](auto*/*ws*/, std::string_view) {
        /* You don't need to handle this one either */
        };
    sb.close = [](auto*/*ws*/, int /*code*/, std::string_view /*message*/) {
        /* You may access ws->getUserData() here, but sending or
         * doing any kind of I/O with the socket is not valid. */
        };

    /* Keep in mind that uWS::SSLApp({options}) is the same as uWS::App() when compiled without SSL support.
     * You may swap to using uWS:App() if you don't need SSL */
    uWS::SSLApp(ctx).ws<PerSocketData>("/*", std::move(sb)).listen(9001, [](auto *listen_socket) {
        if (listen_socket) {
            std::cout << "Listening on port " << 9001 << std::endl;
        }
    }).run();
}
