#include "scenechoose.h"
#include <iostream>

SceneChoose::SceneChoose(GameController *parent) : QGraphicsScene::QGraphicsScene(), gc(parent), inputStatus(0), checkTimer(new QTimer)
{
    // initialize the scene size
    setSceneRect(0, 0, 1280, 720);
    // set the background pic
    QImage bg;
    bg.load(":/Graphics/Scene_Choose/background.png");
    setBackgroundBrush(QBrush(bg));
    for (int i = 0 ; i < 4 ; ++i)
    {
        FlashCommand *sprite = new FlashCommand(0, 0, QPixmap(":/Graphics/Scene_Choose/command_" + QString::number(i) + ".png"));
        sprite->setZValue(990);
        sprite->setOffset((-1) * sprite->pixmap().width() / 2, (-1) * sprite->pixmap().height() / 2);
        sprite->setOpacity(0.1);
        command.push_back(sprite);
        if ((i + 1) % 2 == 0)
            sprite->setY(500);
        else
            sprite->setY(300);
        if (i / 2 > 0)
            sprite->setX(1130 + (2 - i) * 200 + 150);
        else
            sprite->setX(150 + i*200 - 150);
        addItem(sprite);
    }
    // initialize the light point
    srand((unsigned)time(NULL));
    for (int i = 0 ; i < 8 ; ++i)
    {
        QGraphicsPixmapItem *light_point = new QGraphicsPixmapItem(QPixmap(":/Graphics/light_point.png"));
        light_point->setPos(rand() % 1280, rand() % 720);
        light_point->setScale(sin(rand() % 6) + 1.2);
        light_point->setRotation(rand() % 360);
        light_point->setOpacity(sin(rand() % 6));
        light_point->setOffset((-1)*light_point->pixmap().width() / 2, (-1)*light_point->pixmap().height() / 2);
        light.push_back(light_point);
        addItem(light_point);
    }
    // initialize the level command
    level_command.push_back(new FlashCommand(359, 154, QPixmap(":/Graphics/EasyLevel.png")));
    level_command.push_back(new FlashCommand(359, 304, QPixmap(":/Graphics/HardLevel.png")));
    level_command.push_back(new FlashCommand(359, 454, QPixmap(":/Graphics/NightmareLevel.png")));
    for (int i = 0 ; i < 3 ; ++i)
    {
        level_command.at(i)->setZValue(999);
        level_command.at(i)->setOpacity(0);
        addItem(level_command.at(i));
    }
    // initialize the connection command
    connection_back = new QGraphicsPixmapItem(QPixmap(":/Graphics/Scene_Choose/connection_back.png"));
    connection_back->setPos(314, 76-100);
    connection_back->setZValue(999);
    connection_back->setOpacity(0);
    black_rect = new QGraphicsRectItem(0, 0, 1280, 720);
    black_rect->setBrush(QBrush(QColor(0, 0, 0)));
    black_rect->setZValue(998);
    black_rect->setOpacity(0);
    host_command = new FlashCommand(436, 215, QPixmap(":/Graphics/Scene_Choose/connection_host.png"));
    host_command->setOpacity(0);
    host_command->setZValue(1000);
    client_command = new FlashCommand(436, 380, QPixmap(":/Graphics/Scene_Choose/connection_client.png"));
    client_command->setOpacity(0);
    client_command->setZValue(1000);
    addItem(connection_back);
    addItem(black_rect);
    addItem(host_command);
    addItem(client_command);


    /*
     * use client_IP and client_port to enter the data
     * use host_data->setPlainText() to show the data
     * line 432 and 437 display the client_IP and client_port or host_data
     */

    client_IP = new QLabel();
    client_port = new QLabel();
    host_data = new QGraphicsTextItem();
    host_data->setPlainText("");
    host_data->setScale(2);
    host_data->setOpacity(0);
    host_data->setPos(430, 220);
    host_data->setZValue(1000);
    host_data->setDefaultTextColor(QColor(255, 255, 255));
    //client_IP->setFrameStyle(QFrame::Sunken | QFrame::Panel);
    client_IP->resize(160, 32);
    client_IP->setGeometry(560, 277, 160, 32);
    client_IP->setFocus();
    //client_IP->hide();
    //client_port->setFrameStyle(QFrame::Sunken | QFrame::Panel);
    client_port->resize(160, 32);
    client_port->setGeometry(560, 412, 160, 32);
    proxy_IP = addWidget(client_IP);
    proxy_port = addWidget(client_port);
    proxy_IP->setZValue(1000);
    proxy_IP->setOpacity(0);
    proxy_port->setZValue(1000);
    proxy_port->setOpacity(0);
    addItem(proxy_IP);
    addItem(proxy_port);
    addItem(host_data);

    // initialize point window
    point_window = new QGraphicsPixmapItem(QPixmap(":/Graphics/Scene_Choose/point_window.png"));
    point_window->setPos(-160, 10);
    point_window->setOpacity(0);
    addItem(point_window);
    // initialize point text
    point_text = new QGraphicsTextItem("戰役成就 " + QString::number(point) + " pt");
    point_text->setFont(QFont("Bookman", 24));
    point_text->setDefaultTextColor(QColor(255, 255, 255));
    point_text->setPos(-50, 30);
    point_text->setOpacity(0);
    addItem(point_text);

    // initialize the transition
    transition = new GraphicsTransition(0, 0, 1280, 720);
    addItem(transition);
}

void SceneChoose::refresh()
{
    // set the transtion update
    transition->update();
    // background light point animation
    background_animation();
    if (choose_phase)
        command_in_animation();
    command_out_animation();
    command_active_animation();
    if (transition->opacity() == 1)
    {
        if (choose_phase)
        {
            switch(command_index)
            {
            case 0:
                emit transfer_to_card();
                break;
            case 1:
                emit transfer_to_draw();
                break;
            case 2:
                emit transfer_to_battle();
                break;
            case 3:
                gc->mode="AI";
                qDebug()<<gc->mode;
                emit transfer_to_battle();
                break;
            }
        }
        else
        {
            emit transfer_to_title();
        }
    }
}

void SceneChoose::return_the_card()
{
}

void SceneChoose::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->lastScenePos() != event->scenePos() && !second_stage)
    {
        if (event->scenePos().x() > 19 && event->scenePos().x() < 250)
            if (event->scenePos().y() > 167 && event->scenePos().y() < 433)
            {
                command_index = 0;
                if (last_index != command_index)
                {
                    // play the se.
                    QSound::play(":/Audio/SE/move.wav");
                    last_index = command_index;
                }
            }
        if (event->scenePos().x() > 250 && event->scenePos().x() < 481)
            if (event->scenePos().y() > 367 && event->scenePos().y() < 633)
            {
                command_index = 1;
                if (last_index != command_index)
                {
                    // play the se.
                    QSound::play(":/Audio/SE/move.wav");
                    last_index = command_index;
                }
            }
        if (event->scenePos().x() > 1055 && event->scenePos().x() < 1230)
            if (event->scenePos().y() > 167 && event->scenePos().y() < 433)
            {
                command_index = 2;
                if (last_index != command_index)
                {
                    // play the se.
                    QSound::play(":/Audio/SE/move.wav");
                    last_index = command_index;
                }
            }
        if (event->scenePos().x() > 849 && event->scenePos().x() < 1055)
            if (event->scenePos().y() > 367 && event->scenePos().y() < 633)
            {
                command_index = 3;
                if (last_index != command_index)
                {
                    // play the se.
                    QSound::play(":/Audio/SE/move.wav");
                    last_index = command_index;
                }
            }
    }
    else if (event->lastScenePos() != event->scenePos() && second_stage && command_index == 2)
    {

    }
    else if (event->lastScenePos() != event->scenePos() && second_stage && command_index == 3)
    {

    }
}

void SceneChoose::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->scenePos().x() > 19 && event->scenePos().x() < 250)
        if (event->scenePos().y() > 167 && event->scenePos().y() < 433)
        {
            // play the se.
            QSound::play(":/Audio/SE/enter.wav");
            // start transition
            transition->start_out();
        }
    if (event->scenePos().x() > 250 && event->scenePos().x() < 481)
        if (event->scenePos().y() > 367 && event->scenePos().y() < 633)
        {
            // play the se.
            QSound::play(":/Audio/SE/enter.wav");
            // start transition
            transition->start_out();
        }
    if (event->scenePos().x() > 1055 && event->scenePos().x() < 1230)
        if (event->scenePos().y() > 167 && event->scenePos().y() < 433)
        {
            // play the se.
            QSound::play(":/Audio/SE/enter.wav");
            // start connection choose
            second_stage = true;
        }
}

void SceneChoose::keyPressEvent(QKeyEvent *event)
{
    if (!second_stage)
    {
        switch(event->key())
        {
            case Qt::Key_Escape:
                // play the se.
                QSound::play(":/Audio/SE/cancel.wav");
                // set the transformation
                transition->start_out();
                choose_phase = false;
                break;
            case Qt::Key_Right:
                // play the se.
                QSound::play(":/Audio/SE/move.wav");
                command_index = (command_index + 1) % 4;
                last_index = command_index;
                break;
            case Qt::Key_Left:
                // play the se.
                QSound::play(":/Audio/SE/move.wav");
                command_index = (command_index - 1 > -1 ? command_index - 1 : 3);
                last_index = command_index;
                break;
            case Qt::Key_Enter:
            case Qt::Key_Return:
                switch(command_index)
                {
                    case 0:
                        // play the se.
                        QSound::play(":/Audio/SE/enter.wav");
                        // set the transformation
                        transition->start_out();
                        break;
                    case 1:
                        // play the se.
                        QSound::play(":/Audio/SE/enter.wav");
                        // set the transformation
                        transition->start_out();
                        break;
                    case 2:
                        // play the se.
                        QSound::play(":/Audio/SE/enter.wav");
                        second_stage = true;
                        break;
                    case 3:
                        // play the se.
                        QSound::play(":/Audio/SE/enter.wav");
                        second_stage = true;
                        break;
                }
                break;
        }
    }
    else
    {
        if (!show_host_stage && !enter_client_stage)
        {
            switch(event->key())
            {
                case Qt::Key_Escape:
                    // play the se.
                    QSound::play(":/Audio/SE/cancel.wav");
                    // back to the command_choose
                    second_stage = false;
                    break;
                case Qt::Key_Up:
                    // play the se.
                    QSound::play(":/Audio/SE/move.wav");
                    if (command_index == 2)
                        connection_index = 1 - connection_index;
                    else if (command_index == 3)
                        level_index = level_index - 1 > -1 ? level_index - 1 : 2;
                    break;
                case Qt::Key_Down:
                    // play the se.
                    QSound::play(":/Audio/SE/move.wav");
                    if (command_index == 2)
                        connection_index = 1 - connection_index;
                    else if (command_index == 3)
                        level_index = (level_index + 1) % 3;
                    break;
                case Qt::Key_Enter :
                case Qt::Key_Return :
                    // play the se.
                    QSound::play(":/Audio/SE/enter.wav");
                    if (command_index == 2)
                    {
                        if (connection_index == 0)
                            show_host_stage = true;
                        else
                            enter_client_stage = true;
                    }
                    else if (command_index == 3)
                        transition->start_out();
                    break;
            }
        }
        else
        {
            switch(event->key())
            {
                case Qt::Key_Escape:
                    // play the se.
                    QSound::play(":/Audio/SE/cancel.wav");
                    // back to the command_choose
                    show_host_stage = false;
                    enter_client_stage = false;
                    break;
                case Qt::Key_Enter : case Qt::Key_X:
                    qDebug()<<gc->tcpSocket->state();
                    if(gc->tcpSocket->state() == QTcpSocket::UnconnectedState){
                        hostIp = client_IP->text();
                        hostPort = client_port->text().toInt();
                        qDebug()<<"hostIp = "<<hostIp<<" host port = "<<hostPort;
                        gc->address = hostIp;
                        gc->port = hostPort;
                        gc->tcpSocket->connectToHost(hostIp, hostPort);
                        connect(checkTimer, SIGNAL(timeout()), this, SLOT(check_connect()));
                        checkTimer->start(10);
                    }
                    break;
                case Qt::Key_Backspace :
                    if(inputStatus==0){
                        client_IP->setText(client_IP->text().remove(client_IP->text().length()-1,1));
                    }
                    else{
                        client_port->setText(client_port->text().remove(client_port->text().length()-1,1));
                    }
                    break;
                case Qt::Key_Tab :
                    inputStatus=!inputStatus;
                    break;
                case Qt::Key_Period :
                    client_IP->setText(client_IP->text() + ".");
                    break;
            }

            for(int i=0; i<=9; ++i){
                if(inputStatus==0){
                    if(event->key() == i+48){
                        client_IP->setText(client_IP->text() + QString::number(i));
                        break;
                    }
                }
                else{
                    if(event->key() == i+48){
                        client_port->setText(client_port->text() + QString::number(i));
                        break;
                    }
                }
            }
        }
    }
}

void SceneChoose::command_in_animation()
{
    if (command.at(0)->x() < 150)
    {
        command.at(0)->moveBy(15, 0);
        command.at(0)->setOpacity(command.at(0)->opacity() + 0.1);
    }
    if (command.at(1)->x() < 350)
    {
        command.at(1)->moveBy(15, 0);
        command.at(1)->setOpacity(command.at(1)->opacity() + 0.1);
    }
    if (command.at(2)->x() > 1130)
    {
        command.at(2)->moveBy(-15, 0);
        command.at(2)->setOpacity(command.at(2)->opacity() + 0.1);
    }
    if (command.at(3)->x() > 930)
    {
        command.at(3)->moveBy(-15, 0);
        command.at(3)->setOpacity(command.at(3)->opacity() + 0.1);
    }
    if (point_window->x() < -10)
    {
        point_window->moveBy(15, 0);
        point_window->setOpacity(point_window->opacity() + 0.1);
        point_text->moveBy(15, 0);
        point_text->setOpacity(point_text->opacity() + 0.1);
    }
    // refresh the second stage animation
    if (second_stage && command_index == 2)
    {
        if (!show_host_stage && !enter_client_stage)
        {
            proxy_IP->setOpacity(proxy_IP->opacity() - 0.1);
            proxy_port->setOpacity(proxy_port->opacity() - 0.1);
            host_data->setOpacity(host_data->opacity() - 0.1);
            if (connection_back->y() < 76)
            {
                black_rect->setOpacity(black_rect->opacity() + 0.05);
                connection_back->setOpacity(connection_back->opacity() + 0.1);
                connection_back->moveBy(0, 10);
            }
            if (connection_back->opacity() > 0.5)
            {
                host_command->setOpacity(host_command->opacity() + 0.1);
                client_command->setOpacity(host_command->opacity());
            }
        }
    }
    else if (second_stage && command_index == 3)
    {
        if (black_rect->opacity() < 0.5)
        {
            black_rect->setOpacity(black_rect->opacity() + 0.05);
            for (int i = 0 ; i < 3 ; ++i)
                level_command.at(i)->setOpacity(level_command.at(i)->opacity() + 0.1);
        }
    }
}

void SceneChoose::command_out_animation()
{
    if (!choose_phase)
    {
        if (command.at(0)->x() > 0)
        {
            command.at(0)->moveBy(-15, 0);
            command.at(0)->setOpacity(command.at(0)->opacity() - 0.1);
        }
        if (command.at(1)->x() > 200)
        {
            command.at(1)->moveBy(-15, 0);
            command.at(1)->setOpacity(command.at(1)->opacity() - 0.1);
        }
        if (command.at(2)->x() < 1280)
        {
            command.at(2)->moveBy(15, 0);
            command.at(2)->setOpacity(command.at(2)->opacity() - 0.1);
        }
        if (command.at(3)->x() < 1080)
        {
            command.at(3)->moveBy(15, 0);
            command.at(3)->setOpacity(command.at(3)->opacity() - 0.1);
        }
        if (point_window->x() > -410)
        {
            point_window->moveBy(-15, 0);
            point_window->setOpacity(point_window->opacity() - 0.1);
            point_text->moveBy(-15, 0);
            point_text->setOpacity(point_text->opacity() - 0.1);
        }
    }
    else
    {
        // refresh the second stage animation
        if ((!second_stage && command_index == 2) || (second_stage && command_index == 2 && (show_host_stage || enter_client_stage)))
        {
            //if (show_host_stage || enter_client_stage)
            if (host_command->opacity() > 0)
            {
                host_command->setOpacity(host_command->opacity() - 0.1);
                client_command->setOpacity(host_command->opacity());
                /*
                 * important : here diaplsy the client_IP or client_port or host_data!!
                 * you can write another function call here to set the data or something else.
                 * when enter_client_stage is on , then display the client_IP and client_port through QGraphicsProxyWidget
                 * when show_host_stage is on , then display the host_data through QGraphicsProxyWidget
                 */
                if (enter_client_stage)
                {
                    gc->modeChar="client";
                    gc->tcpSocket = new QTcpSocket();
                    proxy_IP->setOpacity(proxy_IP->opacity() + 0.1);
                    proxy_port->setOpacity(proxy_port->opacity() + 0.1);
                }
                else if (show_host_stage)
                {
                    gc->modeChar="host";
                    host_data->setOpacity(host_data->opacity() + 0.1);
                }
            }
            if (host_command->opacity() < 0.5 && connection_back->y() > -24 && (!show_host_stage && !enter_client_stage) )
            {
                black_rect->setOpacity(black_rect->opacity() - 0.05);
                connection_back->setOpacity(connection_back->opacity() - 0.1);
                connection_back->moveBy(0, -10);
            }
        }
        else if (!second_stage && command_index == 3)
        {
            if (black_rect->opacity() > 0)
            {
                black_rect->setOpacity(black_rect->opacity() - 0.05);
                for (int i = 0 ; i < 3 ; ++i)
                    level_command.at(i)->setOpacity(level_command.at(i)->opacity() - 0.1);
            }
        }
    }
}

void SceneChoose::command_active_animation()
{
    if (!second_stage)
    {
        command.at(command_index)->refresh(frame_count);
        foreach (FlashCommand *iter, command)
        {
            if (iter != command.at(command_index))
                iter->flash->setStrength(0);
        }
    }
    else if (second_stage && command_index == 2)
    {
        if (!connection_index)
        {
            host_command->refresh(frame_count);
            client_command->flash->setStrength(0);
        }
        else
        {
            host_command->flash->setStrength(0);
            client_command->refresh(frame_count);
        }
    }
    else if (second_stage && command_index == 3)
    {
        level_command.at(level_index)->refresh(frame_count);
        level_command.at((level_index + 1)%3)->flash->setStrength(0);
        level_command.at(level_index-1 > -1 ? level_index-1 : 2)->flash->setStrength(0);
    }
}

void SceneChoose::background_animation()
{
    foreach(QGraphicsPixmapItem* iter, light)
    {
        iter->setScale(iter->scale() + 0.01);
        iter->setRotation(iter->rotation() + 0.02);
        iter->setOpacity(iter->opacity() - 0.01);
        // reset the point if the point opacity <= 0
        if (iter->opacity() <= 0)
        {
            iter->setPos(rand() % 1280, rand() % 720);
            iter->setScale(sin(rand() % 6) + 1.2);
            iter->setRotation(rand() % 360);
            iter->setOpacity(sin(rand() % 6));
        }
    }
}

void SceneChoose::findHostInfo()
{
    host_data_text.clear();
    gc->tcpServer = new QTcpServer(this);
    if(!gc->tcpServer->listen()){
        qDebug()<<"tcpServer: failed to listen (in function SceneChoose::findHostInfo)";
        return ;
    }

    QList<QHostAddress> ipList = QNetworkInterface::allAddresses();
    host_data_text = "IP : \n";
    foreach(QHostAddress ip, ipList){
        if(ip.toIPv4Address()){
            host_data_text.append(ip.toString() + '\n');
        }
    }
    host_data_text.append("\n");
    host_data_text.append(QString("port : %1\n").arg(gc->tcpServer->serverPort()));
    host_data->setPlainText(host_data_text);
    connect(gc->tcpServer, SIGNAL(newConnection()), this, SLOT(startBattle()));
}

void SceneChoose::startBattle()
{
    if(gc->modeChar == "host"){
        qDebug()<<"host part connected";
        qDebug()<<"pendingConnection?"<<gc->tcpServer->hasPendingConnections();
        gc->clientConnection = gc->tcpServer->nextPendingConnection();
        disconnect(gc->tcpServer, SIGNAL(newConnection()), this, SLOT(startBattle()));
        gc->clientConnection->disconnectFromHost();
        gc->mode="p2p";
        //emit transfer_to_battle();
        transition->start_out();
    }
    else{

    }
}

void SceneChoose::check_connect()
{
    if(gc->tcpSocket->state() == QTcpSocket::ConnectedState){
        gc->mode="p2p";
        gc->modeChar="client";
        disconnect(checkTimer, SIGNAL(timeout()), this, SLOT(check_connect()));
        transition->start_out();
    }
}
void SceneChoose::reset(int value)
{
    point = value;
    point_text->setPlainText("戰役成就 " + QString::number(point) + " pt");

    second_stage = false;
    show_host_stage = false;
    enter_client_stage = false;

    client_IP->clear();
    client_port->clear();
    host_data_text.clear();
    // catch ip,port information
    findHostInfo();

    command_index = 0;
    last_index = 0;
    connection_index = 0;
    choose_phase = true;
    second_stage = false;

    connection_back->setOpacity(0);
    black_rect->setOpacity(0);
    host_command->setOpacity(0);
    client_command->setOpacity(0);
    proxy_IP->setOpacity(0);
    proxy_port->setOpacity(0);
    host_data->setOpacity(0);

    foreach(FlashCommand*iter, level_command)
    {
        iter->setOpacity(0);
    }
    transition->start_in();
    // set the background pic
    QImage bg;
    bg.load(":/Graphics/Scene_Choose/background.png");
    setBackgroundBrush(QBrush(bg));
}

SceneChoose::~SceneChoose()
{
    foreach (QGraphicsPixmapItem* iter, light)
        delete iter;
    light.clear();
}
