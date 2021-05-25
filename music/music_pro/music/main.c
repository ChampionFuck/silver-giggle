#include <stdio.h>
#include "bmp_show.h"
#include "head.h"
#include "touch.h"



void lcd_init()
{
    bmp_show("background.bmp",0,0,800,480);
    bmp_show("bar.bmp",0,0,800,47);
    for(int i=0;i<64*12;i+=64)
    {
        bmp_show("key_off.bmp",10+i,48,64,280);
    }
    bmp_show("logo.bmp",229,370,332,64);
}



int main()
{
    lcd_init();
    while(1)
    {
        int pos_x=0,pos_y=0;
        ts_action(&pos_x,&pos_y);


    }

    return 0;
}
