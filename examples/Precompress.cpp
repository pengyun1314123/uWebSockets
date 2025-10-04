#include "App.h"

int main() {
    /* ws->getUserData returns one of these */
    struct PerSocketData {
        /* Fill with user data */
    };

    /* Keeping track of last prepared message */
    uWS::PreparedMessage preparedMessage;
    std::mutex m;

    /* For demo, we create a thread that will update the precompressed message every second */
    std::thread t2([&m, &preparedMessage]() {
        for (int counter = 1; true; counter++) {
            m.lock();
            std::string newMessage = "Hello you are looking at message number " + std::to_string(counter) + " and this text should be precompressed";        
            
            /* Here the current preparedMessage is updated */
            preparedMessage = uWS::Loop::get()->prepareMessage(newMessage, uWS::OpCode::TEXT, true);
            
            m.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    });

    uWS::App app;

    uWS::App::WebSocketBehavior<PerSocketData> sb;
    /* You must only use SHARED_COMPRESSOR with precompression (can't use dedicated_compressor) */
    sb.compression = uWS::CompressOptions(uWS::SHARED_COMPRESSOR | uWS::DEDICATED_DECOMPRESSOR);
        /* Handlers */
    sb.upgrade = nullptr;
    sb.open = [](auto*/*ws*/) {
        /* Open event here, you may access ws->getUserData() which points to a PerSocketData struct */

        };
    sb.message = [&m, &preparedMessage, &app](auto* ws, std::string_view message, uWS::OpCode opCode) {

        /* First respond by echoing what they send us, without compression */
        ws->send(message, opCode, false);

        /* Send last prepared message */
        m.lock();
        ws->sendPrepared(preparedMessage);

        /* Using publish should also take preparedMessage */
        ws->subscribe("test");
        app.publishPrepared("test", preparedMessage);
        ws->unsubscribe("test");

        m.unlock();
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

    app.ws<PerSocketData>("/*", {
        
    }).listen(9001, [&t2](auto *listen_socket) {
        if (listen_socket) {
            std::cout << "Listening on port " << 9001 << std::endl;
        }
    }).run();
}
