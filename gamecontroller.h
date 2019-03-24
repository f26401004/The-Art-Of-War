#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QObject>
#include <QTimer>
#include <QTcpServer>
#include <QTcpSocket>
#include <QByteArray>
#include <QDataStream>
#include <sstream>
#include <string>
#include "scenetitle.h"
#include "scenechoose.h"
#include "scenebattle.h"
#include "sceneresult.h"
#include "scenecard.h"
#include "scenedraw.h"

class SceneChoose;
class SceneBattle;
class GameController : public QObject
{
    Q_OBJECT
public:
    GameController(QWidget*);
    ~GameController();
    SceneTitle *scene_title;
    SceneChoose *scene_choose;
    SceneBattle *scene_battle;
    SceneResult *scene_result;
    SceneCard *scene_card;
    SceneDraw *scene_draw;
    QGraphicsView *view;
    //QGraphicsView *choose_view;
    //QGraphicsView *battle_view;
    //QGraphicsView *result_view;
    QTimer *timer;
    void pause();
    void resume();
    int frame_count = 0;
    void setScene(QGraphicsScene*);

    // will receive from scenechoose's client part
    QTcpSocket *tcpSocket;
    QString address;
    int port;
    // will receive from scenechoose's host part
    QTcpServer *tcpServer;
    QTcpSocket *clientConnection;
    QString modeChar;//host or client
    QString mode;//AI or p2p
    QTimer *sessionTimer;

signals:
    void isAI();
    void isp2p();

private:
    const int frame_rate = 40;
    QList<int> equip_card_id;
    QList<int> unequip_card_id;
    int point;
    int score;
    QString player_name;
    int game_time;

    void read_data(QString fileName);

private slots:
    void update();
    void transfer_to_title();
    void transfer_to_choose();
    void transfer_to_battle();
    void transfer_to_result();
    void transfer_to_card();
    void transfer_to_draw();
};

#endif // GAMECONTROLLER_H
