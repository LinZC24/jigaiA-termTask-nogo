#include <Windows.h>
#include <graphics.h>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>
#include <filesystem>

using namespace std::filesystem;
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

//bool firstMouse = true;

//集中定义各种颜色
color_t button = EGEARGB(210,65,105,225);
color_t hovered = EGEARGB(15,65,105,225);

//定义每个格子大小，定义行列数为8
int boardWidth = 80,boardHeight = 80;
int nCol = 8,nRow = 8;

//计算窗口大小(840 * 840)，找出中心位置
int winWidth = nCol * boardWidth + 200;
int winHeight = nRow * boardHeight + 200;
//int x0 = winWidth / 2, y0 = winHeight / 2;
  
//定义数组存储棋盘状态
int board[9][9]{};

bool visited[9][9]{};

int turnNumber{1};

int dx[4]{0,1,0,-1};
int dy[4]{-1,0,1,0};

std::ofstream game_save("save.txt");

bool has_air(int x,int y);

bool isValid(int x,int y,int color);

void put();

bool goback{};

//记录步数（本地简单实现中黑棋起手，若步数为偶数则黑棋，为奇数则白棋）
//同时满足本地双人和人机对战的需求
//int count{};

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
RectButton button0 = {
  220,500,
  400,200
};

RectButton button1 = {
	100, 200,  /* x, y */
	500, 100,   /* width, height */
};
bool newGame{};

RectButton button2 = {
  100,350,
  500,100
};
bool continueGame{};

RectButton button3 = {
  100,500,
  500,100
};
//bool loadGame{};
//bool printRules{};

RectButton button4 = {
  100,650,
  500,100
};
bool exitGame{};

RectButton button5 = {
  100,150,
  300,400
};
RectButton button6 = {
  440,150,
  300,400
};

RectButton button7 = {
  0,0,
  50,30
};
RectButton button8 = {
  750,0,
  90,30
};

RectButton button9 = {
  0,0,
  50,30
};

RectButton button10 = {
	100, 200,
	500, 100,
};
RectButton button11 = {
  100,350,
  500,100
};

RectButton button12 = {
  100,500,
  500,100
};

RectButton button13 = {
  100,650,
  500,100
};

RectButton button14 = {
  220,300,
  400,240
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
void drawBoard();

//游戏内有关操作的处理
void game();
void loadGame();
void printPauseMenu();
void pauseMenu();
void printRules();

bool inBoard(int x,int y);

//电脑落子算法
void find();

//胜负判断
bool win();
bool blackWin{},whiteWin{};

//选边
void select();
bool isBlack{},isWhite{};

//加载存档
void load();

int main()
{
  memset(board,0,sizeof(board));
  start();
  cleardevice();
  //先打开界面
  //按下鼠标开始
  menu:;
  PIMAGE background = newimage();
  getimage(background,"D:\\vscode-xege-template-main\\c90ce4f403259b4edcbcae413c7d1423.jpg");
  putimage(0,240,background);  
  drawMenu();

  //getch();
  //按任意键继续
  if(newGame) {
    //select();
    drawBoard();
    game();
  }
  if(continueGame) {
    loadGame();
    drawBoard();
    game();
  }
  if(goback) {
    cleardevice();
    goto menu;
  }
  cleardevice();
  
  drawBoard();
  game();

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
  setfont(40,0,"华文仿宋");
  xyprintf(420,600,L"按任意键开始游戏"_ansi);
  if(getch())
    return;
  //drawRectButton(&button0);

  //处理鼠标消息
  //如果鼠标移动进入该判断
  
  /*for(;is_run();delay_fps(60)){
    //当鼠标按下时进入主菜单
    while(mousemsg()){
      mouse_msg msg = getmouse();

      int x{},y{};
      if(msg.is_move()){
        x = msg.x;
        y = msg.y;
      }

      if(msg.is_left()){
        if(msg.is_down()){
          if(insideButton(&button0,x,y)){
          
          return;
          }
        }
      }
    }
    
  }  */
}

void drawMenu()
{
  
  drawButton();
  //在这中间写处理鼠标命令的代码
  int x{},y{};
  for(;is_run();delay_fps(60)){
    //bool mouseMoved = false;
    
    //记录鼠标位置
    loop:;
    while(mousemsg()){
      mouse_msg msg = getmouse();

      if(msg.is_move()){
       // mouseMoved = true;
        x = msg.x;
        y = msg.y;
      }
      //分别判断在四个按钮范围内按下鼠标的操作
      if(msg.is_left()){
        if(msg.is_down()){
          if(insideButton(&button1,x,y)){
            cleardevice();
            //std::ofstream oFile;
            newGame = true;
            return;
          }
          if(insideButton(&button2,x,y)){
            cleardevice();
            continueGame = true;
            return;
            //load();
          }
          if(insideButton(&button3,x,y)){
            closegraph();
          }
          /*if(insideButton(&button4,x,y)){
          }*/
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

  //设置字体颜色
  setcolor(BLACK);

  //打印菜单
  //在菜单文字下方打印透明的按钮实现点击功能
  settextjustify(CENTER_TEXT, CENTER_TEXT);
  setfont(75,0,"微软雅黑");
  xyprintf(420,100,L"开始游戏"_ansi);

  //设置按钮字体和背景填充模式
  setfont(35,0,"宋体");
  setbkmode(TRANSPARENT);

  //设置文字对齐方式为水平左对齐，垂直中心对齐
  settextjustify(LEFT_TEXT, CENTER_TEXT);

  //依次绘制各个按钮
  xyprintf(135,250,L"开始游戏"_ansi);
  setfillcolor(button);
  ege_fillrect(100,200,buttonWidth,buttonHeight);
  drawRectButton(&button1);

  xyprintf(135,400,L"继续游戏"_ansi);
  setfillcolor(button);
  ege_fillrect(100,350,buttonWidth,buttonHeight);
  drawRectButton(&button2);

  xyprintf(135,550,L"退出"_ansi);
  setfillcolor(button);
  ege_fillrect(100,500,buttonWidth,buttonHeight);
  drawRectButton(&button3);
  
  /*xyprintf(135,700,L"退出"_ansi);
  setfillcolor(button);
  ege_fillrect(100,650,buttonWidth,buttonHeight);
  drawRectButton(&button4);*/
}

void select()
{
  //设置按钮大小有关参数
  int buttonWidth = 300,buttonHeight = 400;

  //界面样式如下
  /*
  选择你想执的子
  黑棋 白棋
  */

  //中心对齐
  settextjustify(CENTER_TEXT, CENTER_TEXT);

  setcolor(BLACK);
  setfont(75,0,"微软雅黑");
  xyprintf(420,100,L"选择你想执的子"_ansi);

  setfont(50,0,"宋体");
  xyprintf(250,300,L"黑棋"_ansi);
  ege_fillrect(100,150,buttonWidth,buttonHeight);
  drawRectButton(&button5);

  setfont(50,0,"宋体");
  xyprintf(590,300,L"白棋"_ansi);
  ege_fillrect(440,150,buttonWidth,buttonHeight);
  drawRectButton(&button6);

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
          if(insideButton(&button5,x,y)){
            isBlack = true;
            return;
          }
          if(insideButton(&button6,x,y)){
            isWhite = true;
            return;
          }
        }
      }
    }
  }  
}

void loadGame() {
  if(exists("game_save.txt")) {
    std::ifstream game_save("save.txt");
    int color{},x{},y{};
    while(game_save >> color) {
      if(!color) break;
      game_save >> x >> y;
      if(color == 1) board[x][y] = 1;
      if(color == -1) board[x][y] = -1;
      color = 0;
    }
  }
}

void drawBoard()
{
  //主界面绘制完成后执行清屏
  //从这里开始绘制棋盘（大小为9*9）（以及UI）
  color_t brown = EGEARGB(223,249,214,91);

  settextjustify(CENTER_TEXT, CENTER_TEXT);
  
  setcolor(BLACK);
  setfont(20,0,"微软雅黑");
  
  /*setfillcolor(EGEARGB(50,65,105,225));
  ege_fillrect(0,0,50,30);
  xyprintf(25,15,L"暂停"_ansi);
  drawRectButton(&button7);*/

  setfillcolor(EGEARGB(50,65,105,225));
  ege_fillrect(750,0,90,30);
  xyprintf(800,15,L"暂停游戏"_ansi);
  drawRectButton(&button8);

  color_t boardColor = brown;

  setfillcolor(boardColor);
  ege_fillrect(50,50,740,740);

  for(int row{};row < nRow;row++){
    for(int col{};col < nCol;col++){
      int x = col * boardWidth + 100, y = row * boardHeight + 100;
      setlinewidth(1);
      setcolor(EGEACOLOR(0xFF, BLACK));

      ege_rectangle(x, y, boardWidth, boardHeight);
    }
  }

  
}

void game()
{
  //bool click{false};
  for(int x{};x < 9;x++) {
    for(int y{};y < 9;y++) {
      if(board[x][y]) {
        int bx = x * 80 + 70;
        int by = y * 80 + 70;
        if(board[x][y] == 1) {
          setfillcolor(EGEACOLOR(0xFF,BLACK));
          ege_fillellipse(bx,by,60,60);
        }
        if(board[x][y] == -1) {
          setfillcolor(EGEACOLOR(0xFF,WHITE));
          ege_fillellipse(bx,by,60,60);
        }
      }
    }
  }
  for(;is_run();delay_fps(60)){
    int boardX{},boardY{};
    //back:;
    while(mousemsg()){
      mouse_msg msg = getmouse();

      if(msg.is_move()){
        //mouseMoved = true;
        boardX = msg.x;
        boardY = msg.y;
      }
      if(msg.is_left()) {
        if(msg.is_down()) {
          boardX = msg.x;
          boardY = msg.y;
          if(boardX < 800 && boardX > 60 && boardY < 800 && boardY > 60){
            int x{},y{},bx{},by{};
            x = (boardX - 60) / 80;
            y = (boardY - 60) / 80;
            bx = x * 80 + 70;
            by = y * 80 + 70;
            if(board[x][y] == 0) {
              if(turnNumber % 2 != 0) {
                turnNumber++;
                setfillcolor(EGEACOLOR(0xFF,BLACK));
                ege_fillellipse(bx,by,60,60);
                board[x][y] = 1;
                game_save << 1 << x << y << '\n';
                boardX = 0;
                boardY = 0;
              }
              else if(turnNumber % 2 == 0) {
                turnNumber++;
                setfillcolor(EGEACOLOR(0xFF,WHITE));
                ege_fillellipse(bx,by,60,60);
                board[x][y] = -1;
                game_save << -1 << x << y << '\n';
                boardX = 0;
                boardY = 0;
              }
            }
          }
          /*if(insideButton(&button7,boardX,boardY)) {
            pauseMenu();
            drawBoard();
            for(int x{};x < 9;x++) {
              for(int y{};y < 9;y++) {
                if(board[x][y] == 1) {
                  int bx = x * 80 + 70;
                  int by = y * 80 + 70;
                  setfillcolor(EGEACOLOR(0xFF,BLACK));
                  ege_fillellipse(bx,by,60,60);
                }
                else if(board[x][y] == -1) {
                  int bx = x * 80 + 70;
                  int by = y * 80 + 70;
                  setfillcolor(EGEACOLOR(0xFF,WHITE));
                  ege_fillellipse(bx,by,60,60);
                }
              }
            }
          }*/
          if(insideButton(&button8,boardX,boardY)) {
            printPauseMenu();
            pauseMenu();
            if(goback) {
              return;
            }
            drawBoard();
            for(int x{};x < 9;x++) {
              for(int y{};y < 9;y++) {
                if(board[x][y] == 1) {
                  int bx = x * 80 + 70;
                  int by = y * 80 + 70;
                  setfillcolor(EGEACOLOR(0xFF,BLACK));
                  ege_fillellipse(bx,by,60,60);
                }
                else if(board[x][y] == -1) {
                  int bx = x * 80 + 70;
                  int by = y * 80 + 70;
                  setfillcolor(EGEACOLOR(0xFF,WHITE));
                  ege_fillellipse(bx,by,60,60);
                }
              }
            }
          }
        }
      }
      /*if(isBlack) {
        isBlack = false;
        isWhite = true;
        if(msg.is_left()){
        //count++;
          if(msg.is_down()){
            //boardX = msg.x;
            //boardY = msg.y;
            int col{},row{};
            col = ((boardX - 60) / 80) * 80 + 100;
            row = ((boardY - 60) / 80) * 80 + 100;
            if(boardX <= 800 && boardX >= 60 && boardY <= 800 && boardY >= 60 && board[row + 1][col + 1] == '0'){
              setfillcolor(EGEACOLOR(0xFF,BLACK));
              ege_fillellipse(col - 30,row - 30,60,60);
              //click = true;
              board[row + 1][col + 1] = 1;
              boardX = 0;
              boardY = 0;
              //goto back;
            }
          }
        }
      }*/
    }
  }
  
}

bool inBoard(int x,int y) {
  if(x >= 0 && x < 9 && y >= 0 && y < 9) {
    return true;
  }
  else {
    return false;
  }
}

bool has_air(int x,int y) {
  visited[x][y] = true;
  bool A{false};
  for(int i{};i < 4;i++) {
    int nx = x + dx[i],ny = y + dy[i];
    if(inBoard(nx,ny)) {
      if(board[nx][ny] == 0) {
        A = true;
      }
      if(board[nx][ny] == board[x][y] && visited[nx][ny] == false) {
        if(has_air(nx,ny)) {
          A = true;
        }
      }
    }
  }
  return A;
}

bool isValid(int x,int y,int color) {
  if(board[x][y]) {
    return false;
  }
  board[x][y] = color;
  memset(visited,0,sizeof(visited));
  if(!has_air(x,y)) {
    board[x][y] = 0;
    return false;
  }
  for(int i{};i < 4;i++) {
    int nx = x + dx[i],ny = y + dy[i];
    if(inBoard(nx,ny)) {
      if(board[nx][ny] && !visited[nx][ny]) {
        if(!has_air(nx,ny)) {
          board[x][y] = 0;
          return false;
        }
      }
    }
  }
  board[x][y] = 0;
  return false;
}

/*void find()
{
  int col{},row{};
        col = rand()%(9) + 1;
        row = rand()%(9) + 1;
        if(board[row][col] == '0'){
          if (board[row - 1][col] != 'b' || 
              board[row + 1][col] != 'b' ||
              board[row][col - 1] != 'b' ||
              board[row][col + 1] != 'b'){
                board[row][col] = 'w';
                col = (col - 1) * 80 + 100;
                row = (row - 1) * 80 + 100;
                setfillcolor(EGEACOLOR(0xFF,WHITE));
                ege_fillellipse(col - 30,row - 30,60,60);
                return;
              }
        }            
}*/

/*bool win()
{

}
*/
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

void printPauseMenu() {
  cleardevice();
  PIMAGE background = newimage();
  getimage(background,"D:\\vscode-xege-template-main\\c90ce4f403259b4edcbcae413c7d1423.jpg");
  putimage(0,240,background);
  
    //设置按钮大小有关参数
    int buttonWidth = 20,buttonHeight = 100;

    //设置文字对齐方式为水平中心对齐，垂直中心对齐
    settextjustify(CENTER_TEXT, CENTER_TEXT);

    //设置字体颜色
    setcolor(BLACK);

    setfont(75,0,"微软雅黑");
    xyprintf(420,100,L"暂停中"_ansi);

    settextjustify(LEFT_TEXT, CENTER_TEXT);
    //设置按钮字体和背景填充模式
    setfont(35,0,"宋体");
    setbkmode(TRANSPARENT);

    //依次绘制各个按钮
    xyprintf(135,250,L"继续游戏"_ansi);
    setfillcolor(button);
    ege_fillrect(100,200,buttonWidth,buttonHeight);
    drawRectButton(&button10);

    xyprintf(135,400,L"存档"_ansi);
    setfillcolor(button);
    ege_fillrect(100,350,buttonWidth,buttonHeight);
    drawRectButton(&button11);

    xyprintf(135,550,L"游戏规则"_ansi);
    setfillcolor(button);
    ege_fillrect(100,500,buttonWidth,buttonHeight);
    drawRectButton(&button12);

    xyprintf(135,700,L"退出游戏并保存"_ansi);
    setfillcolor(button);
    ege_fillrect(100,650,buttonWidth,buttonHeight);
    drawRectButton(&button13);

}

void pauseMenu() {
    for(;is_run();delay_fps(60)){
    
    //记录鼠标位置
      while(mousemsg()){
        mouse_msg msg = getmouse();
        int x{},y{};
        if(msg.is_move()){
          x = msg.x;
          y = msg.y;
        }
        //分别判断在四个按钮范围内按下鼠标的操作
        if(msg.is_left()){
          if(msg.is_down()){
            x = msg.x;
            y = msg.y;
            if(insideButton(&button10,x,y)){
              cleardevice();
              return;
            }
            if(insideButton(&button11,x,y)){
              //保存游戏有关代码
              
              return;
            }
            if(insideButton(&button12,x,y)){
              printRules();
              printPauseMenu();
            }
            if(insideButton(&button13,x,y)){
              //保存之后退出
              goback = true;
              return;
            }
          }
        }
      }
    }

}

void printRules() 
{
  cleardevice();
  

  setfont(20,0,"微软雅黑");
  setbkmode(TRANSPARENT);
  settextjustify(CENTER_TEXT,CENTER_TEXT);
  
  drawRectButton(&button9);
  xyprintf(25,15,L"返回"_ansi);

  setfont(40,0,"微软雅黑");
  xyprintf(420,30,L"游戏规则"_ansi);

  settextjustify(LEFT_TEXT,CENTER_TEXT);
  setfont(20,0,"宋体");
  xyprintf(40,100,L"1.黑子先手，双方轮流落子，落子后棋子不可移动"_ansi);
  xyprintf(40,130,L"2.对弈的目标不是吃掉对方的棋子。恰恰相反，如果一方落子后吃掉了对方的棋子，则"_ansi);
  xyprintf(40,160,L"落子一方判负"_ansi);
  xyprintf(40,190,L"3.对弈禁止自杀，落子自杀一方判负"_ansi);
  xyprintf(40,220,L"4.对弈禁止空手(pass)，空手一方判负"_ansi);
  xyprintf(40,250,L"5.对弈结果只有胜负，没有和棋"_ansi);
  xyprintf(40,280,L"6.吃子定义：一个棋子在棋盘上，与它直线紧邻的空点是这个棋子的“气”。 棋子直线"_ansi);
  xyprintf(40,310,L"紧邻的点上，如果有同色棋子存在，则它们便相互连接成一个不可分割的整体。它们的"_ansi);
  xyprintf(40,340,L"气也应一并计算。 棋子直线紧邻的点上，如果有异色棋子存在，这口气就不复存在。如"_ansi);
  xyprintf(40,370,L"所有的气均为对方所占据，便呈无气状态。无气状态的棋子不能在棋盘上存在，也就是"_ansi);
  xyprintf(40,400,L"提子。把无气之子提出盘外的手段叫“提子”"_ansi);

  int x{},y{};
  for(;is_run();delay_fps(60)){
    //bool mouseMoved = false;

    //记录鼠标位置
    while(mousemsg()){
      mouse_msg msg = getmouse();

      if(msg.is_move()){
        //mouseMoved = true;
        x = msg.x;
        y = msg.y;
      }

      if(msg.is_left()){
        if(msg.is_down()){
          if(insideButton(&button9,x,y)) {
            cleardevice();
            return;
          }
        }
      }
    }
  }  
}

void put() {
  if(turnNumber) {
    
  }
}
//如果鼠标移动悬浮时变色
/*
      /*bool redraw = false;
      if(mouseMoved){
        if(insideButton(&button1,x,y)){
          setfillcolor(hovered);
          ege_fillrect(120,200,480,100);
          //redraw = true;
        }
        else if(insideButton(&button2,x,y)){
          setfillcolor(hovered);
          ege_fillrect(120,350,480,100);
          //redraw = true;
        }
        else if(insideButton(&button3,x,y)){
          setfillcolor(hovered);
          ege_fillrect(120,500,480,100);
          //redraw = true;
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
