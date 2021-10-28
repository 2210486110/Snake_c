/*
写着玩的贪吃蛇
Owner:彭路
版本1.1
1.修复了结束语句乱窜
2.或许修复了蛇突然不动的bug
*/

#include <stdio.h>//这玩意不写你试试
#include <time.h>//随机种子
#include <conio.h>//监听键盘
#include <windows.h>//gotoxy

#define SNAKE_HEAD "◎"//脑壳
#define SNAKE_BODY "●"//身子
#define MAP_MODE "■"//墙
#define MAP_FOOD "□"//食物
#define MAP_WIDTH 80//地图长度(大于2)
#define MAP_HIGHT 30//地图高度(必须为2的倍数大于4)
#define MOVE_CENTER 12//地图偏移位置(必须为2的倍数可为0)
#define MAXLEN (MAP_WIDTH - 2) * (MAP_HIGHT - 4)//最大长度
#define INITLEN 3//初始长度
#define SLEEP 300//最慢行动间隔

void gotoxy(int x, int y);//必要控制函数

void creatMap();//地图创建函数

void creatFood();//生成食物

void initSnake();//初始化蛇

void moveSnake();//移动蛇函数

int direct = 'a';//方向变量
int flag = 1;//判定食物是否被吃的标记
int lastx, lasty;

struct FOOD//食物结构体
{
	int x;
	int y;
} food;

struct SNAKE//蛇结构体
{
	int x[MAXLEN];
	int y[MAXLEN];
	int currentLen;
} snake;

int statement()//判断蛇状态
{
	if (snake.x[0] == MOVE_CENTER || snake.x[0] == MAP_WIDTH + MOVE_CENTER || snake.y[0] == 0 || snake.y[0] == MAP_HIGHT - 1)
	{
		return 1;//撞墙
	}
	for (int i = 1; i < snake.currentLen; i++)
	{
		if (snake.x[0] == snake.x[i] && snake.y[0] == snake.y[i])
		{
			return 1;//吃自己
		}
	}
	return 0;
}

int main(void)//主函数
{
	creatMap();//地图初始化
	initSnake();//蛇初始化
	while (1)
	{
		if (SLEEP - (snake.currentLen - INITLEN) * 5 > 10)
		{
			Sleep(SLEEP - (snake.currentLen - INITLEN) * 5);//计算行动间隔
		}
		else
		{
			Sleep(10);//最快间隔,觉得还不够直接删掉这句
		}
		if (flag)
		{
			creatFood();//判断是否需要生成食物
		}
		lastx = snake.x[0];
		lastx = snake.y[0];
		moveSnake();//移动蛇

		if (snake.x[0] == lastx && snake.y[0] == lasty)
		{
			moveSnake();
		}
		if (statement()) //游戏结束
		{
			gotoxy(MAP_WIDTH / 2, MAP_HIGHT / 2);
			printf("GG!");
			gotoxy(MAP_WIDTH / 2, MAP_HIGHT / 2 + 1);
			printf("The length of the snake is %d", snake.currentLen);
			gotoxy(MAP_WIDTH + MOVE_CENTER + 10, 0);
			exit(0);
		}
	}
	return 0;
}

void moveSnake()//控制移动
{
	if (_kbhit())//收取键盘值
	{
		fflush(stdin);
		direct = _getch();
	}
	gotoxy(snake.x[snake.currentLen - 1], snake.y[snake.currentLen - 1]);
	printf("  ");//删除蛇尾
	for (int i = snake.currentLen - 1; i > 0; i--)//移动蛇身
	{
		snake.x[i] = snake.x[i - 1];
		snake.y[i] = snake.y[i - 1];
		gotoxy(snake.x[i], snake.y[i]);
		printf(SNAKE_BODY);
	}
	switch (direct)//判断方向
	{
		case 'w':
		case 'W':
			snake.y[0]--;
			break;
		case 's':
		case 'S':
			snake.y[0]++;
			break;
		case 'a':
		case 'A':
			snake.x[0] -= 2;
			break;
		case 'd':
		case 'D':
			snake.x[0] += 2;
			break;
	}
	gotoxy(snake.x[0], snake.y[0]);//移动并生成蛇头
	printf(SNAKE_HEAD);
	gotoxy(MAP_WIDTH + MOVE_CENTER + 10, 0);
	if (snake.x[0] == food.x && snake.y[0] == food.y)//若吃到食物则长度变长并生成新的食物
	{
		snake.currentLen++;
		flag = 1;
		creatFood();
	}
}

void initSnake()//初始化蛇懒得写
{
	snake.currentLen = INITLEN;
	snake.x[0] = MAP_WIDTH  / 2 + MOVE_CENTER;
	snake.y[0] = MAP_HIGHT / 2;
	gotoxy(snake.x[0], snake.y[0]);
	printf(SNAKE_HEAD);
	for (int i = 1; i < snake.currentLen; i++)
	{
		snake.x[i] = snake.x[i - 1] + 2;
		snake.y[i] = snake.y[i - 1];
		gotoxy(snake.x[i], snake.y[i]);
		printf(SNAKE_BODY);
	}

	gotoxy(MAP_WIDTH + MOVE_CENTER + 10, 0);
}
void creatFood()//生成食物
{
	//随机坐标
	srand(time(NULL));
	int isCreat = 1;
	do
	{
		food.x = rand() % (MAP_WIDTH  - 4 ) + MOVE_CENTER + 2;
		food.y = rand() % (MAP_HIGHT - 2 ) + 1;
		if (food .x % 2 == 0)
		{
			for (int i = 0; i < snake.currentLen; i++)
			{
				if (snake.x[i] == food.x && snake.y[i] == food.y)
				{
					isCreat = 0;//控制不出现在蛇身上
				}
			}
			if (isCreat)
			{
				gotoxy(food.x, food.y);
				printf(MAP_FOOD);
				flag = 0;
				gotoxy(MAP_WIDTH + MOVE_CENTER + 10, 0);
			}
		}
	}
	while (flag == 1);
}

void creatMap()//创建地图
{
	for (int i = 0 + MOVE_CENTER; i < MAP_WIDTH + MOVE_CENTER; i += 2)
	{
		gotoxy(i, 0); //上边
		printf(MAP_MODE);
		gotoxy(i, MAP_HIGHT - 1); //下边
		printf(MAP_MODE);
	}
	for (int i = 0; i < MAP_HIGHT; i++)
	{
		gotoxy(0 + MOVE_CENTER, i); //左边
		printf(MAP_MODE);
		gotoxy(MAP_WIDTH + MOVE_CENTER, i); //右边
		printf(MAP_MODE);
	}
	gotoxy(MAP_WIDTH + MOVE_CENTER + 10, 0);
}

void gotoxy(int x, int y)//控制函数
{
	COORD pos = {x, y};
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOut, pos);
}
