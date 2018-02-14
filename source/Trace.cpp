#include "Trace.h"
#include "Ship.h"
#include <QTimer>
#include <QPen>
#include <QDebug>

TracepointList::TracepointList(Tracepoint *tra,TracepointList *nex,TracepointList *pre):tracepoint(tra),next(nex),prev(pre){}
TracepointList::~TracepointList(){if(prev)prev->next=next;if(next)next->prev=prev;}
TracepointManager::TracepointManager():root(new TracepointList())
{
    if(!MESSAGE::showtrace)return;
    QTimer *timer=new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(move()));
    timer->start(traceinterval);
}
TracepointManager::~TracepointManager(){while(root->next)delete(root->next);delete(root);}
void TracepointManager::add(Tracepoint *newtracepoint)
{
    TracepointList *temp=root->next;
    root->next=new TracepointList(newtracepoint,root->next,root);
    if(temp)temp->prev=root->next;
}
void TracepointManager::move()
{
    TracepointList *q;
    for(TracepointList *p=root->next;p;)
    {
        q=p;p=q->next;
        if(q->tracepoint->fade()==1)delete(q);
    }
}

Tracepoint::Tracepoint(vec r0,int w,int h,int Owner):owner(Owner),toe(tfadetime),color(ALIVE::allships[owner]->inner)
{
   setRect(0,0,w,h);
   setBrush(QBrush(color.tocolor(1),Qt::SolidPattern));
   setPen(QPen(Qt::transparent,1,Qt::SolidLine));
   TRACEMANAGER::tracemanager->add(this);
   MysetPos(r0);
}
void Tracepoint::destroy(){delete(this);}
void Tracepoint::MysetPos(vec r)
{
    int X=r.x-wTracepoint/2,Y=r.y-hTracepoint/2;
    setPos(X*MESSAGE::ratio,Y*MESSAGE::ratio);
}
int  Tracepoint::fade()
{
    toe-=traceinterval;if(toe<=0){destroy();return 1;}
    double rate=toe/(double)tfadetime;
    setBrush(QBrush(color.tocolor(rate),Qt::SolidPattern));
    return 0;
}
