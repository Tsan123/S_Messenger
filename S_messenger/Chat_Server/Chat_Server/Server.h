#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <vector>
#include <thread>
#include <mutex>

#pragma comment(lib, "ws2_32.lib")

struct Client {
    SOCKET sock;
    std::string name;
    int id;
};

class Server {
public:
    Server();
    ~Server();
    void run();

private:
    SOCKET listenSocket;
    std::vector<Client> clients;
    std::mutex clientsMutex;
    std::mutex coutMutex;
    int nextId;

    void acceptClients();
    void handleClient(Client c);
    void broadcast(const std::string& message, int exceptId = -1);
    void listUsers(SOCKET clientSocket);
    void removeClient(int id);
};

#endif
