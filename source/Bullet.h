#ifndef BULLET_H
#define BULLET_H
#include <QGraphicsRectItem>
#include <QObject>
#include "spaceship.h"
#include "Message.h"
/*这里包括了有关子弹的所有定义*/

class Bullet;
class BulletList
{
public:
    Bullet *bullet;
    BulletList *next,*prev;
    BulletList(Bullet *bul=NULL,BulletList *nex=NULL,BulletList *pre=NULL);
    ~BulletList();
};//管理子弹的链表节点
class BulletManager: public QObject{
Q_OBJECT
public:
    BulletList *root;
    BulletManager();
    void add(Bullet *newbullet);
    ~BulletManager();
    int numofbullets();
//public slots:
    void move();
};//子弹管理器，负责调用所有子弹的运动，销毁，全局只有一个实例

class Bullet: public QObject, public QGraphicsEllipseItem{
Q_OBJECT
public:
    int owner;
    Bullet(vec r0=vec(),vec v0=vec(),int w=wBullet,int h=hBullet,int Owner=0);
    Spaceship ha;
    void MysetPos(vec r);
    void destroy();
    int  crushdetect();
    int  outofbox();
//public slots:
    int  move();
};//子弹类

class MANAGER{public:static BulletManager *manager;};//全局的子弹管理器
#endif // BULLET_H

