#ifndef UIMANAGER_H
#define UIMANAGER_H

#include "PegBoard.h"
#include "PegMove.h"
#include <string>

// #include <graphics.h>

class UIManager {
public:
    static const int CELL_SIZE = 64;  // 单格像素
    static const int MARGIN = 40;     // 边界留白
    static const int BOARD_PIXEL = PegBoard::BOARD_SIZE * CELL_SIZE + 2 * MARGIN;

    UIManager();
    void DrawBoard(const PegBoard& pegBoard, int selected_x = -1, int selected_y = -1, std::vector<std::pair<int,int>> hints = {});
    void DrawTips(const std::string& msg, int remain_pegs, bool showWin = false, bool win = false);
    PegMove GetUserMove(const PegBoard& pegBoard, int& select_x, int& select_y);
    void DrawMenu();
    void WaitForClick();
    void DrawButton(int x, int y, int w, int h, const wchar_t* text, bool highlight=false);
    bool IsInButton(int mx, int my, int x, int y, int w, int h);
};

#endif // UIMANAGER_H