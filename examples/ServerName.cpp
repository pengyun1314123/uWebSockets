#include "App.h"

/* Note that uWS::SSLApp({options}) is the same as uWS::App() when compiled without SSL support */

int main() {

    uWS::SocketContextOptions ctx;
    ctx.key_file_name = "misc/key.pem";
    ctx.cert_file_name = "misc/cert.pem";
    ctx.passphrase = "1234";

    /* The SSL context given in SSLApp constructor is the default / catch-all context */
    uWS::SSLApp app = uWS::SSLApp(ctx).get("/*", [](auto *res, auto */*req*/) {
        res->end("Hello from catch-all context!");
    }).addServerName("*.google.*", ctx).domain("*.google.*").get("/*", [](auto *res, auto */*req*/) {
        res->end("Hello from *.google.* context!");
    }).listen(3000, [](auto *listenSocket) {
        if (listenSocket) {
            std::cout << "Listening on port " << 3000 << std::endl;
        } else {
            std::cout << "Failed to listen on port 3000" << std::endl;
        }
    }).run();
}
