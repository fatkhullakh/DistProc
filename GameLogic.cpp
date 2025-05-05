// GameLogic.cpp
#include "GameLogic.h"
#include <cstdlib>

void initializeBoard(GameState& state) {
    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 24; j++)
            state.board[i][j] = ' ';

    for (int i = 0; i < 2; i++) state.board[i][23] = 'W';
    for (int i = 0; i < 5; i++) state.board[i][12] = 'W';
    for (int i = 0; i < 3; i++) state.board[i][7] = 'W';
    for (int i = 0; i < 5; i++) state.board[i][5] = 'W';

    for (int i = 0; i < 2; i++) state.board[i][0] = 'B';
    for (int i = 0; i < 5; i++) state.board[i][11] = 'B';
    for (int i = 0; i < 3; i++) state.board[i][16] = 'B';
    for (int i = 0; i < 5; i++) state.board[i][18] = 'B';

    state.currentPlayer = 1;
}

void updateBoard(GameState& state, int newField, int fieldCurrent, char player) {
    for (int i = 5; i >= 0; i--) {
        if (state.board[i][fieldCurrent - 1] == player) {
            state.board[i][fieldCurrent - 1] = ' ';
            break;
        }
    }
    for (int i = 0; i < 6; i++) {
        if (state.board[i][newField - 1] == ' ') {
            state.board[i][newField - 1] = player;
            break;
        }
    }
}

void captureOpponentPawn(GameState& state, int newField, int fieldCurrent, char player) {
    char opponent = (player == 'W') ? 'B' : 'W';
    if (state.board[0][newField - 1] == opponent) {
        state.board[0][newField - 1] = ' ';
    }
    updateBoard(state, newField, fieldCurrent, player);
}

int* checkMoveOptions(const GameState& game, int field, int d1, int d2, char player) {
    static int result[2] = { 0, 0 };
    result[0] = result[1] = 0;

    int col = field - 1;
    char opponent = (player == 'W') ? 'B' : 'W';

    // Find topmost pawn in column
    char top = ' ';
    for (int i = 0; i < 6; ++i) {
        if (game.board[i][col] != ' ') {
            top = game.board[i][col];
            break;
        }
    }

    if (top != player) return result;  // Not your piece

    if (player == 'W') {
        if (d1 && col - d1 >= 0) {
            char dest = game.board[0][col - d1];
            if (dest == ' ' || dest == player) result[0] = field - d1;
        }
        if (d2 && col - d2 >= 0) {
            char dest = game.board[0][col - d2];
            if (dest == ' ' || dest == player) result[1] = field - d2;
        }
    }
    else if (player == 'B') {
        if (d1 && col + d1 < 24) {
            char dest = game.board[0][col + d1];
            if (dest == ' ' || dest == player) result[0] = field + d1;
        }
        if (d2 && col + d2 < 24) {
            char dest = game.board[0][col + d2];
            if (dest == ' ' || dest == player) result[1] = field + d2;
        }
    }

    return result;
}


void applyMove(GameState& state, int fromField, int toField, int& d1, int& d2) {
    char player = (state.currentPlayer == 1) ? 'W' : 'B';
    updateBoard(state, toField, fromField, player);
    if (abs(toField - fromField) == d1) d1 = 0;
    else if (abs(toField - fromField) == d2) d2 = 0;
}

void rollDice(int& dice1, int& dice2) {
    dice1 = (rand() % 6) + 1;
    dice2 = (rand() % 6) + 1;
}

bool isGameOver(const GameState& state, char opponent) {
    for (int j = 0; j < 24; j++) {
        for (int i = 0; i < 6; i++) {
            if (state.board[i][j] == opponent)
                return false;
        }
    }
    return true;
}

