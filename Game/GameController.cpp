#include "GameController.h"
#include <iostream>
using namespace std;

void GameController::Run() {
    bool playing = true;
    while (playing) {
        int opt=ui.DrawMenu();
        if (opt == 0) return;
        if (opt == 2) {
            ui.DrawTips(L"规则：点选棋子后选择移动方向，跳过相邻棋子后落在空格，该棋子被移除。", 0, false);
            ui.WaitForClick();
            continue;
        }
        pegBoard.Reset();
        while (true) {
            ui.DrawBoard(pegBoard);
            ui.DrawTips(L"", pegBoard.CountPegs(), true);
            if (!pegBoard.HasValidMove()) {
                bool win = (pegBoard.CountPegs() <= 5);
                ui.DrawTips(L"", pegBoard.CountPegs(), true, true, win);
                ui.WaitForClick();
                break;
            }
            int sx = -1, sy = -1;
            PegMove move = ui.GetUserMove(pegBoard, sx, sy);
            if (move.from_x == -1) {
                // 撤销
                if (!history.empty()) {
                    pegBoard.Undo(history.top());
                    history.pop();
                    continue;
                }
            } else if (move.from_x == -2) {
                // 重开
                while (!history.empty()) {
                    pegBoard.Undo(history.top());
                    history.pop();
                }
                continue;
            } else {
                int dx = (move.to_x - move.from_x) / 2;
                int dy = (move.to_y - move.from_y) / 2;
                PegMove realMove;
                if (pegBoard.Move(move.from_x, move.from_y, dx, dy, realMove)) {
                    history.push(realMove);
                } else {
                    ui.DrawTips(L"非法移动", pegBoard.CountPegs(), true);
                }
            }
        }
    }
}