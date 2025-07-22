#ifndef CLIENT_H
#define CLIENT_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <thread>
#include <mutex>

#pragma comment(lib, "ws2_32.lib")

class ClientApp {
public:
    ClientApp();
    ~ClientApp();
    void run();

private:
    SOCKET clientSocket;
    std::mutex coutMutex;
    bool exitFlag;

    void receiveLoop();
    void sendLoop();
};

#endif
