#include <iostream>
#include <windows.h>
#include <conio.h>
using namespace std;

// 全局变量（初学者简单写法）
const int WIDTH = 20;
const int HEIGHT = 15;

// 蛇
int snakeX[100], snakeY[100];
int len;       // 蛇长度
int foodX, foodY;
char dir;      // 方向 W A S D
bool gameOver;
int score;

// 隐藏光标，防止闪烁
void HideCursor()
{
    CONSOLE_CURSOR_INFO cursor;
    cursor.dwSize = 1;
    cursor.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}

// 初始化游戏
void Init()
{
    gameOver = false;
    dir = 'D'; // 初始向右
    len = 3;
    score = 0;

    // 蛇初始位置
    snakeX[0] = WIDTH / 2;
    snakeY[0] = HEIGHT / 2;
    snakeX[1] = WIDTH / 2 - 1;
    snakeY[1] = HEIGHT / 2;
    snakeX[2] = WIDTH / 2 - 2;
    snakeY[2] = HEIGHT / 2;

    // 随机食物
    foodX = rand() % WIDTH;
    foodY = rand() % HEIGHT;
}

// 绘制地图
void Draw()
{
    // 定位光标到左上角
    COORD pos = { 0, 0 };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);

    // 上边框
    for (int i = 0; i < WIDTH + 2; i++)
        cout << "#";
    cout << endl;

    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH + 2; x++)
        {
            if (x == 0 || x == WIDTH + 1)
            {
                cout << "#"; // 左右墙
            }
            else if (x - 1 == foodX && y == foodY)
            {
                cout << "F"; // 食物
            }
            else
            {
                bool isBody = false;
                for (int i = 0; i < len; i++)
                {
                    if (snakeX[i] == x - 1 && snakeY[i] == y)
                    {
                        cout << "O"; // 蛇头
                        isBody = true;
                        break;
                    }
                }
                if (!isBody)
                {
                    for (int i = 1; i < len; i++)
                    {
                        if (snakeX[i] == x - 1 && snakeY[i] == y)
                        {
                            cout << "o"; // 蛇身体
                            isBody = true;
                            break;
                        }
                    }
                }
                if (!isBody)
                    cout << " ";
            }
        }
        cout << endl;
    }

    // 下边框
    for (int i = 0; i < WIDTH + 2; i++)
        cout << "#";
    cout << endl;
    cout << "分数：" << score << endl;
}

// 键盘输入处理
void Input()
{
    if (_kbhit()) // 检测有没有按键按下
    {
        switch (_getch())
        {
        case 'w':
        case 'W':
            if (dir != 'S') dir = 'W';
            break;
        case 's':
        case 'S':
            if (dir != 'W') dir = 'S';
            break;
        case 'a':
        case 'A':
            if (dir != 'D') dir = 'A';
            break;
        case 'd':
        case 'D':
            if (dir != 'A') dir = 'D';
            break;
        case 'x':
        case 'X':
            gameOver = true;
            break;
        }
    }
}

// 游戏逻辑更新
void Logic()
{
    // 身体跟随：后一节移动到前一节位置，从尾部往前更新
    int prevX = snakeX[0];
    int prevY = snakeY[0];
    int tempX, tempY;
    snakeX[0] += (dir == 'D') ? 1 : (dir == 'A') ? -1 : 0;
    snakeY[0] += (dir == 'S') ? 1 : (dir == 'W') ? -1 : 0;

    for (int i = 1; i < len; i++)
    {
        tempX = snakeX[i];
        tempY = snakeY[i];
        snakeX[i] = prevX;
        snakeY[i] = prevY;
        prevX = tempX;
        prevY = tempY;
    }

    // 撞墙判断
    if (snakeX[0] < 0 || snakeX[0] >= WIDTH || snakeY[0] < 0 || snakeY[0] >= HEIGHT)
        gameOver = true;

    // 撞到自己
    for (int i = 1; i < len; i++)
    {
        if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i])
            gameOver = true;
    }

    // 吃到食物
    if (snakeX[0] == foodX && snakeY[0] == foodY)
    {
        score += 10;
        len++;
        foodX = rand() % WIDTH;
        foodY = rand() % HEIGHT;
    }
}

int main()
{
    HideCursor();
    Init();
    while (!gameOver)
    {
        Draw();
        Input();
        Logic();
        Sleep(120); // 控制游戏速度，数值越小越快
    }
    cout << "\n游戏结束！最终分数：" << score << endl;
    system("pause");
    return 0;
}
