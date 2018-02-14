#ifndef BUTTON_H
#define BUTTON_H
#include "Supportive.h"
#include <QWidget>
#include <QMouseEvent>
/*这里定义了自定义按钮*/
class ActionHolder;
//typedef int (ActionHolder::*actype)();
enum actype{NO,NEWGAME,SETTINGS,QUIT,ONEMORE,ONELESS,CONFIRM,FINISHONE,LOADGAME};//按钮动作枚举

class MyButton:public QObject{
Q_OBJECT
public:
    int X,Y,W,H;
    actype action;
    ActionHolder *target;
    MyButton(int x0=0,int y0=0,int w0=0,int h0=0,actype act=NO,ActionHolder* target=NULL);
    void MysetPos(vec r);
    void setText(const char* scr);
};//广义的按钮

class MyRectButton:public MyButton,public QGraphicsRectItem{
Q_OBJECT
public:
    MyRectButton(int x0=0,int y0=0,int w0=0,int h0=0,actype act=NO,ActionHolder* target=NULL,const char* scr="",QFont font=QFont(),QColor color=QColor(0,0,0));
    ~MyRectButton();
    void MysetPos(vec r);
    void setText(const char* scr);
    void addtoscene(QGraphicsScene *sce);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    MyText *txt;
signals:
    void clicked();
};//狭义的按钮

#endif // BUTTON_H
