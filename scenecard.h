#ifndef SCENECARD_H
#define SCENECARD_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QList>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QDrag>
#include <QMimeData>
#include <QSound>
#include <QGraphicsBlurEffect>
#include <cmath>
#include <QDebug>
#include "graphicstransition.h"
#include "flashcommand.h"

class SceneCard : public QGraphicsScene
{
    Q_OBJECT
public:
    SceneCard();
    void reset(QList<int>, QList<int>);
    int frame_count;

signals:
    void transfer_to_choose();

private:
    QList<int> equip_card;
    QList<int> unequip_card;
    QList<QGraphicsPixmapItem*> equip_card_command;
    QList<FlashCommand*> unequip_card_command;
    FlashCommand *arrow_up;
    FlashCommand *arrow_down;
    int equip_card_index = 0;
    int unequip_card_index = 0;

    GraphicsTransition *transition;
    void card_in_animation();

    void refresh_move(int);
    int step;
    bool counter_clock = false;

    void write_data();

private slots:
    void refresh();

protected:
    virtual void keyPressEvent(QKeyEvent*);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent*);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent*);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent*);
    virtual void dropEvent(QGraphicsSceneDragDropEvent*);


};

#endif // SCENECARD_H
