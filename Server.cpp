#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <ctime>
#include "GameLogic.h"

#pragma comment(lib, "ws2_32.lib")
#define PORT 54000

void sendToClient(SOCKET client, const std::string& message) {
    send(client, message.c_str(), message.length(), 0);
}

bool recvFromClient(SOCKET client, std::string& out) {
    char buffer[2048] = { 0 };
    int bytesReceived = recv(client, buffer, sizeof(buffer), 0);
    if (bytesReceived <= 0) return false;
    out = std::string(buffer, bytesReceived);
    return true;
}

std::string boardToString(const GameState& state) {
    std::stringstream ss;
    ss << "BOARD_BEGIN\n";

    ss << "    ";
    for (int j = 13; j <= 24; j++) ss << (j < 10 ? " " : "") << j << " ";
    ss << "\n";

    for (int i = 0; i < 6; i++) {
        ss << "   ";
        for (int j = 12; j < 24; j++) ss << "[" << state.board[i][j] << "]";
        ss << "\n";
    }

    ss << "    ";
    for (int j = 12; j >= 1; j--) ss << (j < 10 ? " " : "") << j << " ";
    ss << "\n";

    for (int i = 5; i >= 0; i--) {
        ss << "   ";
        for (int j = 11; j >= 0; j--) ss << "[" << state.board[i][j] << "]";
        ss << "\n";
    }

    ss << "BOARD_END\n";
    return ss.str();
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverHint;
    serverHint.sin_family = AF_INET;
    serverHint.sin_port = htons(PORT);
    serverHint.sin_addr.s_addr = INADDR_ANY;

    bind(listening, (sockaddr*)&serverHint, sizeof(serverHint));
    listen(listening, SOMAXCONN);
    std::cout << "Waiting for 2 players to connect...\n";

    SOCKET client1 = accept(listening, nullptr, nullptr);
    sendToClient(client1, "Player 1 (White)\n");

    SOCKET client2 = accept(listening, nullptr, nullptr);
    sendToClient(client2, "Player 2 (Black)\n");

    std::ofstream logFile("game_log.txt", std::ios::out);
    logFile << "[INFO] Game started\n";

    GameState game;
    initializeBoard(game);

    while (true) {
        SOCKET currentClient = (game.currentPlayer == 1) ? client1 : client2;
        SOCKET otherClient = (currentClient == client1) ? client2 : client1;
        char playerChar = (game.currentPlayer == 1) ? 'W' : 'B';

        int dice1, dice2;
        rollDice(dice1, dice2);
        int d1 = dice1, d2 = dice2;
        int movesRemaining = (d1 == d2) ? 4 : 2;

        std::string boardStr = boardToString(game);
        sendToClient(currentClient, boardStr);

        std::stringstream rollMsg;
        rollMsg << "ROLL " << d1 << " " << d2 << "\n";
        sendToClient(currentClient, rollMsg.str());
        logFile << "[ROLL] Player " << game.currentPlayer << ": " << d1 << ", " << d2 << "\n";

        while (movesRemaining > 0) {
            bool validTurn = false;

            int from = -1, to = -1;

            sendToClient(currentClient, "ENTER_FROM\n");
            std::string fromStr;
            if (!recvFromClient(currentClient, fromStr)) {
                sendToClient(otherClient, "DISCONNECT\nOpponent disconnected.\n");
                goto cleanup;
            }

            if (sscanf_s(fromStr.c_str(), "%d", &from) != 1) {
                sendToClient(currentClient, "INVALID_FROM\nInvalid input.\n");
                continue;
            }

            if (from == 0) {
                // Check if there are still valid moves
                bool hasValid = false;
                for (int f = 1; f <= 24; ++f) {
                    int* opts = checkMoveOptions(game, f, d1, d2, playerChar);
                    if (opts[0] != 0 || opts[1] != 0) {
                        hasValid = true;
                        break;
                    }
                }
                if (hasValid) {
                    sendToClient(currentClient, "CANNOT_SKIP\nYou still have valid moves.\n");
                    continue;
                }
                else {
                    sendToClient(currentClient, "SKIPPED\nNo moves available, skipping turn.\n");
                    break;
                }
            }


            bool ownsPiece = false;
            for (int i = 0; i < 6; i++) {
                if (game.board[i][from - 1] == playerChar) {
                    ownsPiece = true;
                    break;
                }
            }

            if (!ownsPiece) {
                sendToClient(currentClient, "INVALID_FROM\nThat is not your piece.\n");
                continue;
            }

            int* options = checkMoveOptions(game, from, d1, d2, playerChar);
            if (options[0] == 0 && options[1] == 0) {
                sendToClient(currentClient, "INVALID_FROM\nNo moves from that field.\n");
                continue;
            }

            std::stringstream opt;
            opt << "MOVE_OPTIONS\nYou can move to: ";
            if (options[0]) opt << options[0] << " ";
            if (options[1]) opt << options[1];
            opt << "\n";
            sendToClient(currentClient, opt.str());

            sendToClient(currentClient, "ENTER_TO\n");
            std::string toStr;
            if (!recvFromClient(currentClient, toStr)) {
                sendToClient(otherClient, "DISCONNECT\nOpponent disconnected.\n");
                goto cleanup;
            }

            if (sscanf_s(toStr.c_str(), "%d", &to) != 1 || to < 1 || to > 24 || (to != options[0] && to != options[1])) {
                sendToClient(currentClient, "INVALID_MOVE\nNot a valid destination.\n");
                continue;
            }

            applyMove(game, from, to, d1, d2);
            logFile << "[MOVE] Player " << game.currentPlayer << ": " << from << " -> " << to << "\n";
            if (abs(from - to) == d1) d1 = 0;
            else if (abs(from - to) == d2) d2 = 0;

            std::stringstream moveMsg;
            moveMsg << "MOVE " << from << " " << to << "\n";
            sendToClient(client1, moveMsg.str());
            sendToClient(client2, moveMsg.str());

            std::string updatedBoard = boardToString(game);
            sendToClient(client1, updatedBoard);
            sendToClient(client2, updatedBoard);

            movesRemaining--;
            if (abs(from - to) == d1) d1 = 0;
            else if (abs(from - to) == d2) d2 = 0;

            // Check if player still has valid moves
            bool canStillMove = false;
            for (int i = 1; i <= 24; i++) {
                int* check = checkMoveOptions(game, i, d1, d2, playerChar);
                if (check[0] != 0 || check[1] != 0) {
                    canStillMove = true;
                    break;
                }
            }

            if (!canStillMove) {
                sendToClient(currentClient, "NO_VALID_MOVES\n");
                break;
            }

            // Show updated board and dice for next move
            std::string boardMid = boardToString(game);
            sendToClient(currentClient, boardMid);

            std::stringstream remRoll;
            remRoll << "ROLL " << d1 << " " << d2 << "\n";
            sendToClient(currentClient, remRoll.str());
        }

        if (isGameOver(game, (playerChar == 'W') ? 'B' : 'W')) {
            std::string winner = (playerChar == 'W') ? "Player 1 (White)" : "Player 2 (Black)";
            sendToClient(client1, "GAME_OVER\n" + winner + " wins!\n");
            sendToClient(client2, "GAME_OVER\n" + winner + " wins!\n");
            logFile << "[WIN] " << winner << "\n";
            break;
        }

        game.currentPlayer = (game.currentPlayer == 1) ? 2 : 1;
    }

cleanup:
    logFile << "[INFO] Game ended.\n";
    logFile.close();
    closesocket(client1);
    closesocket(client2);
    closesocket(listening);
    WSACleanup();
    return 0;
}
