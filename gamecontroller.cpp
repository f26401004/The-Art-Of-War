#include "gamecontroller.h"
#include <iostream>

GameController::GameController(QWidget *parent) :
    scene_title(new SceneTitle),
    scene_choose(new SceneChoose(this)),
    scene_battle(new SceneBattle(this)),
    scene_result(new SceneResult),
    scene_card(new SceneCard),
    scene_draw(new SceneDraw),
    view(new QGraphicsView(scene_title)),
    timer(new QTimer),
    sessionTimer(new QTimer)
{

    // read the data from local file.
    read_data("./achievement.dat");
    read_data("./equip_card.dat");
    read_data("./unequip_card.dat");

    // connect to refresh the scene and frame count
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    connect(timer, SIGNAL(timeout()), scene_title, SLOT(refresh()));
    scene_title->reset();
    //connect(timer, SIGNAL(timeout()), scene_choose, SLOT(refresh()));
    timer->start(1000 / frame_rate);
    // connect to window to quit the program.
    connect(scene_title, SIGNAL(quit_signal()), parent, SLOT(close()));
    // connect to transfer to scene choose
    connect(scene_title, SIGNAL(transfer_to_choose()), this, SLOT(transfer_to_choose()));
    connect(scene_choose, SIGNAL(transfer_to_title()), this, SLOT(transfer_to_title()));
    connect(scene_choose, SIGNAL(transfer_to_battle()), this, SLOT(transfer_to_battle()));
    connect(scene_choose, SIGNAL(transfer_to_card()), this, SLOT(transfer_to_card()));
    connect(scene_choose, SIGNAL(transfer_to_draw()), this, SLOT(transfer_to_draw()));
    connect(scene_card, SIGNAL(transfer_to_choose()), this, SLOT(transfer_to_choose()));
    connect(scene_draw, SIGNAL(transfer_to_choose()), this, SLOT(transfer_to_choose()));
    connect(scene_battle, SIGNAL(transfer_to_result()), this, SLOT(transfer_to_result()));
    connect(scene_result, SIGNAL(transfer_to_title()), this, SLOT(transfer_to_title()));
    //connect(scene_choose, SIGNAL(transfer_to_title()), this, SLOT(return_the_card()));
    // set the view environment.
    view->setStyleSheet("background: transparent;border:0px");
    view->setWindowFlags(Qt::FramelessWindowHint);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->show();
}

void GameController::update()
{
    ++frame_count;
    frame_count %= (frame_rate + 1);
    if (frame_count == 0)
        frame_count = 1;
    // update scene frame count
    scene_title->frame_count = frame_count;
    scene_card->frame_count = frame_count;
    scene_draw->frame_count = frame_count;
    scene_choose->frame_count = frame_count;
    scene_battle->frame_count = frame_count;
    scene_result->frame_count = frame_count;
}

void GameController::transfer_to_title()
{
    scene_title->reset();
    view->setScene(scene_title);
    connect(timer, SIGNAL(timeout()),
                   scene_title,  SLOT(refresh()));
    // diconnect other scene to refresh
    disconnect(timer, SIGNAL(timeout()),
               scene_choose,  SLOT(refresh()));
    disconnect(timer, SIGNAL(timeout()),
               scene_result,  SLOT(refresh()));
    // place the card back.
    scene_choose->return_the_card();
}


void GameController::transfer_to_choose()
{
    equip_card_id.clear();
    unequip_card_id.clear();
    // read the data
    read_data("./achievement.dat");
    // reset the data of scene choose
    scene_choose->reset(point);
    view->setScene(scene_choose);
    connect(timer, SIGNAL(timeout()),
                   scene_choose,  SLOT(refresh()));
    // disconnect other scene to refresh
    disconnect(timer, SIGNAL(timeout()),
               scene_title,  SLOT(refresh()));
    disconnect(timer, SIGNAL(timeout()),
               scene_card,  SLOT(refresh()));
    disconnect(timer, SIGNAL(timeout()),
               scene_draw,  SLOT(refresh()));
}

void GameController::transfer_to_card()
{
    // reset the data of scene choose
    qDebug()<<"going to transfer to card";
    unequip_card_id.clear(); equip_card_id.clear();
    read_data("./unequip_card.dat");    read_data("./equip_card.dat");
    scene_card->reset(equip_card_id, unequip_card_id);
    view->setScene(scene_card);
    connect(timer, SIGNAL(timeout()),
                   scene_card,  SLOT(refresh()));
    // disconnect other scene to refresh
    disconnect(timer, SIGNAL(timeout()),
               scene_choose,  SLOT(refresh()));
}

void GameController::transfer_to_draw()
{
    qDebug() << point;
    // reset the data of scene choose
    scene_draw->reset(point);
    view->setScene(scene_draw);
    connect(timer, SIGNAL(timeout()),
                   scene_draw,  SLOT(refresh()));
    // disconnect other scene to refresh
    disconnect(timer, SIGNAL(timeout()),
               scene_choose,  SLOT(refresh()));
}

void GameController::transfer_to_battle()
{
    if(mode == "AI"){
        emit isAI();
        connect(timer, SIGNAL(timeout()), scene_battle, SLOT(refresh()));
    }
    else if(mode == "p2p"){
        if(modeChar == "host"){
            while(!tcpServer->hasPendingConnections());
            clientConnection = tcpServer->nextPendingConnection();
            connect(clientConnection, SIGNAL(readyRead()), scene_battle, SLOT(readCommand()));
        }
        else{
            tcpSocket->connectToHost(address,port);
            connect(tcpSocket, SIGNAL(readyRead()), scene_battle, SLOT(readCommand()));
        }
        connect(sessionTimer, SIGNAL(timeout()), scene_battle, SLOT(startSession()));
        sessionTimer->start(100);
        emit isp2p();
    }
    else{
        qDebug()<<"error in GameController::transfer_to_battle, can't find mode";
        return ;
    }

    equip_card_id.clear();
    unequip_card_id.clear();
    // read the data
    read_data("./equip_card.dat");
    read_data("./unequip_card.dat");
    // set the map and AI level
    scene_battle->set_map_and_level(0, scene_choose->level_index);
    scene_battle->set_card(equip_card_id);
    // reset the data of scene battle
    scene_battle->reset();
    // set the counter
    scene_battle->battle_start();
    view->setScene(scene_battle);
    connect(timer, SIGNAL(timeout()),
                   scene_battle,  SLOT(refresh()));
    // disconnect other scene to refresh
    disconnect(timer, SIGNAL(timeout()),
               scene_choose,  SLOT(refresh()));
}

void GameController::transfer_to_result()
{
    //reset the data

    if (mode == "p2p")
    {
        if(modeChar == "host"){
            tcpServer->close();
            clientConnection->disconnectFromHost();
        }
        else tcpSocket->abort();
        disconnect(sessionTimer, SIGNAL(timeout()), scene_battle, SLOT(startSession()));
    }

    qDebug() << scene_battle->get_interval();
    // gain the data
    scene_result->set_damage_rate(scene_battle->get_rate(0), scene_battle->get_rate(1));
    scene_result->set_interval(scene_battle->get_interval());
    // read file
    scene_result->readFile();
    // output the result to file
    scene_result->output_level();
    view->setScene(scene_result);
    connect(timer, SIGNAL(timeout()),
                   scene_result,  SLOT(refresh()));
    // disconnect other scene to refresh
    disconnect(timer, SIGNAL(timeout()),
               scene_battle,  SLOT(refresh()));
}

/*
void GameController::transfer(int type)
{
    std::cout << "press success!!" << std::endl;
    switch(type)
    {
        // transfer to scene title.
        case 1:
            view->setScene(scene_title);
            break;
        // transfer to scene choose.
        case 2:
            view->setScene(scene_choose);
            break;
        // transfer to scene battle.
        case 3:
            view->setScene(scene_battle);
            break;
        // transfer to scene result.
        case 4:
            view->setScene(scene_result);
            break;
    }
}
*/

void GameController::pause()
{
    disconnect(timer, SIGNAL(timeout()),
               this,  SLOT(advance()));
    disconnect(timer, SIGNAL(timeout()),
               scene_title,  SLOT(advance()));
}

void GameController::resume()
{
    connect(timer, SIGNAL(timeout()),
            this,  SLOT(advance()));
    connect(timer, SIGNAL(timeout()),
            scene_title,  SLOT(advance()));
}

void GameController::read_data(QString fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<file.errorString();
        qDebug()<<"file cannot open for reading";
        return ;
    }
    QByteArray byte = QByteArray::fromHex(file.readAll());
    std::stringstream ss(byte.toStdString());
    std::string a, b;

    if(fileName == "./achievement.dat"){
        while(ss>>a>>b){
            qDebug()<<QString::fromStdString(a)<<" "<<QString::fromStdString(b);
            if(a == "point"){
                point=atoi(b.c_str());
            }
            else if(a == "score"){
                score=atoi(b.c_str());
            }
            else if(a == "player"){
                player_name=QString::fromStdString(b);
            }
        }
    }
    else if(fileName == "./equip_card.dat"){
        while(ss>>a){
            qDebug()<<QString::fromStdString(a);
            equip_card_id.push_back(atoi(a.c_str()));
        }
        qDebug()<<"equip_card_id size="<<equip_card_id.size();
    }
    else if(fileName == "./unequip_card.dat"){
        while(ss>>a){
            qDebug()<<QString::fromStdString(a);
            unequip_card_id.push_back(atoi(a.c_str()));
        }
        qDebug()<<"unequip_card_id size="<<unequip_card_id.size();
    }
    file.close();
}


GameController::~GameController()
{
    delete scene_title;
    delete scene_choose;
    delete scene_battle;
    delete scene_result;
    delete view;
    delete timer;
}
