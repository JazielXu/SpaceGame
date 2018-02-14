#include "File.h"
#define tr QObject::tr

MyFile::MyFile(QString openfile):name(openfile){}
QFile* MyFile::selectload()
{
    QString fname=QFileDialog::getOpenFileName(0,tr("Select savefile"),tr("."),tr("Save Files(*.sav)"));
    QFile *sfile=new QFile(fname);
    if(!sfile->open(QIODevice::Text|QIODevice::ReadOnly)){QMessageBox::warning(NULL,tr("Warning"),tr("Can't open"),QMessageBox::Yes);return NULL;}
    return sfile;
}
QFile* MyFile::savegame()
{
    QString fname=QFileDialog::getSaveFileName(0,tr("Select savefile"),tr("."),tr("Save Files(*.sav)"));
    QFile *sfile=new QFile(fname);
    if(!sfile->open(QIODevice::Text|QIODevice::WriteOnly)){QMessageBox::warning(NULL,tr("Warning"),tr("Can't open"),QMessageBox::Yes);return NULL;}
    return sfile;
}
QFile* MyFile::savesett()
{
    QFile *sfile=new QFile("settings.ini");
    if(!sfile->open(QIODevice::Text|QIODevice::WriteOnly)){QMessageBox::warning(NULL,tr("Warning"),tr("Can't save settings"),QMessageBox::Yes);return NULL;}
    return sfile;
}
QFile* MyFile::loadsett()
{
    QFile *sfile=new QFile("settings.ini");
    if(!sfile->open(QIODevice::Text|QIODevice::ReadOnly)){QMessageBox::warning(NULL,tr("Warning"),tr("Can't load settings"),QMessageBox::Yes);return NULL;}
    return sfile;
}
