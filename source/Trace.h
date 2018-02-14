#ifndef TRACE_H
#define TRACE_H
#include <QGraphicsRectItem>
#include <QObject>
#include "spaceship.h"
#include "Ship.h"
/*定义了轨迹类，结构和子弹部分相似*/

class Tracepoint;
class TracepointList
{
public:
    Tracepoint *tracepoint;
    TracepointList *next,*prev;
    TracepointList(Tracepoint *tra=NULL,TracepointList *nex=NULL,TracepointList *pre=NULL);
    ~TracepointList();
};
class TracepointManager: public QObject{
Q_OBJECT
public:
    TracepointList *root;
    TracepointManager();
    void add(Tracepoint *newtracepoint);
    ~TracepointManager();
public slots:
    void move();
};

class Tracepoint: public QObject, public QGraphicsEllipseItem{
Q_OBJECT
public:
    int owner,toe;
    MyColor color;
    Tracepoint(vec r0=vec(),int w=wTracepoint,int h=hTracepoint,int Owner=0);
    void MysetPos(vec r);
    void destroy();
    int  fade();
};

class TRACEMANAGER{public:static TracepointManager *tracemanager;};
#endif // TRACE_H

