#ifndef SCENERESULT_H
#define SCENERESULT_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPixmap>
#include <QFile>
#include <QDataStream>
#include <QKeyEvent>
#include <QSound>
#include <QGraphicsSceneMouseEvent>
#include <QTcpSocket>
#include <QHostAddress>
#include <QHostInfo>
#include <iostream>
#include <QByteArray>
#include "graphicstransition.h"
#include <sstream>
#include <string>


class SceneResult : public QGraphicsScene
{
    Q_OBJECT
public:
    SceneResult();
    ~SceneResult();
    void set_damage_rate(double, double);
    void set_interval(int);
    void output_level();
    int frame_count;

//-------------can move to private?-----------
    void writeFile();
    void readFile();
    void sendResult();
    struct Rank{
        QString name;
        int score;
    };
    QList<Rank> rank;
public slots:
    void readResult();
//-------------------------------------------

private:
    //ResultContent *content;
    QGraphicsPixmapItem *level_pic;
    QGraphicsTextItem *data;
    QGraphicsTextItem *achievement_point;
    QGraphicsTextItem *ranking;

    int paint_stage = 0;

    double enemy_damage_rate;
    double player_damage_rate;
    int interval;
    int level;
    int point;

    qint32 last_point;
    qint32 times;

    QString player_name;
    double score;

    QTcpSocket *tcpSocket;

    GraphicsTransition *transition;


signals:
    void transfer_to_title();

private slots:
    void refresh();
protected:
    virtual void keyPressEvent(QKeyEvent*);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

};

#endif // SCENERESULT_H
