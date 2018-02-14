#include "Gamecontrol.h"
#include "Ship.h"
#include "Bullet.h"
#include "spaceship.h"
#include "Button.h"
#include "Trace.h"
#include "settingui.h"
#include "File.h"
#include <QGraphicsView>
#include <QMessageBox>
#include <QDebug>
#include <QTimer>

MyGame::MyGame(int argc,char *argv[]):QApplication(argc,argv){MyGame::ongoing=1;}

GameOnce::GameOnce(int p,GameHolder *h):players(p),holder(h)
{
    MESSAGE::gamenow=this;
    MESSAGE::finished=0;
    init();playit();
}
int GameOnce::init()
{
    MESSAGE::N=players;
    MESSAGE::setScreen(MESSAGE::rate);
    for(int i=0;i<MAXN;i++)ALIVE::Alive[i]=0;ALIVE::total=players;//qDebug()<<MyGame::ongoing;
    //MESSAGE::gamenow=app;

    MESSAGE::sce=scene=new QGraphicsScene();
    scene->setSceneRect(-MESSAGE::wDisplay/2,-MESSAGE::hDisplay/2,MESSAGE::wDisplay,MESSAGE::hDisplay);

    QPixmap bg(":/new/im/bg.jpg");
    QGraphicsPixmapItem *pix=new QGraphicsPixmapItem(bg.scaledToHeight(MESSAGE::hDisplay+20));
    pix->setPos(-pix->boundingRect().width()/2,-pix->boundingRect().height()/2-10);
    pix->setOpacity(.2);
    scene->addItem(pix);

    MyRect0 *cent=new MyRect0;  cent->setRect(-1,-1,2,2);   scene->addItem(cent);
    cent->setFlag(QGraphicsItem::ItemIsFocusable);
    cent->setFocus();
    controller=cent;
    cent->t1->stop();cent->t2->stop();

    int MX=MESSAGE::wScene/2*MESSAGE::ratio,MY=MESSAGE::hScene/2*MESSAGE::ratio;
    QGraphicsRectItem *boundary=new QGraphicsRectItem;
    boundary->setRect(-MX,-MY,MX*2,MY*2);scene->addItem(boundary);
    boundary->setPen(QPen(Qt::red,3,Qt::SolidLine));

    double NR=nearR*MESSAGE::ratio;
    QGraphicsEllipseItem *inner=new QGraphicsEllipseItem;
    inner->setRect(-NR,-NR,2*NR,2*NR);scene->addItem(inner);
    inner->setPen(QPen(Qt::red,3,Qt::SolidLine));
    inner->setBrush(QBrush(QColor(255,0,0,120),Qt::SolidPattern));

    QGraphicsEllipseItem *innerc=new QGraphicsEllipseItem;
    innerc->setRect(-NR/2,-NR/2,NR,NR);scene->addItem(innerc);
    innerc->setPen(QPen(Qt::transparent,3,Qt::SolidLine));
    innerc->setBrush(QBrush(QColor(255,255,0,120),Qt::SolidPattern));
    //QGraphicsRectItem *ppp=new QGraphicsRectItem;ppp->setRect(0,0,40,40);ppp->setPos(0,0);scene->addItem(ppp);

    Ship **ship=(Ship**)malloc(MAXN*sizeof(Ship*)),*ship2;//qDebug()<<(int)ship[5];
    ALIVE::allships=ship;
    vec X0=vec(0,startR),V0=vec(-ShipSpeed,0);
    if(!MESSAGE::loading){
    for(int i=1;i<=players;i++)
    {
        ship2  =new Ship(Rot(X0,2*i*PI/players),Rot(V0,2*i*PI/players),2*i*PI/players,1,color2[i],color1[i]);            scene->addItem(ship2);
        ship[i]=new Ship(Rot(X0,2*i*PI/players),Rot(V0,2*i*PI/players),2*i*PI/players,1,color1[i],color2[i],ship2);      scene->addItem(ship[i]);
        ALIVE::Alive[ship[i]->id=i]=1;
        cent->addship(ship[i]);
        ship[i]->txt.setText(i);scene->addItem(&(ship[i]->txt));
        ship[i]->setkeyMap(i*1000+1,i*1000+2,i*1000+3,i*1000+4,i*1000+5);
        ship[i]->toi=players*1000;
        ship[i]->addstate();
        ship[i]->isai=(i+MESSAGE::computers<=players)?0:1;
        //if(i>4)ship[i]->zhuxi.w=2.5*PI;
    }}
    else{
    MESSAGE::loading=0;
    QFile *sfile=(new MyFile)->selectload();
    int gameleft,id,tf,tl,ts,ti,nn,nb;
    double rx,ry,vx,vy,hp,ww,th;
    if(sfile)
    {
        QTextStream stream(sfile);
        stream>>holder->players>>holder->games>>holder->gamesleft>>MESSAGE::computers;players=holder->players;
        //for(int i=1;i<=players;i++){ship[i]->zhuxi.hp=-999;ship[i]->destroy();ALIVE::Alive[i]=0;}
        for(int i=1;i<=MESSAGE::N;i++)stream>>MESSAGE::scores[i];
        stream>>ALIVE::total;
        for(int i=1;i<=ALIVE::total;i++)
        {
            stream>>id;
            stream>>rx>>ry>>vx>>vy>>th>>ww>>hp>>nn>>tf>>tl>>ts>>ti;
            ship2   =new Ship(vec(rx,ry),vec(vx,vy),th,ww,color2[id],color1[id],NULL);          scene->addItem(ship2);
            ship[id]=new Ship(vec(rx,ry),vec(vx,vy),th,ww,color1[id],color2[id],ship2);         scene->addItem(ship[id]);
            ship[id]->zhuxi.hp=hp;ship[id]->zhuxi.n=nn;
            ship[id]->taf=tf;ship[id]->tal=tl;ship[id]->tas=ts;ship[id]->toi=ti;
            ALIVE::Alive[ship[id]->id=id]=1;
            cent->addship(ship[id]);
            ship[id]->txt.setText(id);scene->addItem(&(ship[id]->txt));
            ship[id]->setkeyMap(id*1000+1,id*1000+2,id*1000+3,id*1000+4,id*1000+5);
            ship[id]->addstate();
            ship[id]->isai=(id+MESSAGE::computers<=players)?0:1;
        }
        stream>>nb;
        Bullet *B;
        for(int i=1;i<=nb;i++)
        {
            stream>>id>>hp>>rx>>ry>>vx>>vy;
            B=new Bullet(vec(rx,ry),vec(vx,vy),wBullet,hBullet,id);
            B->ha.hp=hp;
            scene->addItem(B);
            MANAGER::manager->add(B);
        }
        sfile->close();
        delete(sfile);
    }else return 1;}

    for(int i=1;i<=players;i++)if(MESSAGE::keys[i][1])if(ALIVE::Alive[i])
    {
        ship[i]->setkeyMap(MESSAGE::keys[i][1],MESSAGE::keys[i][2],MESSAGE::keys[i][3],MESSAGE::keys[i][4],MESSAGE::keys[i][5]);
    }

    cent->t1->start();cent->t2->start();

    return 0;
}
GameOnce::~GameOnce()
{
    delete(controller);//delete(scene);//delete(view);qDebug()<<2;holder->checknext();
}
void GameOnce::playit()
{
    MESSAGE::vie=view=new QGraphicsView(scene);
    view->showFullScreen();
    view->setStyleSheet("background:transparent");
    view->show();

    view->setAttribute(Qt::WA_DeleteOnClose);
    connect(view,SIGNAL(destroyed()),this,SLOT(ended()));
}
void GameOnce::ended(){delete(this);}

GameHolder::GameHolder(int p,int g):players(p),games(g),gamesleft(g)
{
    MESSAGE::roundnow=this;
    for(int i=0;i<MAXN;i++)MESSAGE::scores[i]=0;
}
GameHolder::GameHolder(const GameHolder& h):players(h.players),games(h.games),gamesleft(h.gamesleft){}
GameHolder::~GameHolder(){MESSAGE::cen->gameon=0;}
void GameHolder::checknext()
{//qDebug()<<games;
    if(MyGame::ongoing==0){delete(this);return;}
    if(gamesleft--!=0)connect(new GameOnce(players,this),SIGNAL(destroyed()),this,SLOT(one_ended()));
    else
    {
        QString final("Scores:\n");
        for(int i=1;i<=players;i++)final=final+"  Player"+QString::number(i)+":  "+QString::number(MESSAGE::scores[i])+"    \n";
        final=final+"\nPlay Again?";
        QMessageBox::StandardButton reply=QMessageBox::warning(0,"Gameover",final,QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
        if(reply==QMessageBox::Yes)(new GameHolder(players,games))->checknext();
        delete(this);
    }
}
void GameHolder::one_ended(){checknext();}

Gamecenter::Gamecenter(MyGame *a):gameon(0),app(a){}
void Gamecenter::startnewgame()
{
    MyGame::ongoing=1;gameon=1;
    (new GameHolder(MESSAGE::nextN,MESSAGE::turns))->checknext();
}
void Gamecenter::exitwhole(){app->exit();}
void Gamecenter::loadfromfile(){MESSAGE::loading=1;startnewgame();}

SpecialScene::SpecialScene(int w0,int h0):QGraphicsView(scene=new QGraphicsScene()),w(w0),h(h0)
{
    scene->setSceneRect(-w/2*MESSAGE::ratio,-h/2*MESSAGE::ratio,w*MESSAGE::ratio,h*MESSAGE::ratio);

    setWindowFlags(Qt::FramelessWindowHint);
    resize(scene->width()+20,scene->height()+20);
    show();
}
SpecialScene::~SpecialScene(){delete(scene);}

StartScene::StartScene(int w0,int h0):SpecialScene(w0,h0)
{
    Special::onsetting=0;

    scene->setBackgroundBrush(QBrush(Qt::blue,Qt::SolidPattern));

    QFont font("Hebrew",10,QFont::Bold);
    font.setPixelSize(w/10*MESSAGE::ratio);
    font.setBold(true);
    (sta=new MyRectButton(0,-h*0.3,w/2,h/6,NEWGAME ,Special::actor,"Start",   font))->addtoscene(scene);
    (loa=new MyRectButton(0,-h*0.1,w/2,h/6,LOADGAME,Special::actor,"Load",    font))->addtoscene(scene);
    (set=new MyRectButton(0,h*0.1, w/2,h/6,SETTINGS,Special::actor,"Settings",font))->addtoscene(scene);
    (qui=new MyRectButton(0,h*0.3, w/2,h/6,QUIT    ,Special::actor,"Quit",    font))->addtoscene(scene);
}

SettingScene::SettingScene(int w0,int h0):SpecialScene(w0,h0)
{
    Special::onsetting=1;
    Special::sett=this;

    QFont font("Hebrew",10,QFont::Bold);
    //font.setPixelSize(w/10);
    font.setBold(true);
    (pup=new MyRectButton(0,-h*0.3,w/2,h/4,ONEMORE,Special::actor,"+",font))->addtoscene(scene);
    (pdo=new MyRectButton(0,0,     w/2,h/4,CONFIRM,Special::actor,"=",font))->addtoscene(scene);
    (pnu=new MyRectButton(0,h*0.3, w/2,h/4,ONELESS,Special::actor,"-",font))->addtoscene(scene);
    Special::n_pl=pdo;pdo->txt->setText(MESSAGE::nextN);

    setAttribute(Qt::WA_DeleteOnClose);
}
SettingScene::~SettingScene(){delete(pup);delete(pdo);delete(pnu);Special::onsetting=0;}//qDebug()<<1;}
void SettingScene::hidefordel(){hide();delete(this);}

int ActionHolder::startsceneavailable(){return (MESSAGE::cen->gameon==0)&&(Special::onsetting==0);}
ActionHolder::ActionHolder():a(0){}
ActionHolder::~ActionHolder(){}
void ActionHolder::connectact(MyRectButton *but,actype action)
{
    switch(action)
    {
    case NEWGAME:
        connect(but,SIGNAL(clicked()),this,SLOT(newgame()));break;
    case SETTINGS:
        connect(but,SIGNAL(clicked()),this,SLOT(tosett()));break;
    case QUIT:
        connect(but,SIGNAL(clicked()),this,SLOT(quit()));break;
    case ONEMORE:
        connect(but,SIGNAL(clicked()),this,SLOT(onemore()));break;
    case ONELESS:
        connect(but,SIGNAL(clicked()),this,SLOT(oneless()));break;
    case CONFIRM:
        connect(but,SIGNAL(clicked()),this,SLOT(confirm()));break;
    case FINISHONE:
        connect(but,SIGNAL(clicked()),this,SLOT(finishone()));break;
    case LOADGAME:
        connect(but,SIGNAL(clicked()),this,SLOT(loadgame()));break;
    }
}
void ActionHolder::newgame(){if(startsceneavailable())MESSAGE::cen->startnewgame();}
void ActionHolder::tosett(){if(startsceneavailable()){QWidget *mysettingui=new settingui;mysettingui->show();}}//{if(startsceneavailable())new SettingScene();}
void ActionHolder::quit(){if(startsceneavailable())MESSAGE::cen->exitwhole();}
void ActionHolder::onemore(){MyRectButton *npl=Special::n_pl;npl->txt->setText(++MESSAGE::nextN);}
void ActionHolder::oneless(){MyRectButton *npl=Special::n_pl;if(MESSAGE::nextN>2)npl->txt->setText(--MESSAGE::nextN);}
void ActionHolder::confirm(){Special::sett->hidefordel();}
void ActionHolder::finishone(){delete(MESSAGE::vie);}
void ActionHolder::loadgame(){if(startsceneavailable())MESSAGE::cen->loadfromfile();}
