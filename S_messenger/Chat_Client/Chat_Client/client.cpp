#include "Client.h"

#include <iostream>

#define MAX_LEN 200
#define PORT 10000

ClientApp::ClientApp() : exitFlag(false) {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "WSAStartup failed\n";
        exit(1);
    }

    char server_ip[INET_ADDRSTRLEN];
    std::cout << "Enter server IP (e.g. 127.0.0.1): ";
    std::cin.getline(server_ip, sizeof(server_ip));

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed\n";
        WSACleanup();
        exit(1);
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    if (InetPtonA(AF_INET, server_ip, &serverAddr.sin_addr) != 1) {
        std::cerr << "Invalid IP address\n";
        closesocket(clientSocket);
        WSACleanup();
        exit(1);
    }

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed\n";
        closesocket(clientSocket);
        WSACleanup();
        exit(1);
    }
    std::cout << "--------------------------------------------------------------------------------------\n: ";
    std::cout << "----------------Welcome to the chat server--------------------------------------------\n: ";
    std::cout << "--------------------------------------------------------------------------------------\n: ";
    std::cout << "Enter your name: ";
    char name[MAX_LEN];
    std::cin.getline(name, MAX_LEN);
    send(clientSocket, name, (int)strlen(name), 0);
}

ClientApp::~ClientApp() {
    closesocket(clientSocket);
    WSACleanup();
}

void ClientApp::run() {
    std::thread receiver(&ClientApp::receiveLoop, this);
    std::thread sender(&ClientApp::sendLoop, this);

    sender.join();
    exitFlag = true;
    receiver.join();
}

void ClientApp::receiveLoop() {
    char buffer[MAX_LEN];
    while (!exitFlag) {
        int bytes = recv(clientSocket, buffer, MAX_LEN - 1, 0);
        if (bytes <= 0) break;
        buffer[bytes] = '\0';

        std::lock_guard<std::mutex> lock(coutMutex);
        std::cout << "\r" << buffer << "\nYou : ";
        std::cout.flush();
    }
    exitFlag = true;
}

void ClientApp::sendLoop() {
    while (!exitFlag) {
        std::cout << "You : ";
        std::string msg;
        std::getline(std::cin, msg);
        if (msg.empty()) continue;
        send(clientSocket, msg.c_str(), (int)msg.size(), 0);
        if (msg == "#exit") break;
    }
    exitFlag = true;
}
