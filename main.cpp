#include <Windows.h>
#include <graphics.h>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

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

bool firstMouse = true;

//集中定义各种颜色
color_t button = EGEARGB(210,65,105,225);
color_t hovered = EGEARGB(15,65,105,225);

//定义每个格子大小，定义行列数为8
int broadWidth = 80,broadHeight = 80;
int nCol = 8,nRow = 8;

//计算窗口大小(840 * 840)，找出中心位置
int winWidth = nCol * broadWidth + 200;
int winHeight = nRow * broadHeight + 200;
int x0 = winWidth / 2, y0 = winHeight / 2;
  
//定义数组存储棋盘状态
char broad[11][11]{};

//记录步数（本地简单实现中黑棋起手，若步数为偶数则黑棋，为奇数则白棋）
//同时满足本地双人和人机对战的需求
int count{};

/*定义按钮的状态
struct buttonstate{
  bool hovered;
  bool pressed; 
}*/

//定义矩形按钮
struct RectButton
{
	int x, y;
	int width, height;
};

//绘图函数
void draw();

//定义按钮，确定区域
RectButton button1 = {
	100, 200,  /* x, y */
	500, 100,   /* width, height */
};
RectButton button2 = {
  100,350,
  500,100
};
RectButton button3 = {
  100,500,
  500,100
};
RectButton button4 = {
  100,650,
  500,100
};

//绘制按钮
void drawRectButton(const RectButton* button);

//判断是否在按钮内部
bool insideButton(const RectButton* button,int x,int y);

//开始界面
void start();

//菜单界面
void drawMenu();
void drawButton();

//棋盘
void drawBroad();

//游戏内有关操作的处理
void game();

//电脑落子算法
void find();

//胜负判断
bool win();

int main()
{
  for(int i{1};i <= 9;i++){
    for(int j{1};j <= 9;j++){
      broad[i][j] = '0';
    }
  }
  start();

  //先打开界面
  //这里还要有标题
  //按下鼠标开始

  drawMenu();

  //getch();
  //按任意键继续

  cleardevice();
  
  drawBroad();

  //游戏结束，关闭窗口
  getch();
  closegraph();
  return 0;
}

void start()
{
  setcaption(L"NOGO不围棋"_ansi);
  //设置字体

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
  xyprintf(420,600,L"按下鼠标开始游戏"_ansi);

  //处理鼠标消息
  //如果鼠标移动进入该判断
  
  for(;is_run();delay_fps(60)){
    mouse_msg msg = getmouse();

    //当鼠标按下时进入主菜单
    if(msg.is_down()){
      cleardevice();
      break;
    }
  }  
}

void drawMenu()
{
  drawButton();

  //在这中间写处理鼠标命令的代码
  int x{},y{};
  for(;is_run();delay_fps(60)){
    bool mouseMoved = false;

    //记录鼠标位置
    while(mousemsg()){
      mouse_msg msg = getmouse();

      if(msg.is_move()){
        mouseMoved = true;
        x = msg.x;
        y = msg.y;
      }

      //分别判断在四个按钮范围内按下鼠标的操作
      if(msg.is_left()){
        if(msg.is_down()){
          if(insideButton(&button1,x,y)){
            cleardevice();
            drawBroad();
          }
          else if(insideButton(&button2,x,y)){
            
          }
          else if(insideButton(&button3,x,y)){
            
          }
          else if(insideButton(&button4,x,y)){
            closegraph();
          }
        }
      }

    }

  }
}

void drawButton()
{
  //设置按钮大小有关参数
  int buttonWidth = 20,buttonHeight = 100;

  //从这里开始绘制主界面
  /*主界面
    NOGO不围棋
    -开始游戏
    -读取存档
    -查看历史游戏
    -退出
  */

  //打印菜单
  //在菜单文字下方打印透明的按钮实现点击功能
  setfont(75,0,"微软雅黑");
  xyprintf(420,100,L"开始游戏"_ansi);

  //设置按钮字体和背景填充模式
  setfont(35,0,"宋体");
  setbkmode(TRANSPARENT);

  //设置文字对齐方式为水平左对齐，垂直中心对齐
  settextjustify(LEFT_TEXT, CENTER_TEXT);

  //依次绘制各个按钮
  xyprintf(135,250,L"开始游戏(S)"_ansi);
  setfillcolor(button);
  ege_fillrect(100,200,buttonWidth,buttonHeight);
  drawRectButton(&button1);

  xyprintf(135,400,L"继续游戏(C)"_ansi);
  setfillcolor(button);
  ege_fillrect(100,350,buttonWidth,buttonHeight);
  drawRectButton(&button2);

  xyprintf(135,550,L"查看历史游戏(H)"_ansi);
  setfillcolor(button);
  ege_fillrect(100,500,buttonWidth,buttonHeight);
  drawRectButton(&button3);

  xyprintf(135,700,L"退出(ESC)"_ansi);
  setfillcolor(button);
  ege_fillrect(100,650,buttonWidth,buttonHeight);
  drawRectButton(&button4);
}

void drawBroad()
{
  //主界面绘制完成后执行清屏
  //从这里开始绘制棋盘（大小为9*9）（以及UI）
  color_t brown = EGEARGB(223,249,214,91);

  setfont(20,0,"微软雅黑");
  setfillcolor(EGEARGB(50,65,105,225));
  ege_fillrect(10,10,50,30);
  xyprintf(10,25,L"暂停"_ansi);

  color_t broadColor = brown;

  setfillcolor(broadColor);
  ege_fillrect(50,50,740,740);

  for(int row{};row < nRow;row++){
    for(int col{};col < nCol;col++){
      int x = col * broadWidth + 100, y = row * broadHeight + 100;
      setlinewidth(1);
      setcolor(EGEACOLOR(0xFF, BLACK));

      ege_rectangle(x, y, broadWidth, broadHeight);
    }
  }

  game();
}

void game()
{
  bool click{false};
  int broadX{},broadY{};
  for(;is_run();delay_fps(60)){
    back:;
    while(mousemsg()){
      
      mouse_msg msg = getmouse();

      if(msg.is_left()){
        count++;
        if(msg.is_down()){
          broadX = msg.x;
          broadY = msg.y;
          int col{},row{};
          col = ((broadX - 60) / 80) * 80 + 100;
          row = ((broadY - 60) / 80) * 80 + 100;
          if(broadX <= 800 && broadX >= 60 && broadY <= 800 && broadY >= 60 && broad[row + 1][col + 1] == '0'){
            setfillcolor(EGEACOLOR(0xFF,BLACK));
            ege_fillellipse(col - 30,row - 30,60,60);
            click = true;
            broad[row + 1][col + 1] = 'b';
            broadX = 0;
            broadY = 0;
            goto back;
          }
        }
      }

      else if((count % 2 != 0) && click){
        count++;
        find();
        click = false;
      }

    }
  }
  
}

void find()
{
  int col{},row{};
        col = rand()%(9) + 1;
        row = rand()%(9) + 1;
        if(broad[row][col] == '0'){
          if (broad[row - 1][col] != 'b' || 
              broad[row + 1][col] != 'b' ||
              broad[row][col - 1] != 'b' ||
              broad[row][col + 1] != 'b'){
                broad[row][col] = 'w';
                col = (col - 1) * 80 + 100;
                row = (row - 1) * 80 + 100;
                setfillcolor(EGEACOLOR(0xFF,WHITE));
                ege_fillellipse(col - 30,row - 30,60,60);
                return;
              }
        }            
}

bool win()
{

}

bool insideButton(const RectButton* button,int x,int y)
{
  return (x >= button->x) && (y >= button->y)
      && (x <= button->x + button->width)
      && (y <= button->y + button->height);
}

void drawRectButton(const RectButton* button)
{
  setfillcolor(EGEARGB(0,0,0,0));
  ege_rectangle(button->x, button->y, 
                button->width, button->height);
}

//如果鼠标移动悬浮时变色
/*
bool redraw = false;
if(mouseMoved){
      if(insideButton(&button1,x,y)){
        setfillcolor(hovered);
        ege_fillrect(120,200,480,100);
        redraw = true;
      }
      else if(insideButton(&button2,x,y)){
        setfillcolor(hovered);
        ege_fillrect(120,350,480,100);
        redraw = true;
      }
      else if(insideButton(&button3,x,y)){
        setfillcolor(hovered);
        ege_fillrect(120,500,480,100);
        redraw = true;
      }
      else if(insideButton(&button4,x,y)){
        setfillcolor(hovered);
        ege_fillrect(120,650,480,100);
        redraw = true;
      }
      if(redraw){
        cleardevice();
        drawButton();
      }
    }*/    