# 孔明棋（Peg Solitaire）

## 项目简介
本项目旨在 C++ x EasyX 实现 Peg Solitaire Game 小游戏。

## 目录结构

```
PegSolitaire/
├── main.cpp          // 程序入口，主循环
├── PegBoard.h/cpp    // 棋盘及规则逻辑
├── UIManager.h/cpp   // UI显示与交互
├── GameController.h/cpp // 游戏主控
├── PegMove.h         // 单步移动数据结构
├── resource/         // 存放图片、字体等资源
└── More...
```

## 编译与运行

### 环境要求
- **开发环境**：Visual Studio（推荐使用 VS2022），或其他支持 C++ 的 IDE。
- **操作系统**：Windows，使用 `_getch()` 实现无回车输入。

### 编译步骤
1. 将所有源代码放置在同一工作目录下，按照上面的目录结构组织文件。  
2. 在 Visual Studio 中打开 `Game.sln` 。  
3. 编译并运行项目。  
4. 根据提示在主菜单选择相应功能进行测试。

## 贡献指南

欢迎提交 Issue 和 Pull Request。

## 许可证

MIT License