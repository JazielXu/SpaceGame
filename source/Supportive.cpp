#include <QDebug>
#include "Supportive.h"
#include "Message.h"

MyColor::MyColor(int r0,int g0,int b0):r(r0),g(g0),b(b0){}
MyColor::MyColor(const MyColor &B){r=B.r;g=B.g;b=B.b;}
QColor MyColor::tocolor(double rate){return QColor(r,g,b,255*rate);}

MyText::MyText()
{
    QFont font;
    font.setPixelSize(wShip/5*MESSAGE::ratio);
    font.setBold(true);
    setDefaultTextColor(QColor(0,0,0));
    setFont(font);
    //setTextWidth(wShip/5);
}
MyText::MyText(QFont font,QColor color){setFont(font);setDefaultTextColor(color);}
void MyText::setText(int id){setPlainText(QString::number(id));}
void MyText::setText(const char *scr){setPlainText(QString(scr));}
void MyText::MysetPos(vec r)
{
    int X=r.x,Y=r.y;//qDebug()<<r.y-boundingRect().height()/2<<' '<<this->boundingRect().height();}
    setPos(X*MESSAGE::ratio-boundingRect().width()/2,Y*MESSAGE::ratio-boundingRect().height()/2);
}
