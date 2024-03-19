#include "snake1.c"

int main()
{
    srand((unsigned int)time(0));//生成随机种子
    int end = 1,result;
    while(end)
    {
        result = Menu();//显示主菜单，并接受用户信息
        switch(result)
        {
            case 1:     //选择1：开始贪吃蛇游戏
                InitMap();
                while(MoveSnake());//如果返回0，则蛇停止移动
                break;
            case 2:     //选择2：显示帮助信息
                Help();
                break;
            case 3:     //选择3：显示关于信息
                About();
                break;
            case 0:     //选择0：结束程序
                end=0;
                break;
        }
    
    }
    return 0;
}