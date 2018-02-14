#ifndef GAMECONTROL
#define GAMECONTROL
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QObject>
#include "spaceship.h"
#include "Ship.h"
#include "Message.h"
#include "Button.h"
/*定义了用于控制游戏流程的类，以及开始界面*/

class MyGame:public QApplication
{
public:
    static int ongoing;
    static int gametime;
    MyGame(int argc, char *argv[]);
};//必须的QApplication

class GameHolder:public QObject{
Q_OBJECT
public:
    int players,games,gamesleft;
    GameHolder(int p=MESSAGE::nextN,int g=-1);
    GameHolder(const GameHolder& h);
    ~GameHolder();
    void checknext();
public slots:
    void one_ended();
};//一轮n局游戏的控制类

class GameOnce:public QObject{
Q_OBJECT
public:
    QGraphicsScene *scene;
    QGraphicsView *view;
    GameHolder *holder;
    MyRect0 *controller;
    int players;
    GameOnce(int p=MESSAGE::nextN,GameHolder *h=NULL);
    ~GameOnce();
    int init();
    void playit();
public slots:
    void ended();
};//一局游戏的控制类

class Gamecenter:public QObject{
Q_OBJECT
public:
    int gameon;
    MyGame *app;
    Gamecenter(MyGame *a);
    void loadfromfile();
    void startnewgame();
    void exitwhole();
};//整个游戏的控制类，控制开始新游戏和读档，只有一个实例

class SpecialScene:public QGraphicsView
{
public:
    QGraphicsScene *scene;
    int w,h;
    SpecialScene(int w0,int h0);
    ~SpecialScene();
};//界面类

class StartScene:public SpecialScene
{
public:
    MyRectButton *sta,*loa,*set,*qui;
    StartScene(int w0=800,int h0=1000);
};//开始界面

class SettingScene:public SpecialScene
{
public:
    MyRectButton *pup,*pdo,*pnu;
    SettingScene(int w0=600,int h0=600);
    ~SettingScene();
    void hidefordel();
};//设置界面，已废弃

class ActionHolder:public QObject{
Q_OBJECT
public:
    int startsceneavailable();
    int a;
    ActionHolder();
    ~ActionHolder();
    void connectact(MyRectButton *but,actype action);
public slots:
    void newgame();
    void tosett();
    void quit();
    void onemore();
    void oneless();
    void confirm();
    void finishone();
    void loadgame();
};//动作类，用于执行按钮动作，全局只有一个实例

#endif // GAMECONTROL

