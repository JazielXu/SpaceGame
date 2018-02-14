#include <QMenuBar>
#include <QDesktopWidget>
#include <QDebug>
#include <QObject>
#include "mainwindow.h"
#include "Gamecontrol.h"
#define tr QObject::tr

MyMainWindow::MyMainWindow():QMainWindow()
{
    Gamecenter *center=new Gamecenter(MESSAGE::app);
    MESSAGE::cen=center;
    Special::actor=new ActionHolder();

    QMenu *game=new QMenu("&Game");                     this->menuBar()->addMenu(game);
    QMenu *sett=new QMenu("&Settings");                 this->menuBar()->addMenu(sett);

    QAction *game_start=game->addAction("&Start");      game_start->setShortcut(Qt::Key_F10);
    QAction *game_exit =game->addAction("&Exit");       game_exit ->setShortcut(Qt::Key_Escape);

    //connect(game_start,SIGNAL(triggered()),center,SLOT(startnewgame()));
    //connect(game_exit ,SIGNAL(triggered()),center,SLOT(exitwhole()));

    QDesktopWidget *dw=QApplication::desktop();
    QRect cR=dw->availableGeometry(),aR=dw->screenGeometry();
    setGeometry(cR.left()+cR.width()/2,cR.top()+cR.height()/2,200,0);
    MESSAGE::wcom=cR.width();MESSAGE::hcom=cR.height();
}


//MyMainWindow::~MyMainWindow(){}
