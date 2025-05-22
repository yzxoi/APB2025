// UIManager.cpp
#include "UIManager.h"
#include <graphics.h>
#include <string>
#include <vector>
#include <cmath>
using namespace std;

UIManager::UIManager() {
    // 启动窗口
    initgraph(BOARD_PIXEL, BOARD_PIXEL + 120); // 顶部留操作区
    setbkcolor(RGB(242,242,242)); // 温和浅灰
    cleardevice();
}

void UIManager::DrawBoard(const PegBoard& pegBoard, int selected_x, int selected_y, std::vector<std::pair<int,int>> hints) {
    cleardevice();
    // 绘棋盘背景
    setfillcolor(WHITE); setlinecolor(RGB(180,180,180));
    fillroundrect(MARGIN-10, MARGIN-10, BOARD_PIXEL-MARGIN+10, BOARD_PIXEL-MARGIN+10, 30, 30);

    // 绘棋盘格和棋子
    for (int x = 0; x < PegBoard::BOARD_SIZE; ++x) {
        for (int y = 0; y < PegBoard::BOARD_SIZE; ++y) {
            int val = pegBoard.board[x][y];
            int px = MARGIN + y * CELL_SIZE;
            int py = MARGIN + x * CELL_SIZE;

            if (val == -1) continue; // 无效区不画

            // 绘制底座
            setfillcolor(RGB(224, 224, 224));
            fillcircle(px+CELL_SIZE/2, py+CELL_SIZE/2, CELL_SIZE/2-2);

            // 可选中高亮
            bool isSelect = (selected_x==x && selected_y==y);
            bool isHint = false;
            for (auto& pr : hints)
                if (pr.first==x && pr.second==y) isHint = true;
            if (isSelect) {
                setfillcolor(RGB(255, 224, 0)); // 黄色高亮
                fillcircle(px+CELL_SIZE/2, py+CELL_SIZE/2, CELL_SIZE/2-8);
            } else if (isHint) {
                setfillcolor(RGB(140, 255, 140)); // 绿色提示
                fillcircle(px+CELL_SIZE/2, py+CELL_SIZE/2, CELL_SIZE/2-12);
            }

            // 棋子绘制
            if (val == 1) {
                // 渐变蓝色球体
                for (int r= CELL_SIZE/2-12; r > 0; r--) {
                    int blue = 120 + 135 * r/(CELL_SIZE/2-12);
                    setfillcolor(RGB(30, 144, blue));
                    fillcircle(px+CELL_SIZE/2, py+CELL_SIZE/2, r);
                }
                setlinecolor(RGB(24,48,98));
                circle(px+CELL_SIZE/2, py+CELL_SIZE/2, CELL_SIZE/2-12);
            }
            // 空孔用淡灰描边圈
            if (val == 0) {
                setlinecolor(RGB(180, 180, 180));
                setfillcolor(RGB(250, 250, 250));
                fillcircle(px+CELL_SIZE/2, py+CELL_SIZE/2, CELL_SIZE/2-18);
                circle(px+CELL_SIZE/2, py+CELL_SIZE/2, CELL_SIZE/2-18);
            }
        }
    }
    // 绘制标题
    settextstyle(34, 0, _T("微软雅黑"));
    setbkmode(TRANSPARENT);
    settextcolor(RGB(24,48,98));
    outtextxy(MARGIN, BOARD_PIXEL + 10, _T("Peg Solitaire 孔明棋"));
}

void UIManager::DrawButton(int x, int y, int w, int h, const wchar_t* text, bool highlight) {
    setfillcolor(highlight ? RGB(90,180,255) : RGB(210,210,210));
    solidroundrect(x, y, x+w, y+h, 16, 16);
    settextcolor(RGB(24,48,98));
    setbkmode(TRANSPARENT);
    settextstyle(24, 0, _T("微软雅黑"));
    int tw = textwidth(text);
    int th = textheight(text);
    outtextxy(x+(w-tw)/2, y+(h-th)/2, text);
}
bool UIManager::IsInButton(int mx, int my, int x, int y, int w, int h) {
    return mx>=x && mx<=x+w && my>=y && my<=y+h;
}

void UIManager::DrawTips(const std::string& msg, int remain_pegs, bool showWin, bool win) {
    // 操作区：清除旧提示
    setfillcolor(RGB(242,242,242));
    solidrectangle(0, BOARD_PIXEL, BOARD_PIXEL, BOARD_PIXEL+100);

    settextstyle(24, 0, _T("微软雅黑"));
    setbkmode(TRANSPARENT);

    // 主提示
    settextcolor(RGB(0,120,224));
    outtextxy(MARGIN, BOARD_PIXEL + 50, msg.c_str());

    // 棋子计数
    wchar_t buf[32];
    swprintf(buf, 32, L"剩余棋子: %d", remain_pegs);
    settextcolor(RGB(20,20,20));
    outtextxy(MARGIN + 260, BOARD_PIXEL + 50, buf);

    // 胜负弹窗
    if (showWin) {
        setfillcolor(win ? RGB(50,240,180) : RGB(255,180,0));
        solidroundrect(MARGIN+60, BOARD_PIXEL+20, BOARD_PIXEL-MARGIN-60, BOARD_PIXEL+90, 20, 20);
        settextstyle(36,0,_T("微软雅黑"));
        settextcolor(win?RGB(0,64,128):RGB(220,60,10));
        outtextxy(MARGIN+120, BOARD_PIXEL+36, win? L"恭喜，胜利！" : L"无路可走，失败");
    }
}
PegMove UIManager::GetUserMove(const PegBoard& pegBoard, int& select_x, int& select_y) {
    // 1. 先点选棋子，2. 再点目标格
    int stage = 0;
    int from_x = -1, from_y = -1;
    std::vector<std::pair<int,int>> hints;
    ExMessage m;
    while (true) {
        // 画按钮（撤销/重置/退出等）
        DrawButton(BOARD_PIXEL-240, BOARD_PIXEL+28, 100, 40, L"撤销");
        DrawButton(BOARD_PIXEL-120, BOARD_PIXEL+28, 100, 40, L"重开");

        if (peekmessage(&m, EX_MOUSE)) {
            if (m.message == WM_LBUTTONDOWN) {
                int mx = m.x, my = m.y;
                // 按钮
                if (IsInButton(mx, my, BOARD_PIXEL-240, BOARD_PIXEL+28, 100, 40)) {
                    // 返回撤销操作
                    return PegMove(-1, -1, -1, -1, -1, -1, true);
                }
                if (IsInButton(mx, my, BOARD_PIXEL-120, BOARD_PIXEL+28, 100, 40)) {
                    // 复原全盘
                    return PegMove(-2, -2, -2, -2, -2, -2, true);
                }
                // 棋盘内点击
                int gx = (my-MARGIN)/CELL_SIZE, gy = (mx-MARGIN)/CELL_SIZE;
                if (gx<0||gy<0||gx>=PegBoard::BOARD_SIZE||gy>=PegBoard::BOARD_SIZE) continue;
                if (pegBoard.board[gx][gy]==-1) continue;

                if (stage==0 && pegBoard.board[gx][gy]==1) {
                    // 选棋子，显示可走方向
                    from_x = gx; from_y = gy; stage=1;
                    hints.clear();
                    // 可走方向提示
                    int dirs[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
                    for (auto& d:dirs) {
                        int tx = gx+2*d[0], ty = gy+2*d[1];
                        if (pegBoard.CanMove(gx, gy, d[0], d[1]))
                            hints.push_back({tx, ty});
                    }
                    DrawBoard(pegBoard, from_x, from_y, hints);
                } else if (stage==1) {
                    // 点目标格
                    bool found = false;
                    for (auto& p:hints) {
                        if (p.first==gx && p.second==gy) found=true;
                    }
                    if (found) {
                        select_x = from_x; select_y = from_y;
                        // 确定方向
                        int dx = (gx - from_x)/2, dy = (gy - from_y)/2;
                        return PegMove(from_x, from_y, gx, gy, from_x+dx, from_y+dy, false);
                    }
                    // 如误点其它棋子，重新选
                    if (pegBoard.board[gx][gy]==1) {
                        from_x = gx; from_y = gy;
                        hints.clear();
                        int dirs[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
                        for (auto& d:dirs)
                            if (pegBoard.CanMove(gx, gy, d[0], d[1]))
                                hints.push_back({gx+2*d[0], gy+2*d[1]});
                        DrawBoard(pegBoard, from_x, from_y, hints);
                    }
                }
            }
        }
        Sleep(10);
    }
}

void UIManager::DrawMenu() {
    cleardevice();
    setbkcolor(RGB(42, 113, 181));
    cleardevice();
    settextstyle(46, 0, _T("微软雅黑"));
    setbkmode(TRANSPARENT);
    settextcolor(WHITE);
    outtextxy(BOARD_PIXEL/2-140, BOARD_PIXEL/2-80, _T("Peg Solitaire 孔明棋"));
    DrawButton(BOARD_PIXEL/2-70, BOARD_PIXEL/2, 140, 50, L"开始游戏", true);
    DrawButton(BOARD_PIXEL/2-70, BOARD_PIXEL/2+70, 140, 50, L"游戏说明");
    // 处理点击逻辑略
}