#include "state.h"
#include <QObject>
#include <QGraphicsRectItem>
#include<QGraphicsScene>
#include <QGraphicsTextItem>
#include <QDebug>
#include "Bullet.h"
#include "Ship.h"
#include <QColor>
int state::ownernum=0;

state::state(int id, int hp, int bulletnum, QColor mycolor):owner(id),XL(MESSAGE::wScene/2*MESSAGE::ratio),YT(-MESSAGE::hScene/2*MESSAGE::ratio*(1-2*(double)(owner-1)/MESSAGE::N)),dw(60*MESSAGE::ratio),dh(40*MESSAGE::ratio),len(200*MESSAGE::ratio)
{
   textstate.setDefaultTextColor(QColor(255,0,0));
   textstate.setPlainText("shipID  "+QString::number(owner)+"    score "+
                          QString::number(MESSAGE::scores[owner]));
   textstate.setPos(XL,YT);
   QFont font;font.setPixelSize(dw/3);font.setBold(true);textstate.setFont(font);
   hpstate.setRect(XL+dw,YT+dh,len*hp/totalhp,len/12);
   hpstate.setBrush(QBrush(mycolor,Qt::SolidPattern));
   MESSAGE::sce->addItem(&textstate);
   MESSAGE::sce->addItem(&hpstate);
   addbullet(bulletnum,mycolor);
   //qDebug()<<1;
}
void state::change(int hp,int bulletnum)
{
    textstate.setPlainText("shipID  "+QString::number(owner)+" score "+QString::number(MESSAGE::scores[owner]));
    hpstate.setRect(XL+dw,YT+dh,len*hp/totalhp,len/12);
    bulletchange(bulletnum);
}
void state::addbullet(int bulletnum,QColor mycolor)
{
    for(int i=0;i<bulletnum;i++)
    {
        bulletnumstate[i].setRect(XL+dw+i*len/20,YT+dh*2,2*wBullet*MESSAGE::ratio,2*hBullet*MESSAGE::ratio);
        bulletnumstate[i].setBrush(QBrush(mycolor,Qt::SolidPattern));
        MESSAGE::sce->addItem(bulletnumstate+i);
    }
}
void state::bulletchange(int bulletnum)
{
    for(int i=0;i<maxBullets;i++)
    {
        if(i<bulletnum)bulletnumstate[i].show();else bulletnumstate[i].hide();
    }
}
void state::destroy(){delete(this);}
