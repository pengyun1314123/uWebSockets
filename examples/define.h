#ifndef define___h__
#define define___h__

#define WITH_LIBUV 1
#define WTH_QUIC 1
//#define WITH_BORINGSSL 1
#define WITH_SSL 1
//#define LIBUS_USE_QUIC

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Iphlpapi.lib")
#pragma comment(lib, "libcrypto.lib")
#pragma comment(lib, "libssl.lib")
//#pragma comment(lib, "liblsquic.lib")
#pragma comment(lib, "zlib.lib")
#pragma comment(lib, "uSockets.lib")
#pragma comment(lib, "uv.lib")
#pragma comment(lib, "mswsock.lib")

//#include <uwebsockets/WebSocket.h>
#include "App.h"


/* This is a simple WebSocket echo server example.
 * You may compile it with "WITH_OPENSSL=1 make" or with "make" */
typedef struct {
    /* Fill with user data */
    std::string name;
}PerSocketData;

//uWS::WebSocket<PerSocketData>;

typedef uWS::App::WebSocketBehavior<PerSocketData> MyBehavior;









#endif