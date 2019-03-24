#ifndef SCENETITLE_H
#define SCENETITLE_H

#include <QObject>
#include <QGraphicsScene>
#include <QPixmap>
#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QSound>
#include <QGraphicsColorizeEffect>
#include <time.h>
#include "graphicstransition.h"

class SceneTitle : public QGraphicsScene
{
    Q_OBJECT
public:
    SceneTitle();
    ~SceneTitle();
    int frame_count;
    void reset();

signals:
    void quit_signal();
    void display_command();
    void transfer_to_choose();

private:
    QGraphicsRectItem *p_back = new QGraphicsRectItem(-10, 530, 1300, 48);
    QGraphicsTextItem *p_text = new QGraphicsTextItem("Press to start");
    void press_animation();
    void button_check();
    GraphicsTransition *transition;

    QList<QGraphicsPixmapItem*> flame;
    void flame_animation();

private slots:
    void refresh();

protected:
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

};

#endif // SCENETITLE_H
