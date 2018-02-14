#include <QDebug>
#include "Message.h"

void MESSAGE::setScreen(double rate)
{
    if(largescreen)rate=2;//qDebug()<<largescreen;
    hScene=hScene0*rate;wScene=wScene0*rate;
    double r1=wcom/(double)wScene,r2=hcom/(double)hScene;
    double r=ratio=(r1<r2)?r1:r2;
    hDisplay=hScene*r;wDisplay=wScene*r;
}
