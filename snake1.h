/*头文件引入*/
#include<stdio.h>
#include<windows.h>
#include<time.h>
#include<conio.h>

/*参数宏定义*/
#define MAP_HEIGHT 20   //地图高度
#define MAP_WIDTH 40    //地图宽度
#define UP 'w'//上下左右
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'
#define Xnum 10 //控制生成障碍物的数量


/*定义结构体*/
typedef struct  //定义食物和蛇结点的结构体
{
    int x;//坐标
    int y;
}Snakenode;

typedef struct  //定义食物结构体
{
    int x;//坐标
    int y;
    int v;
}Food;

typedef struct  //定义蛇的结构体
{
    Snakenode snakeNode[100];   //蛇长
    int length;                 //蛇的长度
    int speed;                  //蛇的速度
}Snake;

typedef struct 
{
    int num;
    int grade;
}Rank;


/*定义分数列表*/
/*函数声明*/
void GotoXY(int, int);  //光标的定位
void Hide();            //隐藏光标
int Menu();             //主菜单函数
void Help();            //帮助
void About();           //关于
void InitMap();         //地图初始化
void PrintFood();       //生成食物
int MoveSnake();        //蛇移动
int IsCorrect();        //自撞或撞墙检测
void SpeedControl();    //速度控制


























