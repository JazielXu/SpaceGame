#ifndef STATE_H
#define STATE_H
#include <QObject>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include<QGraphicsScene>
#include <Message.h>
#include <QColor>
/*定义了状态栏*/

class state:public QObject{
Q_OBJECT
public:
    static int ownernum;
    int owner;
    int XL,YT,dw,dh,len;
    QGraphicsTextItem textstate;
    QGraphicsRectItem hpstate;
    QGraphicsEllipseItem bulletnumstate[maxBullets];
    state(int id, int hp=5, int bulletnum=20,QColor mycolor=(QColor(255,0,0)));
    void destroy();
    void change(int hp,int bulletnum);
    void addbullet(int bulletnum,QColor mycolor);
    void bulletchange(int bulletnum);
};//状态栏

#endif // STATE_H
