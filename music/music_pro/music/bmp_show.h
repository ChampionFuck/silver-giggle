#ifndef BMP_SHOW_H
#define BMP_SHOW_H
#include "head.h"
int bmp_show(char *bmp_name,int x_offset,int y_offset,int bmp_x,int bmp_y)
{
    int fd_bmp,fd_lcd;
    int lcd_buf[800*480];
    int lcd_new[800*480];
    char bmp_buf[800*480*3];
    int *lcd_base;//指向lcd液晶屏的每个像素点

    //1.打开bmp图
    fd_bmp = open(bmp_name, O_RDONLY);
    if(fd_bmp == -1)
    {
        perror("open bmp");
        return -1;
    }

    //2.移动文件指针54B-----lseek()
    lseek(fd_bmp, 54, SEEK_SET);

    //3.读出bmp图的RGB数据：800*480*3B
    read(fd_bmp, bmp_buf, sizeof(bmp_buf));

    //4.关闭bmp图
    close(fd_bmp);
    //5. 将bmp图的数据转换成LCD的数据
    for(int i=0; i<bmp_x*bmp_y; i++)
        lcd_buf[i] = (bmp_buf[3*i+2]<<16) + (bmp_buf[3*i+1]<<8) + bmp_buf[3*i];
    //翻转图片
    for(int i=0; i<bmp_y; i++)
        for(int j=0; j<bmp_x; j++)
            lcd_new[j + i*bmp_x]  = lcd_buf[j + (bmp_y-1-i)*bmp_x];

    //6.打开液晶屏
    fd_lcd = open("/dev/fb0", O_RDWR);
    if(fd_lcd == -1)
    {
        perror("open lcd");
        return -1;
    }


    //7.显存的映射
        //void *mmap(void *addr, size_t length, int prot, int flags,
                //int fd, off_t offset)
    lcd_base = mmap(NULL, 800*480*4, PROT_READ|PROT_WRITE, MAP_SHARED,
                  fd_lcd, 0);
    /*若需要清除背景颜色，可以使用以下函数*/
//     bzero(&lcd_base,800*480*4);

    //把数据写在LCD上

    for(int i=y_offset;i<y_offset+bmp_y;i++)
    {

        for(int j=x_offset;j<x_offset+bmp_x;j++)
        {

            memcpy(lcd_base+j+800*i,lcd_new+j-x_offset+bmp_x*(i-y_offset),4);
        }
    }


//    for(int i=0; i<800*480;i++)
//        *(lcd_base + i) = lcd_new[i];

    munmap(lcd_base, 800*480*4);
    //8.关闭液晶屏
    close(fd_lcd);
    return 0;
}

#endif // BMP_SHOW_H
