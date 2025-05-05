/*
// Client.cpp
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <queue>

#pragma comment(lib, "ws2_32.lib")
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 54000

void sendToServer(SOCKET server, const std::string& message) {
    send(server, message.c_str(), message.length(), 0);
}

bool readLine(SOCKET sock, std::queue<std::string>& lines, std::string& buffer) {
    char chunk[512];
    int received = recv(sock, chunk, sizeof(chunk) - 1, 0);
    if (received <= 0) return false;

    chunk[received] = '\0';
    buffer += chunk;

    size_t pos;
    while ((pos = buffer.find('\n')) != std::string::npos) {
        lines.push(buffer.substr(0, pos + 1));
        buffer.erase(0, pos + 1);
    }
    return true;
}

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET server = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &hint.sin_addr);

    if (connect(server, (sockaddr*)&hint, sizeof(hint)) == SOCKET_ERROR) {
        std::cerr << "Connection failed.\n";
        return 1;
    }

    std::string playerRole = "Unknown";

    // Get welcome message (player role)
    std::string initialMsg;
    char initBuf[512] = { 0 };
    recv(server, initBuf, sizeof(initBuf) - 1, 0);
    initialMsg = std::string(initBuf);
    std::cout << "[Server] " << initialMsg;
    playerRole = initialMsg;

    std::queue<std::string> lines;
    std::string buffer;

    while (true) {
        if (lines.empty()) {
            if (!readLine(server, lines, buffer)) {
                std::cout << "Disconnected.\n";
                break;
            }
        }

        while (!lines.empty()) {
            std::string line = lines.front(); lines.pop();

            if (line.find("ENTER_FROM") != std::string::npos) {
                std::cout << "Enter source field (1-24): ";
                int from; std::cin >> from;
                sendToServer(server, std::to_string(from) + "\n");
            }
            else if (line.find("ENTER_TO") != std::string::npos) {
                std::cout << "Enter destination field: ";
                int to; std::cin >> to;
                sendToServer(server, std::to_string(to) + "\n");
            }
            else if (line.find("MOVE_OPTIONS") != std::string::npos) {
                std::cout << "[Server] " << line;
            }
            else if (line.find("ROLL") != std::string::npos) {
                std::cout << "[Dice] " << line;
            }
            else if (line.find("GAME_OVER") != std::string::npos || line.find("DISCONNECT") != std::string::npos) {
                std::cout << "[Server] " << line;
                goto cleanup;
            }
            else if (line.find("BOARD_BEGIN") != std::string::npos) {
                std::string board;
                while (line.find("BOARD_END") == std::string::npos) {
                    board += line;
                    if (lines.empty()) {
                        if (!readLine(server, lines, buffer)) break;
                    }
                    line = lines.front(); lines.pop();
                }
                board += line;
                system("cls");
                std::cout << "You are: " << playerRole;
                std::cout << "\n=== BOARD ===\n" << board << "=====================\n";
            }
            else {
                std::cout << "[Server] " << line;
            }
        }
    }

cleanup:
    closesocket(server);
    WSACleanup();
    return 0;
}

*/