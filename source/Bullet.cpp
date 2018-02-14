#include "Bullet.h"
#include "Ship.h"
#include <QTimer>
#include <QPen>
#include <QDebug>

BulletList::BulletList(Bullet *bul,BulletList *nex,BulletList *pre):bullet(bul),next(nex),prev(pre){}
BulletList::~BulletList(){if(prev)prev->next=next;if(next)next->prev=prev;}
BulletManager::BulletManager():root(new BulletList())
{
    /*QTimer *timer=new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(move()));
    timer->start(interval);*/
}
BulletManager::~BulletManager(){while(root->next)delete(root->next);delete(root);}
void BulletManager::add(Bullet *newbullet)
{
    //qDebug()<<"o";
    BulletList *temp=root->next;
    root->next=new BulletList(newbullet,root->next,root);
    if(temp)temp->prev=root->next;
}
void BulletManager::move()
{
    BulletList *q;//qDebug()<<"j";
    for(BulletList *p=root->next;p;)
    {
        q=p;p=q->next;
        if(q->bullet->move()==1)delete(q);//qDebug()<<"x";
    }
}
int BulletManager::numofbullets()
{
    BulletList *p=root;int ans=0;
    for(p=p->next;p;p=p->next)ans++;
    return ans;
}

Bullet::Bullet(vec r0,vec v0,int w,int h,int Owner):owner(Owner),ha(r0,v0)
{
   setRect(0,0,w*MESSAGE::ratio,h*MESSAGE::ratio);
   //connect
   /*QTimer * timer= new QTimer();
   connect(timer,SIGNAL(timeout()),this,SLOT(move()));
   timer->start(interval);*/
   setBrush(QBrush(MyColor(color2[owner]).tocolor(1),Qt::SolidPattern));
   setPen(QPen(Qt::black,1,Qt::SolidLine));
}
void Bullet::destroy(){delete(this);}
void Bullet::MysetPos(vec r)
{
    int X=r.x-wBullet/2,Y=r.y-hBullet/2;
    setPos(X*MESSAGE::ratio,Y*MESSAGE::ratio);
}
int  Bullet::outofbox(){return ha.r.x<-MESSAGE::wScene/2||ha.r.x>MESSAGE::wScene/2||ha.r.y<-MESSAGE::hScene/2||ha.r.y>MESSAGE::hScene/2;}
int  Bullet::move(){
    //int a=ha.nextflip();
    for(int i=0;i<dts_interval;i++)ha.nextflip();
    int x=ha.r.x,y=ha.r.y;
    MysetPos(ha.r);
    if(outofbox()){destroy();return 1;}//qDebug()<<"r";
    if(x*x+y*y<nearR*nearR){destroy();return 1;}
    ha.hp+=interval;if((MESSAGE::isonesec&&(ha.hp>2000))||ALIVE::total<2){destroy();return 1;}
    return crushdetect();
}
int Bullet::crushdetect()
{
    Ship *S;
    for(int i=1;i<=MESSAGE::N;i++)if(ALIVE::Alive[i])
    {//qDebug()<<i;
        S=ALIVE::allships[i];
        if(S->toi>0)continue;
        //if(ALIVE::Alive[ALIVE::allships[i]->id])
        //{
            //if(S->id==owner)continue;
            //vec r_relative=ha.r+S->zhuxi.r*(-1),X=S->zhuxi.front();
            //double a=(r_relative&X)*2/hShip,b=(r_relative^X)*2/wShip;
            //qDebug()<<a<<' '<<b<<endl;
            //if(a*a+b*b<1)
            if(S->inside(ha.r))
            {//qDebug()<<S->zhuxi.hp;
                S->zhuxi.bulletshot(ha.v-S->zhuxi.v,ha.r-S->zhuxi.r);
                if(S->id!=owner)
                {
                    S->zhuxi.hp-=1;S->changecolor();
                    S->statechange();
                    MESSAGE::scores[owner]++;
                    if(S->zhuxi.hp<1e-3)
                    {
                        S->destroy();
                        MESSAGE::scores[owner]+=3;
                        if(ALIVE::Alive[owner])ALIVE::allships[owner]->toi+=1000;
                    }
                    if(ALIVE::Alive[owner])ALIVE::allships[owner]->statechange();
                }
                destroy();return 1;
            }
    }
    return 0;
}
