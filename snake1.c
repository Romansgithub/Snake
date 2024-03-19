#include "snake1.h"

/*全局变量的定义*/
Snake snake;            //定义蛇结构体
Food food;              //定义食物结构体
char now_Dir =RIGHT;    //初始化蛇头方向
char direction =RIGHT;  //预期蛇头方向
char foodlist[]={35,36,37};
Snakenode obs[100];     //障碍物列表
Rank Rank_1[100];   //分数列表
int rank=1;             //游戏场次

/*主菜单实现*/
int Menu()
{
    GotoXY(40,12);//通过GotoXY函数来绘制主菜单的界面
    printf("欢迎来到贪吃蛇小游戏！");
    GotoXY(43,14);
    printf("1.开始游戏");
    GotoXY(43,16);
    printf("2.帮助");
    GotoXY(43,18);
    printf("3.关于");
    GotoXY(43,20);
    printf("其他任意键退出游戏");
    Hide();
    char ch;
    int result=0;
    ch=_getch();//该函数可以实现从stdio流中读取字符而不显示在屏幕上
    switch(ch)//通过用户的输入判断要跳转的页面
    {
        case'1':result=1;break;
        case'2':result=2;break;
        case'3':result=3;break;
    }
    system("cls");//调用系统命令cls完成清屏操作
    return result;
}   
 
/*光标定位函数，将光标定位到（x,y）处*/
void GotoXY(int x,int y)
{
    HANDLE hout;//HANDLE,也叫句柄，可以看作指向指针的指针，但更为稳定
    COORD cor;//COORD是Windows头文件中内置的坐标结构体
    hout= GetStdHandle(STD_OUTPUT_HANDLE);//获得标准的输出句柄
    cor.X=x;
    cor.Y=y;
    SetConsoleCursorPosition(hout,cor);
}

/*光标隐藏函数*/
void Hide()
{
    HANDLE hout=GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cor_info ={1,0};//设置光标信息
    SetConsoleCursorInfo(hout,&cor_info);
}

/*编写菜单实现*/
void About() {
	GotoXY(30, 12);
	printf("贪吃蛇1.3");
	GotoXY(43, 12);
	printf("无限");
	Hide();		
	char ch = _getch();
	system("cls");
}


/*帮助菜单*/
void Help() {
	GotoXY(40, 12 );
	printf("w 上");
	GotoXY(40, 14);
	printf("s 下");
	GotoXY(40, 16 );
	printf("a 左");
	GotoXY(40, 18);
	printf("d 右");
	GotoXY(40, 20);
	printf("撞墙或撞到自身时游戏结束");
    GotoXY(60,12);
    printf("# shiwu1");
    GotoXY(60,14);
    printf("$ shiwu2");
    GotoXY(60,16);
    printf("X 障碍物");
	GotoXY(40, 22);
	printf("按随意键返回上级菜单");
	Hide();		
	char ch = _getch();
	system("cls");
}

/*显示分数列表*/
void ShowList()
{
    Rank temp;
    for(int i=1;i<rank+1;i++)//将上一场结束后的分数排序
    {
        for(int j=rank;j>i;j--)
        {
            if(Rank_1[j].grade>Rank_1[j-1].grade)
            {
                temp.grade=Rank_1[j].grade;temp.num=Rank_1[j].num;
                Rank_1[j].grade=Rank_1[j-1].grade;Rank_1[j].num=Rank_1[j-1].num;
                Rank_1[j-1].grade=temp.grade;Rank_1[j-1].num=temp.num;
            }
        }
    }
    for(int i=1;i<4;i++)//输出前三的排名
    {
        GotoXY(50,5+2*(i+1));
        printf("%d.第%d场得分%d",i,Rank_1[i].num,Rank_1[i].grade);
    }
}

/*初始化地图函数*/
void InitMap()
{
    ShowList();
    Hide();
    /*设置蛇头位置在地图中心*/
    snake.snakeNode[0].x=MAP_WIDTH/2-1;
    snake.snakeNode[0].y=MAP_HEIGHT/2-1;
    GotoXY(snake.snakeNode[0].x,snake.snakeNode[0].y);
    printf("@");//打印蛇头
    snake.length=3;//设置蛇长初始长度，速度，方向
    snake.speed=250;
    now_Dir=RIGHT;
    /*显示蛇身*/
    for(int i=1;i<snake.length;i++)
    {
        snake.snakeNode[i].y=snake.snakeNode[i-1].y;//设置蛇身的纵坐标位置和蛇头位置相同
        snake.snakeNode[i].x=snake.snakeNode[i-1].x-1;//设置蛇身的位置在蛇头的左边
        GotoXY(snake.snakeNode[i].x,snake.snakeNode[i].y);//移动光标到蛇身某处位置
        printf("o");//打印蛇身
    }

    /*生成地图上下边界*/
    for(int i=0;i<MAP_WIDTH;i++)
    {
        GotoXY(i,0);
        printf("-");
        GotoXY(i,MAP_HEIGHT-1);
        printf("-");
    }

    /*生成地图左右边界*/
    for(int i=1;i<MAP_HEIGHT-1;i++)
    {
        GotoXY(0,i);
        printf("|");
        GotoXY(MAP_WIDTH-1,i);
        printf("|");
    }
    /*生成障碍物*/
    int num=0;
    while(num<Xnum)
    {
        obs[num].x=rand()%(MAP_WIDTH-2)+1;
        obs[num].y=rand()%(MAP_HEIGHT-2)+1;
        for(int k=0;k<=snake.length-1;k++)//判断障碍物是否生成在蛇身上
        {
            if(snake.snakeNode[k].x==obs[num].x&&snake.snakeNode[k].y==obs[num].y)
            {
                num--;
                break;
            }
        }
        GotoXY(obs[num].x,obs[num].y);
        printf("X");
        num++;
    }

    /*生成食物*/
    PrintFood();
    GotoXY(50,5);
    printf("当前得分为0");
}

/*打印食物*/
void PrintFood()
{
    int var;//食物的种类
    int flag=1;
    while(flag)
    {
        flag=0;
        food.x=rand()%(MAP_WIDTH-2)+1;//随机设置食物的位置
        food.y=rand()%(MAP_HEIGHT-2)+1;
        for(int k=0;k<=snake.length-1;k++)//判断食物是否生成在蛇身上
        {
            if(snake.snakeNode[k].x==food.x&&snake.snakeNode[k].y==food.y)
            {
                flag=1;
                break;
            }
        }
        GotoXY(food.x,food.y);
        var=rand()%3;
        food.v=var;
        printf("%c",foodlist[var]);

    }
}

/*蛇移动函数，返回值1表示继续移动，0代表停止*/
int MoveSnake()
{
    Snakenode temp;
    int flag=0;// 记录蛇是否吃到了食物
    temp=snake.snakeNode[snake.length-1];//用来保存蛇的尾节点，在后面有用
    /*蛇身绘制*/
    for(int i=snake.length-1;i>=1;i--)
    {
        snake.snakeNode[i]=snake.snakeNode[i-1];//事实上，尾节点消失了
    }
    GotoXY(snake.snakeNode[1].x,snake.snakeNode[1].y);
    printf("o");//将原本蛇头的位置打印成了蛇身，其他结点不动



    /*蛇方向设置*/
    if(_kbhit())//若键盘输入则返回1
    {
        direction=_getch();
        switch(direction)//注意输入与当前蛇头方向相反的情况
        {
            case UP:
            if(now_Dir!=DOWN){now_Dir=direction;}break;
            case DOWN:
            if(now_Dir!=UP){now_Dir=direction;}break;
            case LEFT:
            if(now_Dir!=RIGHT){now_Dir=direction;}break;
            case RIGHT:
            if(now_Dir!=LEFT){now_Dir=direction;}break;
        }
    }



    /*蛇头绘制*/
    switch(now_Dir)//根据键盘输入方向来打印蛇头
    {
        case UP:snake.snakeNode[0].y--;break;
        case DOWN:snake.snakeNode[0].y++;break;
        case LEFT:snake.snakeNode[0].x--;break;
        case RIGHT:snake.snakeNode[0].x++;break;
    }
    GotoXY(snake.snakeNode[0].x,snake.snakeNode[0].y);
    printf("@");

    /*判断是否吃到了食物*/
    if(snake.snakeNode[0].x==food.x&&snake.snakeNode[0].y==food.y)//若吃到了
    {
        snake.length++;
        flag=1;//flag为1表示蛇吃到食物
        snake.snakeNode[snake.length-1]=temp;//若吃到食物则保留为尾节点
        if(food.v==0)snake.speed=200;
        else if(food.v==1)snake.speed=150;
        else if(food.v==2)snake.speed=100;
    }


    if(!flag)//若没吃到
    {
        GotoXY(temp.x,temp.y);
        printf(" ");
    }

    else//吃到了
    {
        PrintFood();
        GotoXY(50,5);
        printf("当前得分：%d",snake.length-3);
    }
    if(IsSelf())//如果自撞，那么截断撞后的部分
    {
        int index=IsSelf();
        for(int i=index;i<snake.length;i++)
        {
            GotoXY(snake.snakeNode[i].x,snake.snakeNode[i].y);
            printf(" ");
        }
        snake.length=index;//将蛇的长度更新
    }

    if(!IsCorrect())// 判断是否死亡
    {
        system("cls");
        GotoXY(45,14);
        printf("最终得分：%d",snake.length-3);
        Rank_1[rank].grade=snake.length-3;
        Rank_1[rank].num=rank; //更新分数列表
        rank++;//场次增加
        GotoXY(45,16);
        printf("Over~");
        GotoXY(45,18);
        printf("按任意键返回");
        char c=_getch();
        system("cls");
        return 0;
    }

    /*调整速度*/
    //SpeedControl();
    Sleep(snake.speed);//进程挂起
    
    return 1;
}


    /*蛇死亡判断*/
    int IsCorrect() //如果死亡则返回0
    {
        //撞墙了或撞障碍物了
        if(snake.snakeNode[0].x == 0 || snake.snakeNode[0].y == 0 || snake.snakeNode[0].x == MAP_WIDTH - 1 || snake.snakeNode[0].y == MAP_HEIGHT - 1)
        {
            return 0;
        }
        for(int i=0;i<Xnum;i++)
        {
            if(snake.snakeNode[0].x==obs[i].x&&snake.snakeNode[0].y==obs[i].y)
            {
                return 0;
            }
        }
            return 1;
    }
    /*判断是否自撞*/
    int IsSelf()
    {
        for(int i=1;i<snake.length-1;i++)
        {
            if(snake.snakeNode[i].x==snake.snakeNode[0].x&&snake.snakeNode[i].y==snake.snakeNode[0].y)
            {
                return i;
            }
        }
        return 0;
    }

/**/

/*
//速度调整函数
void SpeedControl()
{
    switch (snake.length) //速度与蛇长相关
    {  		
        case 6:snake.speed = 200; break;
        case 9:snake.speed = 180; break;
        case 12:snake.speed = 160; break;
        case 15:snake.speed = 140; break;
        case 18:snake.speed = 120; break;
        case 21:snake.speed = 100; break;
        case 24:snake.speed = 80; break;
        case 27:snake.speed = 60; break;
        case 30:snake.speed = 40; break;
        default:break;
    }
}
*/

