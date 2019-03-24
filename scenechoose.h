#ifndef SCENECHOOSE_H
#define SCENECHOOSE_H

#include <QObject>
#include <QGraphicsScene>
#include <QPixmap>
#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QKeyEvent>
#include <QList>
#include <QSound>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QLabel>
#include <QGraphicsProxyWidget>
#include <QLineEdit>
#include <cmath>
#include <time.h>
#include <QDebug>
#include "flashcommand.h"
#include "graphicstransition.h"
#include "gamecontroller.h"

#include <QByteArray>
#include <QDataStream>
#include <QNetworkInterface>

class GameController;
class SceneChoose : public QGraphicsScene
{
    Q_OBJECT
public:
    SceneChoose(GameController *parent);
    ~SceneChoose();
    int frame_count;
    int command_index = 0;
    int connection_index = 0;
    int card_index = 0;
    int map_index = 0;
    int level_index = 0;
    int last_index = 0;
    void return_the_card();
    void reset(int);

signals:
    void transfer_to_title();
    void transfer_to_battle();
    void transfer_to_card();
    void transfer_to_draw();

private:
    int point;
    bool choose_phase = true;
    bool second_stage = false;
    bool show_host_stage = false;
    bool enter_client_stage = false;
    QList<FlashCommand*> command;
    QGraphicsPixmapItem* point_window;
    QGraphicsTextItem *point_text;
    QList<QGraphicsPixmapItem*> light;
    GraphicsTransition *transition;
    GameController *gc;

    QList<FlashCommand*> level_command;

    QGraphicsPixmapItem *connection_back;
    QGraphicsRectItem *black_rect;
    FlashCommand *host_command;
    FlashCommand *client_command;
    QGraphicsProxyWidget *proxy_IP;
    QGraphicsProxyWidget *proxy_port;
    QLabel *client_IP;
    QLabel *client_port;
    QString host_data_text;
    QGraphicsTextItem *host_data;
    QTimer *checkTimer;


    QString hostIp;
    int hostPort;
    int inputStatus;


    void command_in_animation();
    void command_out_animation();
    void command_active_animation();

    void change_background(int);
    void background_animation();

    //catch ip and port to display
    void findHostInfo();

private slots:
    void refresh();
    void startBattle();
    void check_connect();


protected:
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // SCENECHOOSE_H
