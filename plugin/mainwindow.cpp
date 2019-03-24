#include "mainwindow.h"
#include "ui_mainwindow.h"

mainwindow::mainwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mainwindow),
    achievement(new QPushButton(this)),
    minionData(new QPushButton(this)),
    back(new QPushButton(this)),
    confirmA(new QPushButton(this)),
    confirmM(new QPushButton(this)),
    minionBox(new QComboBox(this)),
    achievementPoint(new QLineEdit(this))
{
    setGeometry(size_x,size_y,size_w,size_h1);
    ui->setupUi(this);
    for(int i=0; i<12; ++i){
        QLabel *tmp = new QLabel(this);
        QLineEdit *tmp2 = new QLineEdit(this);
        tmp2->hide();
        labelList.push_back(tmp);
        editList.push_back(tmp2);
    }

    back->setText("Back");
    back->setStyleSheet("Font: 13pt");
    confirmA->setText("Confirm");
    confirmA->setStyleSheet("Font: 13pt");
    confirmM->setText("Confirm");
    confirmM->setStyleSheet("Font: 13pt");

    connect(achievement, SIGNAL(clicked(bool)), this, SLOT(achievementMode()));
    connect(minionData, SIGNAL(clicked(bool)), this, SLOT(minionDataMode()));
    connect(confirmA, SIGNAL(clicked(bool)), this, SLOT(modifyPoint()));
    connect(minionBox, SIGNAL(currentIndexChanged(int)), this, SLOT(showDetail(int)));
    connect(confirmM, SIGNAL(clicked(bool)), this, SLOT(modifyMinion()));

    toStart();

}

mainwindow::~mainwindow()
{
    delete ui;
}

void mainwindow::achievementMode()
{
    setGeometry(size_x,size_y,size_w,size_h2);
    disconnect(back);
    connect(back, SIGNAL(clicked(bool)), this, SLOT(toStart()));
    achievement->hide();
    minionData->hide();
    clearLabel();
    back->show(); confirmA->show();
    achievementPoint->show();

    labelList[1]->setGeometry(20,20,250,40);
    labelList[1]->setStyleSheet("Font: 13pt");
    labelList[1]->setText(QString("original achievement point :"));
    labelList[2]->setGeometry(20,60,250,40);
    labelList[2]->setStyleSheet("Font: 13pt");
    labelList[2]->setText(QString("modify achievement"));
    achievementPoint->setGeometry(155,65,80,30);
    achievementPoint->setStyleSheet("Font: 13pt");
    achievementPoint->setText(QString("modify achievement"));
    achievementPoint->setFocus();
    confirmA->setGeometry(250,65,135,30);
    back->setGeometry(250,25,135,30);
    //setGeometry(x(),y(),width(), size_h-150);

    QFile file("./achievement.dat");
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"achievement.dat open failed";
        return;
    }
    QByteArray byte = QByteArray::fromHex(file.readAll());
    std::stringstream ss(byte.toStdString());
    std::string name,point;
    ss>>name>>point;
    labelList[1]->setText(QString("original achievement point : %1").arg(QString::fromStdString(point)));
    achievementPoint->setText(QString::fromStdString(point));
    file.close();


}

void mainwindow::minionDataMode()
{
    setGeometry(size_x,size_y,size_w,size_h1);
    clearLabel();
    disconnect(back);
    connect(back, SIGNAL(clicked(bool)), this, SLOT(toStart()));
    achievement->hide();
    minionData->hide();
    minionBox->show(); confirmM->show(); labelList[1]->show(); back->show();
    minionBox->setGeometry(60,50,270,30);
    minionBox->addItems(minionList);
    minionBox->setStyleSheet("Font: 15pt");
    confirmM->setGeometry(120,90,65,30);
    back->setGeometry(200,90,65,30);
    labelList[0]->setText("Select the minon you want to modfiy : ");
    labelList[0]->setGeometry(60,15,270,25);
    labelList[0]->setStyleSheet("Font: 13pt");

    showDetail(minionBox->currentIndex());
}

void mainwindow::clearEdit()
{
    QList<QLineEdit*> list = findChildren<QLineEdit*>();
    foreach(QLineEdit *i, list){
        i->clear();
        i->hide();
    }
}

void mainwindow::clearLabel()
{
    QList<QLabel*> list = findChildren<QLabel*>();
    foreach(QLabel *i, list){
        i->clear();
    }
}

void mainwindow::toStart()
{
    setGeometry(size_x,size_y,size_w,size_h1);
    achievement->show();
    minionData->show();
    achievementPoint->hide();
    clearLabel();
    clearEdit();
    back->hide();
    confirmA->hide();
    confirmM->hide();
    minionBox->hide();

    achievement->setText("Achievement Point");
    achievement->setGeometry(60,30,270,90);
    achievement->setStyleSheet("Font: 20pt");
    minionData->setText("Minion Data");
    minionData->setGeometry(60,150,270,90);
    minionData->setStyleSheet("Font: 20pt");

    //setGeometry(x(),y(),width(), size_h);

}

void mainwindow::modifyPoint()
{
    QFile file("./achievement.dat");
    QDataStream out(&file);
    if(!file.open(QIODevice::WriteOnly)){
        qDebug()<<"can't open achievemenet.dat in mainwindow::modifyPoint";
        return ;
    }

    QString modified="point " + achievementPoint->text();
    achievementPoint->clear();
    achievementPoint->setFocus();
    QByteArray byte = modified.toLocal8Bit().toHex();
    out.writeBytes(byte.data(), byte.size());
    file.close();


    labelList[2]->setGeometry(35,80,250,40);
    labelList[2]->setStyleSheet("Font: 13pt");
    labelList[2]->setText(QString("\n  成功啦~~記得刷新畫面 ^^"));
    achievementMode();
}

void mainwindow::modifyMinion()
{
    QFile file("./minion.dat");
    if(!file.open(QIODevice::Truncate|QIODevice::WriteOnly)){
        qDebug()<<"cannot open minion.dat in mainwindow::modifyMinion";
        return ;
    }
    QString input;
    for(int i=0; i<minions.size(); ++i){
        if(i == minionBox->currentIndex() && editList.at(2)->text() != ""){
            for(int j=0; j<editList.size(); ++j)    input += " "+editList.at(j)->text();
        }
        else{
            for(int j=0; j<minions.at(i).size(); ++j) input += " "+minions.at(i).at(j);
        }
    }
    QDataStream in(&file);
    QByteArray byte = input.toLocal8Bit().toHex();
    in.writeBytes(byte.data(), byte.size());

    file.close();
    showDetail(minionBox->currentIndex());
}

void mainwindow::showDetail(int ind)
{
    setGeometry(size_x,size_y,size_w, size_h3);

    minionChoose = ind;
    foreach(QVector<QString> i, minions){
        i.clear();
    }
    minions.clear();

    QFile file("./minion.dat");
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"cannot open minion.dat in mainwindow::showDetail";
        return ;
    }

    QByteArray byte = QByteArray::fromHex(file.readAll());
    std::stringstream ss(byte.toStdString());
    std::string s;
    while(ss>>s){
        QVector<QString> vec;
        vec.push_back(QString::fromStdString(s));
        for(int i=0; i<10; ++i){
            ss>>s;
            vec.push_back(QString::fromStdString(s));
        }
        minions.push_back(vec);
    }
    for(int i=1; i<=11; ++i){
        labelList[i]->setStyleSheet("Font: 13pt");
        labelList[i]->setGeometry(10,100+25*i,200,20);
        if(i!=1)    editList[i]->show();
        editList[i]->setStyleSheet("Font: 13pt");
        editList[i]->setGeometry(190,101+25*i,120,20);
    }
    labelList[1]->setText(QString("Selected minion : %1").arg(minions.at(minionChoose).at(0)));     editList[1]->setText(minions.at(minionChoose).at(0));
    labelList[2]->setText(QString("Original max HP : %1").arg(minions.at(minionChoose).at(1)));     editList[2]->setText(minions.at(minionChoose).at(1));
    labelList[3]->setText(QString("Original HP : %1").arg(minions.at(minionChoose).at(2)));         editList[3]->setText(minions.at(minionChoose).at(2));
    labelList[4]->setText(QString("Original ATK : %1").arg(minions.at(minionChoose).at(3)));        editList[4]->setText(minions.at(minionChoose).at(3));
    labelList[5]->setText(QString("Original STR : %1").arg(minions.at(minionChoose).at(4)));        editList[5]->setText(minions.at(minionChoose).at(4));
    labelList[6]->setText(QString("Original DEF : %1").arg(minions.at(minionChoose).at(5)));        editList[6]->setText(minions.at(minionChoose).at(5));
    labelList[7]->setText(QString("Original SPD : %1").arg(minions.at(minionChoose).at(6)));        editList[7]->setText(minions.at(minionChoose).at(6));
    labelList[8]->setText(QString("Original DEX : %1").arg(minions.at(minionChoose).at(7)));        editList[8]->setText(minions.at(minionChoose).at(7));
    labelList[9]->setText(QString("Original AGI : %1").arg(minions.at(minionChoose).at(8)));        editList[9]->setText(minions.at(minionChoose).at(8));
    labelList[10]->setText(QString("Original range : %1").arg(minions.at(minionChoose).at(9)));     editList[10]->setText(minions.at(minionChoose).at(9));
    labelList[11]->setText(QString("Original cost mana: %1").arg(minions.at(minionChoose).at(10))); editList[11]->setText(minions.at(minionChoose).at(10));
    file.close();
}
