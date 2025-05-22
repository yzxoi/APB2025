#include "GameController.h"
#include <iostream>
using namespace std;

void GameController::Run() {
    bool playing = true;
    while (playing) {
        ui.DrawMenu();
        int opt; cin >> opt;
        if (opt == 0) return;
        if (opt == 2) {
            cout << "规则：点选棋子后选择移动方向，跳过相邻棋子后落在空格，该棋子被移除。\n";
            continue;
        }
        pegBoard.Reset();
        while (true) {
            ui.DrawBoard(pegBoard);
            ui.DrawTips("点棋子和方向操作，撤销输入-1 -1", pegBoard.CountPegs());
            int sx = -1, sy = -1;
            PegMove move = ui.GetUserMove(pegBoard, sx, sy);
            if (move.from_x == -1) {
                // 撤销
                if (!history.empty()) {
                    pegBoard.Undo(history.top());
                    history.pop();
                    continue;
                }
            } else {
                int dx = (move.to_x - move.from_x) / 2;
                int dy = (move.to_y - move.from_y) / 2;
                PegMove realMove;
                if (pegBoard.Move(move.from_x, move.from_y, dx, dy, realMove)) {
                    history.push(realMove);
                } else {
                    ui.DrawTips("非法移动", pegBoard.CountPegs());
                }
            }
            if (!pegBoard.HasValidMove()) {
                bool win = (pegBoard.CountPegs() == 1);
                ui.DrawTips("", pegBoard.CountPegs(), true, win);
                ui.WaitForClick();
                break;
            }
        }
    }
}