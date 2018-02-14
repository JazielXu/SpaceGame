#ifndef MAINWINDOW
#define MAINWINDOW
#include <QMainWindow>
#include <QMenu>
#include <QDebug>
#include <QObject>
//已经废弃

class MyMainWindow:public QMainWindow
{
public:
    explicit MyMainWindow();
    //~MyMainWindow();
private slots:
    //void startnewgame();
signals:

};

#endif // MAINWINDOW

