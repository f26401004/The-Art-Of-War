#ifndef EMITTER_H
#define EMITTER_H

#include <QList>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QDebug>
#include <cmath>
#include <ctime>
#include "particle.h"

class Emitter : public QGraphicsPixmapItem
{
public:
    Emitter(int, int, int tpye = -1);
    ~Emitter();
    QList<Particle*> list;
    void refresh();
    QRectF boundingRect() const;

private:
    int type;
    int target_x;
    int target_y;

protected:
    virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);


};

#endif // EMITTER_H
