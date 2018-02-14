#include "Ship.h"
#include<QKeyEvent>
#include<QGraphicsScene>
#include "Bullet.h"
#include "Gamecontrol.h"
#include "Trace.h"
#include "Button.h"
#include "File.h"
#include <QDebug>
#include <QTimer>
#include <QPen>
#include <QString>
#include <QFont>
#define SPLITN 1000
#define INF 32767

void Ship::changecolor(double rate)
{
    setPen(QPen(outer.tocolor(rate),1,Qt::SolidLine));
    setBrush(QBrush(inner.tocolor(rate),Qt::SolidPattern));
    if(shadow)shadow->changecolor(rate);
}
void Ship::changecolor(){changecolor(zhuxi.hp/totalhp);}
Ship::Ship(vec r0,vec v0,double t0,double w0,MyColor color1,MyColor color2,Ship *shadow0):zhuxi(r0,v0,t0,w0,totalhp),shadow(shadow0),outer(color1),inner(color2),txt()
{
    setRect(0,0,wShip*MESSAGE::ratio,hShip*MESSAGE::ratio);
    if(shadow)
    {
        //QTimer * timer= new QTimer();
        //connect(timer,SIGNAL(timeout()),this,SLOT(move()));
        //timer->start(interval);
        taf=tas=toi=tal=0;
    }
    changecolor();
    setStartAngle((shadow?-1:1)*90*16);setSpanAngle(180*16);
    MysetPos(zhuxi.r,zhuxi.t);
}
void Ship::MykeyPressEvent(int key)
{
    if(key==keyMap[0]){
        setPos(x()-10,y());
    }
    if((taf>=ROF)&&(zhuxi.n>0))
    {
        if(key==keyMap[1]){
            vec r_relative=zhuxi.transr(wShip/2,0),v_relative=zhuxi.transr(BulletSpeed,0);
            Bullet * bullet =new Bullet(zhuxi.r+r_relative,zhuxi.fire(v_relative,r_relative),wBullet,hBullet,id);
            bullet->MysetPos(bullet->ha.r);
            scene()->addItem(bullet);
            MANAGER::manager->add(bullet);
            taf=0;zhuxi.n--;
            statechange();
        }
        else if(key==keyMap[2]){
            vec r_relative=zhuxi.transr(-wShip/2,0),v_relative=zhuxi.transr(-BulletSpeed,0);
            Bullet * bullet =new Bullet(zhuxi.r+r_relative,zhuxi.fire(v_relative,r_relative),wBullet,hBullet,id);
            bullet->MysetPos(bullet->ha.r);
            scene()->addItem(bullet);
            MANAGER::manager->add(bullet);
            taf=0;zhuxi.n--;
            statechange();
        }
    }
    if(tas>=ROS)
    {
        if(key==keyMap[3])
        {
            zhuxi.w-=maxdw*ROS/1000.0;
            tas=0;
        }
        else if(key==keyMap[4])
        {
            zhuxi.w+=maxdw*ROS/1000.0;
            tas=0;
        }
    }
    if(key==keyMap[5])destroy();
//qDebug()<<zhuxi.w;
}
vec Ship::getCenter(){return Rot(vec(wShip/2,hShip/2),zhuxi.t);}
void Ship::MysetPos(vec r,double rot)
{
    vec position=r-getCenter();
    int X=position.x,Y=position.y;
    setPos(X*MESSAGE::ratio,Y*MESSAGE::ratio);
    setRotation(rot);
}
void Ship::destroy()
{
    if(shadow){ALIVE::Alive[id]=0;ALIVE::total--;shadow->destroy();}else{delete(this);return;}
    int ruins=0xfffe&((int)zhuxi.hp*4+zhuxi.n+8);//qDebug()<<(int)zhuxi.hp*4<<' '<<zhuxi.n;
    for(int i=0;i<ruins;i++)
    {
        double t=i*2*PI/ruins;int r_shot=(i&1)?wShip/3:wShip/6;double v_r=BulletSpeed,v_tao=zhuxi.w*r_shot;
        vec r_relative=Rot(zhuxi.transr(r_shot,0),t),v_relative=Rot(zhuxi.transr(v_r,v_tao),t);
        Bullet * bullet =new Bullet(zhuxi.r+r_relative,zhuxi.v+v_relative,wBullet,hBullet,id);
        bullet->MysetPos(bullet->ha.r);
        scene()->addItem(bullet);
        MANAGER::manager->add(bullet);
    }
    mystate->destroy();
    MESSAGE::scores[id]--;
    delete(this);
    if(ALIVE::total==1)MESSAGE::finished=1;
}
int Ship::outofbox(){return zhuxi.r.x<-MESSAGE::wScene/2||zhuxi.r.x>MESSAGE::wScene/2||zhuxi.r.y<-MESSAGE::hScene/2||zhuxi.r.y>MESSAGE::hScene/2;}
void Ship::move(int splits){
    if(shadow)
    {
        for(int i=0;i<dts_interval/splits;i++)zhuxi.nextflip();
        if(outofbox()||R2(zhuxi.r)<nearR*nearR||zhuxi.w>deadlyspin*2*PI||zhuxi.w<-deadlyspin*2*PI){destroy();return;}
        //if(){destroy();return;}
        shadow->zhuxi=zhuxi;
        shadow->move();
        txt.MysetPos(zhuxi.r);
    }
    MysetPos(zhuxi.r,180*zhuxi.t/PI);
    //txt.setText(taf);
    //setFocus();
    int wid=MESSAGE::ratio*8*sin(MyGame::gametime*2*PI/500);if(wid<0)wid=-wid;
    if(shadow)shadow->toi=toi;if(toi>0)setPen(QPen(QColor(255,204,0),wid,Qt::SolidLine));else changecolor();
    if(shadow&&MESSAGE::showtrace)scene()->addItem(new Tracepoint(zhuxi.r,wTracepoint,hTracepoint,id));
    //qDebug()<<zhuxi.calcE();oz
}
void Ship::setkeyMap(int k1, int k2, int k3, int k4, int k5)
{
    keyMap[0]=Qt::Key_Enter;
    keyMap[1]=k1;keyMap[2]=k2;keyMap[3]=k3;keyMap[4]=k4;keyMap[5]=k5;
}
int  Ship::inside(vec r)
{
    vec r_relative=r-zhuxi.r,X=zhuxi.front();
    double a=(r_relative&X)*2/hShip,b=(r_relative^X)*2/wShip;
    return (a*a+b*b<1);
}
void Ship::bouncedetect()
{
    Ship *S;double t;int i,j,counts;vec testpoint,sum,r_r;
    for(i=id+1;i<=MESSAGE::N;i++)if(ALIVE::Alive[i])
    {
        S=ALIVE::allships[i];if(S==this)continue;
        if(R2(S->zhuxi.r-zhuxi.r)>wShip*wShip)continue;
        //testppp *ppp=new testppp;ppp->setRect(0,0,4,4);ppp->MysetPos(zhuxi.r);MESSAGE::sce->addItem(ppp);
        for(j=0,counts=0,sum=vec(0,0);j<SPLITN;j++)
        {//qDebug()<<i;
            t=j*2*PI/SPLITN;
            r_r=zhuxi.transr(cos(t)*wShip/2,sin(t)*hShip/2);
            testpoint=r_r+zhuxi.r;
            if(S->inside(testpoint)){counts++;sum=sum+r_r;}
            //if(testpoint.x>MESSAGE::wScene/2)bouncecalc2(zhuxi,testpoint,vec(wScene/2,0),vec(-1,0));
        }
        if(counts>0)
        {
            r_r=sum*(1.0/counts);testpoint=zhuxi.r+r_r;
            double nx=(r_r^zhuxi.transr(1,0))*hShip*hShip,ny=(r_r^zhuxi.transr(0,1))*wShip*wShip;
            vec n=zhuxi.transr(nx,ny);n=n*Invabs(n);
            bouncecalc(zhuxi,S->zhuxi,testpoint,n);
           // testppp *ppp=new testppp;ppp->setRect(0,0,4,4);ppp->MysetPos(zhuxi.r);MESSAGE::sce->addItem(ppp);
            //if(S->zhuxi.hp<0)S->destroy();if(zhuxi.hp<0){destroy();return;}
            break;
        }
    }
    if(MESSAGE::stupid||toi>0)boundarybouncedetect();
}
void Ship::boundarybouncedetect()
{
    double t/*,DX=1e-1*/;int i;vec testpoint,r_r,n;
    for(i=0;i<SPLITN;i++)
    {
        t=i*2*PI/SPLITN;
        r_r=zhuxi.transr(cos(t)*wShip/2,sin(t)*hShip/2);
        testpoint=r_r+zhuxi.r;
        //if(testpoint.x> MESSAGE::wScene/2){/*qDebug()<<id<<' '<<i<<" 1";*/zhuxi.bouncewithwall(testpoint,n=vec( 1,0));while(invalidpos(MESSAGE::wScene/2,-INF,INF,-INF,0))zhuxi.r=zhuxi.r+n*(-DX);}
        //if(testpoint.x<-MESSAGE::wScene/2){/*qDebug()<<id<<' '<<i<<" 2";*/zhuxi.bouncewithwall(testpoint,n=vec(-1,0));while(invalidpos(INF,-MESSAGE::wScene/2,INF,-INF,0))zhuxi.r=zhuxi.r+n*(-DX);}
        //if(testpoint.y> MESSAGE::hScene/2){/*qDebug()<<id<<' '<<i<<" 3";*/zhuxi.bouncewithwall(testpoint,n=vec(0, 1));while(invalidpos(INF,-INF,MESSAGE::hScene/2,-INF,0))zhuxi.r=zhuxi.r+n*(-DX);}
        //if(testpoint.y<-MESSAGE::hScene/2){/*qDebug()<<id<<' '<<i<<" 4";*/zhuxi.bouncewithwall(testpoint,n=vec(0,-1));while(invalidpos(INF,-INF,INF,-MESSAGE::hScene/2,0))zhuxi.r=zhuxi.r+n*(-DX);}
        //if(R2(testpoint)<nearR*nearR){n=testpoint*-(Invabs(testpoint));zhuxi.bouncewithwall(testpoint,n);while(invalidpos())zhuxi.r=zhuxi.r+n*(-DX);}
        if(testpoint.x> MESSAGE::wScene/2)zhuxi.bouncewithwall(testpoint,n=vec( 1,0));
        if(testpoint.x<-MESSAGE::wScene/2)zhuxi.bouncewithwall(testpoint,n=vec(-1,0));
        if(testpoint.y> MESSAGE::hScene/2)zhuxi.bouncewithwall(testpoint,n=vec(0, 1));
        if(testpoint.y<-MESSAGE::hScene/2)zhuxi.bouncewithwall(testpoint,n=vec(0,-1));
        if(R2(testpoint)<nearR*nearR){n=testpoint*-(Invabs(testpoint));zhuxi.bouncewithwall(testpoint,n);}
    }
    if(zhuxi.hp<0)destroy();
}
/*int Ship::touched(Ship *another)
{
    double t;vec testpoint;
    for(int i=0;i<SPLITN;i++)
    {
        t=i*2*PI/SPLITN;
        testpoint=zhuxi.r+zhuxi.transr(cos(t)*wShip/2,sin(t)*hShip/2)*1.05;
        if(another->inside(testpoint))return 1;
    }
    return 0;
}
int Ship::invalidpos(int xmax,int xmin,int ymax,int ymin,int rmin)
{
    double t;vec testpoint;
    for(int i=0;i<SPLITN;i++)
    {
        t=i*2*PI/SPLITN;
        testpoint=zhuxi.r+zhuxi.transr(cos(t)*wShip/2,sin(t)*hShip/2)*1.05;
        if(testpoint.x>xmax||testpoint.x<xmin||testpoint.y>ymax||testpoint.y<ymin)return 1;
        if(R2(testpoint)<rmin*rmin)return 1;
    }
    return 0;
}*/
void Ship::addstate(){mystate=new state(id,5,20,inner.tocolor(1.0));}
void Ship::statechange(){mystate->change(zhuxi.hp,zhuxi.n);}
void Ship::aithink()
{
#define f1 MykeyPressEvent(keyMap[1])
#define f2 MykeyPressEvent(keyMap[2])
#define tl MykeyPressEvent(keyMap[3])
#define tr MykeyPressEvent(keyMap[4])
    if(MESSAGE::aimode==2)return;
    double W=zhuxi.w;int X=zhuxi.r.x,Y=zhuxi.r.y;vec H=zhuxi.front(),V=zhuxi.v;
    if(!MESSAGE::stupid)
    {vec R=vec(1,0);vec D=vec(0,1);
        double W1=(H&R)/(H^R);
        double W2=(R&H)/(H^R);
        double W3=(H&D)/(H^D);
        double W4=(D&H)/(H^D);
        double W5=(H&Rot(zhuxi.r,PI/2))/(H^Rot(zhuxi.r,PI/2));
        //if((W<-6)||((W>0)&&(W<0.5)))tr;if((W>6)||((W<0)&&(W>-0.5)))tl;
        if((V.x/(-X+MESSAGE::wScene/2)>0.7)||zhuxi.r.x>MESSAGE::wScene/2-50){
            if(W>W1)tl;if(W<W1)tr;
            if(H.x>0.5)f1;
            if(H.x<-0.5)f2;}
        if(V.x/(-X-MESSAGE::wScene/2)>0.7||zhuxi.r.x<-MESSAGE::wScene/2+50){
            if(W>W2)tl;if(W<W2)tr;
            if(H.x<-0.5)f1;if(H.x>0.5) f2;}
        if(V.y/(-Y+MESSAGE::hScene/2)>0.7||zhuxi.r.y>MESSAGE::hScene/2-50){
 if(W>W3)tl;if(W<W3)tr;
             if(H.y>0.5)f1;if(H.y<-0.5)f2;}
        if(V.y/(-Y-MESSAGE::hScene/2)>0.7||zhuxi.r.y<-MESSAGE::hScene/2+50){
 if(W>W4)tl;if(W<W4)tr;
            if(H.y<-0.5)f1;if(H.y>0.8)f2;}
        else if(zhuxi.calcL()<48000){
            if(W>W5)tl;if(W<W5)tr;
            if((H^V)>0)f2;
            if((H^V)<0)f1;
    }
    }

    Ship *S;int flag=0;
    for(int i=1;i<=MESSAGE::N-((MESSAGE::aimode==0)?0:MESSAGE::computers);i++)if(i!=id)if(ALIVE::Alive[i])
    {
        S=ALIVE::allships[i];
        if(R2(S->zhuxi.r-zhuxi.r)<400*400)
        {
            flag=1;
            if(S->toi==0)
            {
                vec J=(S->zhuxi.r-zhuxi.r)*Invabs(S->zhuxi.r-zhuxi.r);
                double W0=(H&J)/(H^J);
                /*if((J^H)>0){if((J&H)>0) tl;else tr;}
                if((J^H)<0){if((J&H)>0) tr;else tl;}*/
                if(W>W0)tl;if(W<W0)tr;
            }
        }
        if(R2(S->zhuxi.r-zhuxi.r)<310*300+700*zhuxi.n)if(S->toi<2)
        {
        vec J=(S->zhuxi.r-zhuxi.r)*Invabs(S->zhuxi.r-zhuxi.r);

            double U=((S->zhuxi.v-zhuxi.v)^(Rot(J,PI/2)));
            vec W1=((H*(BulletSpeed))+(Rot(H,PI/2))*(wShip/(2*Invabs(zhuxi.w))));
            vec W2=((H*(BulletSpeed)*(-1))+(Rot(H,PI/2))*(wShip/(2*Invabs(zhuxi.w))));


            //if((J^H)>0&&(((D-)*Invabs(D-zhuxi.v))^(H*Invabs(H)))>0.8)f1;
            //if((J^H)<0&&(((D-zhuxi.v/(Invabs(D)*400))*Invabs(D-zhuxi.v))^(H*Invabs(H)))<-0.8)f2;
            if((J^H)>0&&abs(R2((W1^Rot(J,PI/2))-U))<400)f1;
            if((J^H)<0&&abs(R2((W2^Rot(J,PI/2))-U))<400)f2;
        }
        if(R2(S->zhuxi.r-zhuxi.r)<220*220)if(S->toi==0)
        {   vec J=(S->zhuxi.r-zhuxi.r)*Invabs(S->zhuxi.r-zhuxi.r);

            if((J^H)>0&&(J^H)>0.8)f1;
            if((J^H)<0&&(J^H)<-0.8)f2;
        }
        if(R2(S->zhuxi.r-zhuxi.r)<120*150)if(S->toi==0)if(zhuxi.hp<=2)if(ALIVE::total>2)
        {destroy();return;}
    }
    if(!flag){if((W<-1)||((W>0)&&(W<0.5)))tr;if((W>1)||((W<0)&&(W>-0.5)))tl;
              //if(zhuxi.calcL()<78000){if((H^zhuxi.v)>0)f2;else f1; }
    }
#undef f1
#undef f2
#undef tl
#undef tr
}

MyRect0::MyRect0():tot(0)
{
    t1=new QTimer(),t2=new QTimer();
    connect(t1,SIGNAL(timeout()),this,SLOT(move()));
    connect(t2,SIGNAL(timeout()),this,SLOT(checktime()));
    t1->start(interval);t2->start(checkinterval);
    MANAGER::manager=new BulletManager();TRACEMANAGER::tracemanager=new TracepointManager();
    setBrush(QBrush(Qt::yellow,Qt::SolidPattern));
    MyGame::gametime=0;
    //setParent(MESSAGE::vie);
}
MyRect0::~MyRect0(){delete(t1);delete(t2);delete(MANAGER::manager);delete(ALIVE::allships);}
void MyRect0::keyPressEvent(QKeyEvent *event)
{
    if(MESSAGE::finished)return;
    int key_p=event->key();
    switch(key_p)
    {
    case Qt::Key_Escape:
        MyGame::ongoing=0;
        if(MESSAGE::esctosave){finishgame();saveit();}
        delete(MESSAGE::vie);                                               break;
    case Qt::Key_QuoteLeft:
        delete(MESSAGE::vie);                                               break;
    case Qt::Key_Minus:
        //if(MESSAGE::nextN>2)MESSAGE::nextN--;
        MESSAGE::rate*=0.8;                                                 break;
    case Qt::Key_Equal:
        //MESSAGE::nextN++;
        MESSAGE::rate*=1.25;                                                break;
    case Qt::Key_F2:
        MESSAGE::largescreen^=1;                                            break;
    case Qt::Key_F3:
        MESSAGE::isonesec^=1;                                               break;
    case Qt::Key_F1:
        MESSAGE::stupid^=1;                                                 break;
    case Qt::Key_F4:
        MESSAGE::showtrace^=1;                                              break;
    case Qt::Key_Space:
        if(MESSAGE::paused){t1->start();t2->start();}else{t1->stop();t2->stop();}MESSAGE::paused^=1;    break;
    default:
        for(int i=1;i<=tot;i++)if(ALIVE::Alive[ids[i]])manualships[i]->MykeyPressEvent(key_p);
    }
}
void MyRect0::addship(Ship *newship){manualships[++tot]=newship;ids[tot]=newship->id;}
void MyRect0::move()
{
    if(MESSAGE::finished)finishgame();
    Ship *S;
    for(int t=0;t<(MyGame::gametime>150*1000?2:1)*splits;t++)
    {
        for(int i=1;i<=MESSAGE::N;i++)if(ALIVE::Alive[i])
        {
            S=ALIVE::allships[i];
            S->move(splits);
        }
        for(int i=1;i<=MESSAGE::N;i++)if(ALIVE::Alive[i])
        {
            S=ALIVE::allships[i];
            S->bouncedetect();
        }
        for(int i=1;i<=MESSAGE::N;i++)if(ALIVE::Alive[i])
        {
            S=ALIVE::allships[i];
            if(S->isai)S->aithink();
        }
        MANAGER::manager->move();
    }
    MyGame::gametime+=interval;
    setFocus();
}
void MyRect0::checktime()
{
    Ship *S;
    for(int i=1;i<=MESSAGE::N;i++)if(ALIVE::Alive[i])
    {
        S=ALIVE::allships[i];
        S->taf+=checkinterval;S->tas+=checkinterval;
        S->toi-=checkinterval;if(S->toi<0)S->toi=0;
        S->tal+=checkinterval;if(S->tal>=ROL)if(S->zhuxi.n<maxBullets){S->tal=0;S->zhuxi.n++;S->statechange();}
    }
}
void MyRect0::finishgame()
{
    if(!MESSAGE::paused){t1->stop();t2->stop();}
    QFont font("Hebrew",10,QFont::Bold);
    font.setBold(true);
    (new MyRectButton(0,0,200,100,FINISHONE,Special::actor,"Next Round",font))->addtoscene(scene());
}
void MyRect0::saveit()
{
    QFile *sfile=(new MyFile())->savegame();Ship *S;Bullet *B;BulletList *Bl;
    if(sfile)
    {
        QTextStream stream(sfile);
        stream<<MESSAGE::roundnow->players<<' '<<MESSAGE::roundnow->games<<' '<<MESSAGE::roundnow->gamesleft<<' '<<MESSAGE::computers<<endl;
        for(int i=1;i<=MESSAGE::N;i++)stream<<MESSAGE::scores[i]<<' ';stream<<endl;
        stream<<ALIVE::total<<endl;
        for(int i=1;i<=MESSAGE::N;i++)if(ALIVE::Alive[i])
        {
            S=ALIVE::allships[i];
            stream<<S->id<<' ';
            stream<<S->zhuxi.r.x<<' '<<S->zhuxi.r.y<<' ';
            stream<<S->zhuxi.v.x<<' '<<S->zhuxi.v.y<<' ';
            stream<<S->zhuxi.t<<' '<<S->zhuxi.w<<' ';
            stream<<S->zhuxi.hp<<' '<<S->zhuxi.n<<' ';
            stream<<S->taf<<' '<<S->tal<<' '<<S->tas<<' '<<S->toi<<endl;
        }
        int nb=MANAGER::manager->numofbullets();stream<<nb<<endl;
        for(Bl=MANAGER::manager->root->next;Bl;Bl=Bl->next)
        {
            B=Bl->bullet;
            stream<<B->owner<<' '<<B->ha.hp<<' ';
            stream<<B->ha.r.x<<' '<<B->ha.r.y<<' ';
            stream<<B->ha.v.x<<' '<<B->ha.v.y<<endl;
        }
        sfile->close();
        delete(sfile);
    }
}

void testppp::MysetPos(vec r){setPos(r.x,r.y);}
testppp::testppp():QGraphicsRectItem(){}
