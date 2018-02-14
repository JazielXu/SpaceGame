#include "settingui.h"
#include "ui_settingui.h"
#include "Message.h"
#include "QKeyEvent"
#include "File.h"
#include <QDebug>

int GameMode=0;
int RealPersonPlayers=2;
int ComputerPlayers=0;
int turnsofgame=2;
int lar=0,stu=0,tra=0,esv=0;
class MyRadioButton:public QRadioButton
{

};

settingui::settingui(QWidget *parent):a(0),b(0),
    QWidget(parent),
    ui(new Ui::settingui)
{
    ui->setupUi(this);
    this->grabKeyboard();
    this->setAttribute(Qt::WA_DeleteOnClose);

    QFile *sfile=(new MyFile())->loadsett();
    if(sfile)
    {
        QTextStream stream(sfile);
        stream>>turnsofgame>>RealPersonPlayers>>ComputerPlayers;
        stream>>lar>>stu>>tra>>esv;
        stream>>GameMode;
        for(int i=1;i<=9;i++)
        {
            for(int j=0;j<6;j++)stream>>MESSAGE::keys[i][j];
        }
        sfile->close();
        delete(sfile);
    }

    ui->checkBox->setChecked(lar);ui->checkBox_2->setChecked(stu);ui->checkBox_3->setChecked(tra);ui->checkBox_4->setChecked(esv);
    ui->spinBox->setValue(turnsofgame);ui->spinBox_2->setValue(RealPersonPlayers);ui->spinBox_3->setValue(ComputerPlayers);
    ui->radioButton->setChecked(GameMode==0);ui->radioButton_2->setChecked(GameMode==1);ui->radioButton_3->setChecked(GameMode==2);
    this->setthekey(0);
}

settingui::~settingui()
{
    delete ui;

    QFile *sfile=(new MyFile())->savesett();
    if(sfile)
    {
        //qDebug()<<1;
        QTextStream stream(sfile);
        stream<<turnsofgame<<' '<<RealPersonPlayers<<' '<<ComputerPlayers<<endl;
        stream<<lar<<' '<<stu<<' '<<tra<<' '<<esv<<endl;
        stream<<GameMode<<endl;
        for(int i=1;i<=9;i++)
        {
            for(int j=0;j<6;j++)stream<<MESSAGE::keys[i][j]<<' ';
            stream<<endl;
        }
        sfile->close();
        delete(sfile);
    }

    MESSAGE::largescreen=lar;MESSAGE::stupid=stu;MESSAGE::showtrace=tra;MESSAGE::esctosave=esv;
    MESSAGE::nextN=RealPersonPlayers+ComputerPlayers;MESSAGE::computers=ComputerPlayers;MESSAGE::turns=turnsofgame;
    MESSAGE::aimode=GameMode;
}




void settingui::on_spinBox_valueChanged(int arg1)
{
    if(arg1>=1)turnsofgame=arg1;
    else ui->spinBox->setValue(turnsofgame);
}


void settingui::on_spinBox_2_valueChanged(int arg1)
{
    if(arg1+ComputerPlayers>=1)RealPersonPlayers=arg1;
    else ui->spinBox_2->setValue(RealPersonPlayers);
}


void settingui::on_spinBox_3_valueChanged(int arg1)
{
    if(arg1+RealPersonPlayers>=1)ComputerPlayers=arg1;
}

void settingui::on_radioButton_clicked()
{
    GameMode=0;
}

void settingui::on_radioButton_2_clicked()
{
    GameMode=1;
}

void settingui::on_radioButton_3_clicked()
{
    GameMode=2;
}

void settingui::keyPressEvent(QKeyEvent *event){setthekey(event->key());}

void settingui::setthekey(int key)
{
    MESSAGE::keys[a+1][b]=key;
    ui->label_6->setText(QString::number(MESSAGE::keys[a+1][1]));
    ui->label_7->setText(QString::number(MESSAGE::keys[a+1][2]));
    ui->label_8->setText(QString::number(MESSAGE::keys[a+1][3]));
    ui->label_9->setText(QString::number(MESSAGE::keys[a+1][4]));
    ui->label_10->setText(QString::number(MESSAGE::keys[a+1][5]));
}

void settingui::on_pushButton_clicked()
{
    b=1;
}

void settingui::on_pushButton_2_clicked()
{
    b=2;
}

void settingui::on_pushButton_3_clicked()
{
    b=3;
}

void settingui::on_pushButton_4_clicked()
{
    b=4;
}

void settingui::on_pushButton_5_clicked()
{
    b=5;
}

void settingui::on_comboBox_currentIndexChanged(int index)
{
    a=index;
    this->setthekey(a);
}

void settingui::on_checkBox_stateChanged(int arg1)
{
    lar=arg1;
}

void settingui::on_checkBox_2_stateChanged(int arg1)
{
    stu=arg1;
}

void settingui::on_checkBox_3_stateChanged(int arg1)
{
    tra=arg1;
}

void settingui::on_pushButton_6_clicked()
{
    delete(this);
}

void settingui::on_checkBox_4_stateChanged(int arg1)
{
    esv=arg1;
}
