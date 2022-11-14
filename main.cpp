#include <Windows.h>
#include <graphics.h>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <string>

struct String : public std::string {
    using std::string::string;
    operator LPCSTR() {
        return c_str();
    }
};

String operator""_ansi(const wchar_t* src, std::size_t len) {
    if (len == 0) return {};
    int bufferSize =
        WideCharToMultiByte(CP_ACP, 0, src, len, nullptr, 0, nullptr, nullptr);
    String result(bufferSize, 0);
    WideCharToMultiByte(CP_ACP, 0, src, len, result.data(), bufferSize, nullptr,
                        nullptr);
    return result;
}

int main()
{
  
  //设置字体

  int broadWidth = 80,broadHeight = 80;
  int nCol = 8,nRow = 8;
  //定义每个格子大小，定义行列数为8

  int winWidth = nCol * broadWidth + 200;
  int winHeight = nRow * broadHeight + 200;
  int x0 = winWidth / 2, y0 = winHeight / 2;
  //计算窗口大小(840 * 840)，找出中心位置

  //设置窗口大小，打开抗锯齿
  initgraph(winWidth, winHeight, INIT_RENDERMANUAL);
  ege_enable_aa(true);
  
  //设置背景为白色
  setbkcolor(WHITE);
  
  //设置文字水平中心对齐，垂直中心对齐
  settextjustify(CENTER_TEXT, CENTER_TEXT);

  //设置标题的字体和输出位置
  setfont(100,0,"微软雅黑");
  xyprintf(420,300,L"不围棋NOGO\n\n\n"_ansi);

  //设置提示的字体和位置
  setfont(20,0,"华文仿宋");
  xyprintf(420,600,L"按任意键开始游戏"_ansi);

  getch();

  cleardevice();

  //先打开界面
  //这里还要有标题
  //按任意键开始

  //从这里开始绘制主界面
  /*主界面
    NOGO不围棋
    -开始游戏
    -读取存档
    -查看历史游戏
    -退出
  */
  int bottomWidth = 160,bottomHeight = 100;
  color_t bottom = EGEARGB(210,65,105,225);
  setfillcolor(bottom);
  
  ege_fillrect(340,200,bottomWidth,bottomHeight);
  ege_fillrect(340,350,bottomWidth,bottomHeight);
  ege_fillrect(340,500,bottomWidth,bottomHeight);

  //在这中间写处理鼠标命令的代码
  getch();//按任意键继续

  cleardevice();
  
  //主界面绘制完成后执行清屏
  //从这里开始绘制棋盘（大小为9*9）（以及UI）
  color_t brown = EGEARGB(223,249,214,91);

  for(int row{};row < nRow;row++){
    for(int col{};col < nCol;col++){
      int x = col * broadWidth + 100, y = row * broadHeight + 100;
      color_t broadColor = brown;

      setfillcolor(broadColor);
      ege_fillrect(x, y, broadWidth, broadHeight);
    }
  }
  for(int row{};row < nRow;row++){
    for(int col{};col < nCol;col++){
      int x = col * broadWidth + 100, y = row * broadHeight + 100;
      setlinewidth(1);
      setcolor(EGEACOLOR(0xFF, BLACK));

      ege_rectangle(x, y, broadWidth, broadHeight);
    }
  }

  getch();

  closegraph();
  return 0;
}