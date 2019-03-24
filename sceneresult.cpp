#include "sceneresult.h"
#include <QDebug>

SceneResult::SceneResult() : QGraphicsScene::QGraphicsScene(){
    // initialize the scene size
    setSceneRect(0, 0, 1280, 720);
    // set the background pic
    QImage bg;
    bg.load(":/Graphics/result_back.png");
    setBackgroundBrush(QBrush(bg));
    // initialize the content
    //content = new ResultContent();
    level_pic = new QGraphicsPixmapItem();
    level_pic->setOpacity(0);
    level_pic->setPos(420, 60);
    data = new QGraphicsTextItem();
    data->setFont(QFont("Bookman", 32));
    data->setDefaultTextColor(QColor(100, 14, 14, 255));
    data->setPos(218, 280);
    data->setOpacity(0);
    achievement_point = new QGraphicsTextItem();
    achievement_point->setFont(QFont("Bookman", 32));
    achievement_point->setDefaultTextColor(QColor(100, 14, 14, 255));
    achievement_point->setPos(1000, 610);
    achievement_point->setOpacity(0);
    ranking = new QGraphicsTextItem();
    ranking->setFont(QFont("Bookman", 32));
    ranking->setDefaultTextColor(QColor(100, 14, 14, 255));
    ranking->setPos(330, 610);
    ranking->setOpacity(0);
    addItem(level_pic);
    addItem(data);
    addItem(achievement_point);
    addItem(ranking);
    // initialize graphics transition
    transition = new GraphicsTransition(0, 0, 1280, 720);
    addItem(transition);
    //readFile();
}

void SceneResult::refresh()
{
    // update the transition
    transition->update();
    switch(paint_stage)
    {
        case 0:
            level_pic->setOpacity(sin(static_cast<float>(frame_count) / 80 * 3.14159));
            if (level_pic->opacity() >= 0.95)
                ++paint_stage;
            break;
        case 1:
            data->setOpacity(sin(static_cast<float>(frame_count) / 80 * 3.14159));
            if (data->opacity() >= 0.95)
                ++paint_stage;
            break;
        case 2:
            achievement_point->setOpacity(sin(static_cast<float>(frame_count) / 80 * 3.14159));
            if (achievement_point->opacity() >= 0.95)
                ++paint_stage;
            break;
        case 3:
            ranking->setOpacity(sin(static_cast<float>(frame_count) / 80 * 3.14159));
            if (ranking->opacity() >= 0.95)
                ++paint_stage;
            break;
        case 4:
            break;
    }
    if (transition->opacity() == 1)
        emit transfer_to_title();
}

void SceneResult::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::RightButton || event->button() == Qt::LeftButton)
    {
        // play the se.
        QSound::play(":/Audio/SE/enter.wav");
        //emit display_command();
        transition->start_out();
    }
}

void SceneResult::keyPressEvent(QKeyEvent *event)
{
    // if press enter button or escape button
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return || event->key() == Qt::Key_Escape)
    {
        // play the se.
        QSound::play(":/Audio/SE/enter.wav");
        //emit display_command();
        transition->start_out();
    }
}


void SceneResult::set_damage_rate(double e_value, double p_value)
{
    enemy_damage_rate = e_value;
    player_damage_rate = p_value;
}

void SceneResult::set_interval(int value)
{
    interval = value;
}

void SceneResult::output_level()
{
    transition->start_in();
    // compute the score
    int score = 100 + static_cast<double>(interval) / 50000000;
    score = score - player_damage_rate / 10 + enemy_damage_rate / 10;
    if (score <= 100)
        level = 0;
    else if (score > 110)
        level = 1;
    else if (score > 120)
        level = 2;
    else if (score > 130)
        level = 3;
    else if (score > 140)
        level = 4;
    else if (score == 150)
        level = 5;
    // compute the achievement point.
    if (player_damage_rate > 0)
        point += score * 10 / static_cast<int>(player_damage_rate);
    else
        point += score * 10;

    readFile();
    writeFile();
    //textBrowser: name, score
    sendResult();

    // set the data to content and draw the data
    //content->set_content(level, interval, enemy_damage_rate, player_damage_rate, point);
    level_pic->setPixmap(QPixmap(":/Graphics/level_" + QString::number(level)));
    QString text = " 0" +
            QString::number((300 - interval) / 60) +
            ":" +
            ((300 - interval) % 60 > 9 ? QString::number(((300 - interval) % 60)) : "0" + QString::number((300 - interval) % 60)) +
            "\n" +
            "                " + QString::number(enemy_damage_rate) + "\n" +
            "                " + QString::number(player_damage_rate);
    data->setPlainText(text);
    achievement_point->setPlainText(QString("  "+QString::number(point)));
}

void SceneResult::writeFile()
{
    QFile wfile("./achievement.dat");
    QDataStream out(&wfile);
    if(!wfile.open(QIODevice::WriteOnly)){
        qDebug()<<"file cannot open for writing";
        return ;
    }
    QString data="point " + QString::number(point+last_point);
    QByteArray byte = data.toLocal8Bit().toHex();
    out.writeBytes(byte.data(),byte.size());
    wfile.close();
}

void SceneResult::readFile()
{
    QFile file("./achievement.dat");
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"file cannot open for reading";
        return ;
    }

    QByteArray byte = QByteArray::fromHex(file.readAll());
    std::stringstream ss(byte.toStdString());
    std::string a, b;

    //----------------------operate the data from "achievement.dat"--------------------
    //everything that reads in is a string, need to cast
    while(ss>>a){
        if(a=="point"){
            ss>>b;
            last_point=atoi(b.c_str());
        }
    }
    qDebug()<<last_point;

    file.close();
}

void SceneResult::sendResult()
{
    //name, score
    QString name = QHostInfo::localHostName();
    tcpSocket = new QTcpSocket(this);
    tcpSocket->connectToHost("140.116.116.60", 55555);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readResult()));
    ranking->setPlainText(" 未連線");

    QByteArray byte;
    QDataStream stream(&byte, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_7);
    stream<<name<<point;
    qDebug()<<"send : "<<name<<" "<<point;
    tcpSocket->write(byte);
}

void SceneResult::readResult()
{
    QDataStream in(tcpSocket);
    in.startTransaction();

    int myRank, allRank;
    in>>myRank>>allRank;

    ranking->setPlainText(QString(QString::number(myRank)+"/"+QString::number(allRank)));

    qDebug()<<myRank<<allRank;
}

SceneResult::~SceneResult()
{
    delete level_pic;
    delete data;
    delete achievement_point;
    delete ranking;
}
