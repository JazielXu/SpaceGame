#include <QDebug>
#include "Button.h"
#include "Gamecontrol.h"

MyButton::MyButton(int x0,int y0,int w0,int h0,actype act,ActionHolder* tar):X(x0),Y(y0),W(w0),H(h0),action(act),target(tar){}
void MyButton::MysetPos(vec r){X=r.x;Y=r.y;}

MyRectButton::MyRectButton(int x0,int y0,int w0,int h0,actype act,ActionHolder* tar,const char *scr,QFont font,QColor color):MyButton(x0,y0,w0,h0,act,tar),txt(new MyText(font,color))
{
    setRect(X*MESSAGE::ratio,Y*MESSAGE::ratio,W*MESSAGE::ratio,H*MESSAGE::ratio);
    setBrush(QBrush(Qt::yellow,Qt::SolidPattern));
    setText(scr);
    QFont fo=font;
    fo.setPixelSize(W/6*MESSAGE::ratio);
    txt->setFont(fo);
    //this->setMouseTracking(true);
}
MyRectButton::~MyRectButton(){delete(txt);}//qDebug()<<2;scene()->removeItem(this);qDebug()<<2;scene()->removeItem(&txt);qDebug()<<2;}
void MyRectButton::MysetPos(vec r)
{
    int X=r.x-W/2,Y=r.y-H/2;
    setRect(X*MESSAGE::ratio,Y*MESSAGE::ratio,W*MESSAGE::ratio,H*MESSAGE::ratio);//qDebug()<<r.y-H/2<<' '<<H<<' '<<this->boundingRect().height();}
}
void MyRectButton::setText(const char *scr){txt->setText(scr);}
void MyRectButton::addtoscene(QGraphicsScene *sce)
{
    sce->addItem(this);MysetPos(vec(X,Y));
    sce->addItem(txt);txt->MysetPos(vec(X,Y));
    target->connectact(this,action);
}
void MyRectButton::mousePressEvent(QGraphicsSceneMouseEvent *event){emit clicked();}
