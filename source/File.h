#ifndef FILE_H
#define FILE_H
#include <QFile>
#include <QFileDialog>
#include <QObject>
#include <QTextStream>
#include <QMessageBox>
/*这里定义了支持打开文件操作的类*/

class MyFile
{
public:
    QString name;
    MyFile(QString openfile=QString());
    QFile *selectload();
    QFile *savegame();
    QFile *savesett();
    QFile *loadsett();
};//用于打开文件

#endif // FILE_H

