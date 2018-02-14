#ifndef MYRECT_H
#define MYRECT_H
#include <QGraphicsRectItem>
#include <QObject>
#include <QKeyEvent>
#include "spaceship.h"
#include "Bullet.h"
#include "Message.h"
#include "Supportive.h"
#include "state.h"
/*重要部分，定义了飞船类，控制器类，包含了大部分游戏流程以及AI*/

class Ship:public QObject,public QGraphicsEllipseItem{
Q_OBJECT
public:
    int id,isai;
    void MykeyPressEvent(int key);
    Spaceship zhuxi;Ship *shadow;
    Ship(vec r0=vec(),vec v0=vec(),double t0=0,double w0=0,MyColor color1=BLUE,MyColor color2=RED,Ship *shadow0=NULL);
    void MysetPos(vec r,double rot=0);
    vec getCenter();
    int keyMap[6];
    void setkeyMap(int k1,int k2,int k3,int k4,int k5);
    int taf,tas,toi,tal;
    MyColor outer,inner;
    MyText txt;
    void changecolor();
    void changecolor(double rate);
    int outofbox();
    int inside(vec r);
    void bouncedetect();
    void boundarybouncedetect();
    int touched(Ship *another);
    int invalidpos(int xmax=MESSAGE::wScene/2,int xmin=-MESSAGE::wScene/2,int ymax=MESSAGE::hScene/2,int ymin=-MESSAGE::hScene/2,int rmin=nearR);
    state *mystate;
    void statechange();
    void addstate();
    void destroy();
//public slots:
    void move(int splits=1);
    void aithink();
};//飞船类！！

class MyRect0:public QObject,public QGraphicsRectItem{
Q_OBJECT
public:
    MyRect0();
    ~MyRect0();
    void keyPressEvent(QKeyEvent *event);
    Ship *manualships[MAXN];int tot,ids[MAXN];
    void addship(Ship *newship);
    QTimer *t1,*t2;
    void finishgame();
    void saveit();
public slots:
    void move();
    void checktime();
};//控制器！！

class testppp:public QObject,public QGraphicsRectItem{
Q_OBJECT
public:
    testppp();
    void MysetPos(vec r);
};//测试用，已废弃

class ALIVE{public:static int Alive[MAXN],total;static Ship** allships;};//用于保存飞船的指针
#endif // MYRECT

