#ifndef SETTINGUI_H
#define SETTINGUI_H
#include <QWidget>
#include <QGraphicsRectItem>
/*定义了用于设置界面的UI，详见界面文件*/

namespace Ui {
class settingui;
}

class settingui : public QWidget
{
    Q_OBJECT

public:
    int a,b;

    explicit settingui(QWidget *parent = 0);
    ~settingui();

    void keyPressEvent(QKeyEvent *event);
    void setthekey(int key);

private slots:
    void on_spinBox_valueChanged(int arg1);

    void on_spinBox_3_valueChanged(int arg1);


    void on_spinBox_2_valueChanged(int arg1);

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();



    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_checkBox_stateChanged(int arg1);

    void on_checkBox_2_stateChanged(int arg1);

    void on_checkBox_3_stateChanged(int arg1);

    void on_pushButton_6_clicked();

    void on_checkBox_4_stateChanged(int arg1);

private:
    Ui::settingui *ui;
};




#endif // SETTINGUI_H
