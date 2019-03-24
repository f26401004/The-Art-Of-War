#ifndef SCENEDRAW_H
#define SCENEDRAW_H

#include <QGraphicsScene>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QTransform>
#include <QSound>
#include <QFile>
#include <sstream>
#include <string>
#include <QDebug>
#include <ctime>
#include "graphicstransition.h"
#include "emitter.h"

class SceneDraw : public QGraphicsScene
{
    Q_OBJECT
public:
    SceneDraw();
    int frame_count;
    void reset(int);

private slots:
    void refresh();

private:
    int point;
    int card_id = 0;
    int rotate_count = 0;
    int anima_count = 0;
    void write_data(int);
    void card_animation();
    void read_information(int = 0);
    GraphicsTransition *transition;
    QList<Emitter*> animation;
    QGraphicsPixmapItem *card;
    QGraphicsRectItem *black_rect;
    QGraphicsTextItem *information;
    QGraphicsTextItem *system_text;
    QGraphicsTextItem *point_text;
    bool draw = false;
    bool rotate = false;

protected:
    virtual void keyPressEvent(QKeyEvent*);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent*);

signals:
    void transfer_to_choose();

};

#endif // SCENEDRAW_H
