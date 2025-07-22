#include "Server.h"

#include <iostream>
#include <algorithm>

#define MAX_LEN 200
#define PORT 10000

Server::Server() : nextId(1) {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(listenSocket, 5);

    std::cout << "Server running on port " << PORT << "\n";
}

Server::~Server() {
    closesocket(listenSocket);
    WSACleanup();
}

void Server::run() {
    acceptClients();
}

void Server::acceptClients() {
    while (true) {
        SOCKET clientSocket = accept(listenSocket, nullptr, nullptr);
        char name[MAX_LEN];
        int bytes = recv(clientSocket, name, MAX_LEN - 1, 0);
        if (bytes <= 0) {
            closesocket(clientSocket);
            continue;
        }
        name[bytes] = '\0';

        Client c{ clientSocket, name, nextId++ };
        {
            std::lock_guard<std::mutex> lock(clientsMutex);
            clients.push_back(c);
        }

        broadcast(c.name + " joined.\n", c.id);

        std::thread(&Server::handleClient, this, c).detach();
    }
}

void Server::handleClient(Client c) {
    char buffer[MAX_LEN];
    while (true) {
        int bytes = recv(c.sock, buffer, MAX_LEN - 1, 0);
        if (bytes <= 0) break;
        buffer[bytes] = '\0';
        std::string msg(buffer);

        if (msg == "#exit") {
            broadcast(c.name + " has left.\n", c.id);
            break;
        }
        else if (msg == "#list") {
            listUsers(c.sock);
        }
        else if (!msg.empty() && msg[0] == '@') {
            int space = msg.find(' ');
            if (space != std::string::npos) {
                std::string target = msg.substr(1, space - 1);
                std::string pm = msg.substr(space + 1);
                bool found = false;

                std::lock_guard<std::mutex> lock(clientsMutex);
                for (auto& client : clients) {
                    if (client.name == target) {
                        send(client.sock, ("(Private) " + c.name + ": " + pm + "\n").c_str(), MAX_LEN, 0);
                        send(c.sock, ("(Private) " + c.name + " -> " + target + ": " + pm + "\n").c_str(), MAX_LEN, 0);
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    send(c.sock, ("User not found: " + target + "\n").c_str(), MAX_LEN, 0);
                }
            }
        }
        else {
            broadcast(c.name + ": " + msg + "\n", c.id);
        }
    }

    closesocket(c.sock);
    removeClient(c.id);
}

void Server::broadcast(const std::string& message, int exceptId) {
    std::lock_guard<std::mutex> lock(clientsMutex);
    for (auto& c : clients) {
        if (c.id != exceptId) {
            send(c.sock, message.c_str(), (int)message.size(), 0);
        }
    }
}

void Server::listUsers(SOCKET sock) {
    std::lock_guard<std::mutex> lock(clientsMutex);
    std::string msg = "Online users:\n";
    for (auto& c : clients) {
        msg += " - " + c.name + "\n";
    }
    send(sock, msg.c_str(), (int)msg.size(), 0);
}

void Server::removeClient(int id) {
    std::lock_guard<std::mutex> lock(clientsMutex);
    clients.erase(std::remove_if(clients.begin(), clients.end(),
        [&](const Client& x) { return x.id == id; }), clients.end());
}
