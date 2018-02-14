#ifndef SUPPORTIVE_H
#define SUPPORTIVE_H
#include <QPen>
#include <QGraphicsTextItem>
#include "spaceship.h"
/*支持部分，定义了文字类和颜色类*/

class MyColor
{
public:
    int r,g,b;
    MyColor(int r0=0,int g0=0,int b0=0);
    MyColor(const MyColor &B);
    QColor tocolor(double rate);
};//颜色类
const MyColor RED(255,0,0),BLUE(0,0,255),GREEN(0,255,0),YELLOW(255,255,0),NA(0,0,0),PURPLE(255,0,255),SKY(0,255,255),WHITE(255,255,255),GOLDEN(255,204,0);
const MyColor color1[MAXN]={NA,RED,YELLOW,PURPLE,NA,WHITE,WHITE},color2[MAXN]={NA,BLUE,GREEN,SKY,WHITE,WHITE,WHITE};

class MyText:public QGraphicsTextItem
{
public:
    MyText();
    MyText(QFont font,QColor color=QColor(0,0,0));
    void setText(int id);
    void setText(const char* scr);
    void MysetPos(vec r);
};//文字类

#endif // SUPPORTIVE_H

