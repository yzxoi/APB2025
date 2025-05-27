#include "GameController.h"
#include <windows.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>
using namespace std;

void GameController::Run() {
    ui.DrawMenu();
    ui.DrawTips(L"����Ԥ����AI��ʾ���ܣ����Ե�...", 0, false);
    bool playing = true;
    bool useAIHint = false;
    PegMove AIHint;
    while (playing) {
        int opt=ui.DrawMenu();
        if (opt == 0) return;
        if (opt == 2) {
            ui.DrawTips(L"���򣺵�ѡ���Ӻ�ѡ���ƶ����������������Ӻ����ڿո񣬸����ӱ��Ƴ���", 0, false);
            ui.WaitForClick();
            continue;
        }
        pegBoard.Reset();
        while (true) {
            ui.DrawBoard(pegBoard, -1, -1, {}, useAIHint, AIHint);
            ui.DrawTips(L"", pegBoard.CountPegs(), true);
            if (!pegBoard.HasValidMove()) {
                bool win = (pegBoard.CountPegs() ==1 && pegBoard.board[3][3] == 1);
                ui.DrawTips(L"", pegBoard.CountPegs(), true, true, win);
                ui.WaitForClick();
                break;
            }
            int sx = -1, sy = -1;
            PegMove move = ui.GetUserMove(pegBoard, sx, sy, useAIHint, AIHint);
            if (move.from_x == -1) {
                // ����
                useAIHint = false;
                if (!history.empty()) {
                    pegBoard.Undo(history.top());
                    history.pop();
                    continue;
                }
            } else if (move.from_x == -2) {
                // �ؿ�
                useAIHint = false;
                while (!history.empty()) {
                    pegBoard.Undo(history.top());
                    history.pop();
                }
                continue;
            } else if (move.from_x == -3) {
                // use AI Hint
                useAIHint = true;
                AIHint = PegMove();

                auto formatTip = [&](double sec) {
                    wchar_t buf[128];
                    swprintf(buf, 128, L"�����С��Ѻ�ʱ %.1f �룬�� ESC ȡ��", sec);
                    return std::wstring(buf);
                    };

                std::atomic<bool> done(false), cancel(false);
                PegMove result;
                auto t0 = std::chrono::steady_clock::now();

                std::thread worker([&]() {
                    result = pegBoard.GetBestMove(cancel);
                    done = true;
                    });

                while (!done.load()) {
                    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) cancel = true;
                    wchar_t buf[128];
                    double total = std::chrono::duration<double>(
                        std::chrono::steady_clock::now() - t0
                    ).count();
                    swprintf(buf, 128, L"�����С��Ѻ�ʱ %.1f �룬�� ESC ȡ��", total);
                    ui.DrawTips(buf, pegBoard.CountPegs(), true);
                    Sleep(100);
                }

                worker.join();

                if (cancel.load()) {
                    ui.DrawTips(L"��ʾ��ȡ��", pegBoard.CountPegs(), true);
                }
                else {
                    AIHint = result;
                    wchar_t buf[128];
                    double total = std::chrono::duration<double>(
                        std::chrono::steady_clock::now() - t0
                    ).count();
                    swprintf(buf, 128, L"������ɣ���ʱ %.1f ��", total);
                    ui.DrawTips(buf, pegBoard.CountPegs(), true);
                }
                continue;
            } else {
                useAIHint = false;
                int dx = (move.to_x - move.from_x) / 2;
                int dy = (move.to_y - move.from_y) / 2;
                PegMove realMove;
                if (pegBoard.Move(move.from_x, move.from_y, dx, dy, realMove)) {
                    history.push(realMove);
                } else {
                    ui.DrawTips(L"�Ƿ��ƶ�", pegBoard.CountPegs(), true);
                }
            }
        }
    }
}