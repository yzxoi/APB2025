#include "PegBoard.h"
#include <cstring>
#include <cmath>

PegBoard::PegBoard() { Reset(); }

void PegBoard::Reset() {
    int preset[BOARD_SIZE][BOARD_SIZE] = {
        {-1,-1, 1, 1, 1,-1,-1},
        {-1,-1, 1, 1, 1,-1,-1},
        { 1, 1, 1, 1, 1, 1, 1},
        { 1, 1, 1, 0, 1, 1, 1},
        { 1, 1, 1, 1, 1, 1, 1},
        {-1,-1, 1, 1, 1,-1,-1},
        {-1,-1, 1, 1, 1,-1,-1},
    };
    memcpy(board, preset, sizeof(board));
}

bool PegBoard::CanMove(int x, int y, int dx, int dy) const {
    int nx = x + dx, ny = y + dy;
    int nx2 = x + 2*dx, ny2 = y + 2*dy;
    if (nx2 < 0 || nx2 >= BOARD_SIZE || ny2 < 0 || ny2 >= BOARD_SIZE) return false;
    return board[x][y] == 1 &&
           board[nx][ny] == 1 &&
           board[nx2][ny2] == 0 &&
           board[nx][ny] != -1 && board[nx2][ny2] != -1;
}

bool PegBoard::Move(int x, int y, int dx, int dy, PegMove& move) {
    if (!CanMove(x, y, dx, dy)) return false;
    int nx = x + dx, ny = y + dy;
    int nx2 = x + 2*dx, ny2 = y + 2*dy;
    board[x][y] = 0;
    board[nx][ny] = 0;
    board[nx2][ny2] = 1;
    move = PegMove(x, y, nx2, ny2, nx, ny);
    return true;
}

void PegBoard::Undo(const PegMove& move) {
    board[move.from_x][move.from_y] = 1;
    board[move.jumped_x][move.jumped_y] = 1;
    board[move.to_x][move.to_y] = 0;
}

bool PegBoard::HasValidMove() const {
    for (int x = 0; x < BOARD_SIZE; x++)
        for (int y = 0; y < BOARD_SIZE; y++)
            if (board[x][y] == 1)
                for (int dx = -1; dx <= 1; dx++)
                    for (int dy = -1; dy <= 1; dy++)
                        if ((abs(dx)+abs(dy)==1) && CanMove(x, y, dx, dy))
                            return true;
    return false;
}

int PegBoard::CountPegs() const {
    int cnt = 0;
    for (int x = 0; x < BOARD_SIZE; x++)
        for (int y = 0; y < BOARD_SIZE; y++)
            if (board[x][y] == 1) cnt++;
    return cnt;
}