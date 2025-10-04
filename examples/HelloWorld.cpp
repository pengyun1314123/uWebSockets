#include "App.h"

/* Note that uWS::SSLApp({options}) is the same as uWS::App() when compiled without SSL support */

int main() {

    uWS::SocketContextOptions ctx;
    ctx.key_file_name = "misc/key.pem";
    ctx.cert_file_name = "misc/cert.pem";
    ctx.passphrase = "1234";
    //ctx.dh_params_file_name = "misc/dhparams.pem";
    /* Overly simple hello world app */
    uWS::SSLApp(ctx).get("/*", [](auto *res, auto */*req*/) {
        res->end("Hello world!");
    }).listen(3000, [](auto *listen_socket) {
        if (listen_socket) {
            std::cout << "Listening on port " << 3000 << std::endl;
        }
    }).run();

    std::cout << "Failed to listen on port 3000" << std::endl;
}
