// GameLogic.h
#pragma once

struct GameState {
    char board[6][24];
    int currentPlayer; // 1 for White, 2 for Black
};

void initializeBoard(GameState& state);
void updateBoard(GameState& state, int newField, int fieldCurrent, char player);
void captureOpponentPawn(GameState& state, int newField, int fieldCurrent, char player);
int* checkMoveOptions(const GameState& state, int field, int d1, int d2, char player);
void applyMove(GameState& state, int fromField, int toField, int& d1, int& d2);
void rollDice(int& dice1, int& dice2);
bool isGameOver(const GameState& state, char opponent);
