#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QDebug>
#include <QFile>
#include <QByteArray>
#include <sstream>
#include <string>
#include <Qlist>
#include <QDesktopWidget>

namespace Ui {
class mainwindow;
}

class mainwindow : public QDialog
{
    Q_OBJECT

public:
    explicit mainwindow(QWidget *parent = 0);
    ~mainwindow();

private:
    QStringList minionList={"Warrior","Rifleman","Nun","Assassin","Swordman","Knight","Archer","Witch","Poet","Druid","Laser_Blaster","Robot","UAV"};

    Ui::mainwindow *ui;
    int size_x=0,size_y=0,size_w=400;
    int size_h1=300;
    int size_h2=130;
    int size_h3=410;
    int minionChoose;
    QPushButton *achievement;
    QPushButton *minionData;
    QPushButton *back;
    QPushButton *confirmA;
    QPushButton *confirmM;
    QComboBox *minionBox;
    QLineEdit *achievementPoint;
    QList<QLabel*> labelList;
    QList<QLineEdit*> editList;
    QList< QVector<QString> > minions;


private slots:
    void achievementMode();
    void minionDataMode();
    void clearEdit();
    void clearLabel();
    void toStart();
    void modifyPoint();
    void modifyMinion();
    void showDetail(int);
};

#endif // MAINWINDOW_H
