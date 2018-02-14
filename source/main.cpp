#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QDebug>
#include "Ship.h"
#include "Bullet.h"
#include "spaceship.h"
#include "Gamecontrol.h"
#include "Trace.h"
#include "mainwindow.h"
#include "File.h"
#include "settingui.h"

QGraphicsScene* MESSAGE::sce=NULL;GameOnce* MESSAGE::gamenow=NULL;QGraphicsView* MESSAGE::vie=NULL;MyGame* MESSAGE::app=NULL;Gamecenter* MESSAGE::cen=NULL;GameHolder* MESSAGE::roundnow=NULL;
vec MESSAGE::npush=vec();
int MESSAGE::N=2,MESSAGE::nextN=2,MESSAGE::turns=1,MESSAGE::computers=0;
int MESSAGE::largescreen=0,MESSAGE::isonesec=1,MESSAGE::stupid=0,MESSAGE::showtrace=0,MESSAGE::paused=0,MESSAGE::esctosave=0,MESSAGE::loading=0;
int MESSAGE::hScene=hScene0,MESSAGE::wScene=wScene0,MESSAGE::wDisplay=wScene0,MESSAGE::hDisplay=hScene0;
int MESSAGE::wcom=0,MESSAGE::hcom=0;
double MESSAGE::ratio=1,MESSAGE::rate=1;
int MESSAGE::keys[MAXN][6]={{0,0,0,0,0,0},{0,16777235,16777237,16777234,16777236,47},{0,87,83,65,68,69},{0,73,75,74,76,79},{0,84,71,70,72,89}};
int MESSAGE::scores[MAXN]={0},MESSAGE::finished=0;
int MESSAGE::aimode=0;

int MyGame::ongoing=1,MyGame::gametime=0,ALIVE::Alive[MAXN]={0},ALIVE::total=MESSAGE::N;Ship** ALIVE::allships;
BulletManager* MANAGER::manager;//=new BulletManager();
TracepointManager* TRACEMANAGER::tracemanager;

int Special::onsetting=0;SettingScene* Special::sett=NULL;MyRectButton* Special::n_pl=NULL;ActionHolder* Special::actor=NULL;
//QSound* Special::bgm=NULL;

int main(int argc, char *argv[])
{
    MyGame a(argc, argv);
    MESSAGE::app=&a;

    MyMainWindow *mainWin=new MyMainWindow();

    delete(new settingui());

    MESSAGE::setScreen();

    new StartScene();

    return a.exec();
}
