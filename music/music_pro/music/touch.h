#ifndef TOUCH_H
#define TOUCH_H
#include "head.h"
#include "bmp_show.h"

enum action{click,left,right,up,down};
struct position
{
    int x;
    int y;
};
int a = 0;
void *routine(void *arg)
{
    system("killall -9 madplay");
    char output[50];
    sprintf(output,"madplay note/d%d.mp3",a+1);

    system(output);
    pthread_exit(NULL);
}

int ts_action(int *value_x,int *value_y)
{
    int tp = open("/dev/input/event0",O_RDONLY);
    struct position pos1;
    struct position pos2;
    bool xread=false;
    bool yread=false;
    int i=0;
    struct input_event buf;
    pos1.x=0;pos1.y=0;pos2.x=0;pos2.y=0;
    while(1)
    {
        bzero(&buf,sizeof(buf));
        read(tp,&buf,sizeof(buf));
        //获取坐标值
        if(buf.type == EV_ABS)
        {
            if(buf.code ==ABS_X && i==0)
            {
                pos1.x = buf.value*800/1024;
                xread =true;
            }
            else if(buf.code ==ABS_X)
            {
                pos2.x = buf.value*800/1024;
            }
            if(buf.code == ABS_Y && i==0)
            {
                pos1.y = buf.value*400/600;
                yread =true;
            }
            else if(buf.code ==ABS_Y)
            {
                pos2.y = buf.value*400/600;
            }
            //要pos1的x和y的值同时获得，才能获取pos2的值
            if(xread ==true && yread == true)
            {
                i++;
            }
        }
        //如果检查不到按键按下，或者只记录一个x或者y的值，循环继续
        if(buf.type ==EV_KEY && xread ==true && yread == true)
        {

            if(buf.code ==BTN_TOUCH)
            {
                if(buf.value>0)
                {
                    a = (pos1.x-10)/64;
                    printf("屏幕被按下\n");
                    if(a<12&&pos1.y>47&&pos1.y<280)
                    {
                        pthread_t tid;
                        bmp_show("key_on.bmp",10+a*64,48,64,280);
                        pthread_create(&tid,NULL,routine,(void *)a);
                    }
                }
                else
                {
                    printf("pos1.x:%d \t pos1.y:%d\n",pos1.x,pos1.y);
                    printf("pos2.x:%d \t pos2.y:%d\n",pos2.x,pos2.y);
                    *value_x = pos1.x;*value_y =pos1.y;
                    a = (pos1.x-10)/64;
                    if(a<12&&pos1.y>47&&pos1.y<280)
                    {
                        bmp_show("key_off.bmp",10+a*64,48,64,280);
                    }
                    break;
                }
            }
        }
    }

    int data_x = pos1.x-pos2.x >0 ? pos1.x-pos2.x:pos2.x-pos1.x;
    int data_y = pos1.y-pos2.y >0 ? pos1.y-pos2.y:pos2.y-pos1.y;
    printf("data_x:%d \t data_y:%d\n", data_x,data_y);

    if(pos2.x==0&&pos2.y==0)
        return click;
    else if(pos1.x>pos2.x && data_x>data_y)
        return left;
    else if(pos2.x>pos1.x && data_x>data_y)
        return right;
    else if(pos1.y>pos2.y && data_y>data_x)
        return up;
    else if(pos2.y>pos1.y && data_y>data_x)
        return down;
    else
        return -1;

}

#endif // TOUCH_H
