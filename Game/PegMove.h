#ifndef PEGMOVE_H
#define PEGMOVE_H

struct PegMove {
    int from_x, from_y;
    int to_x, to_y;
    int jumped_x, jumped_y;
    bool isUndo = false; // 标记是否为撤销操作
    PegMove(int fx=0,int fy=0,int tx=0,int ty=0,int jx=0,int jy=0,bool undo=false)
        : from_x(fx), from_y(fy), to_x(tx), to_y(ty), jumped_x(jx), jumped_y(jy), isUndo(undo) {}
};

#endif // PEGMOVE_H