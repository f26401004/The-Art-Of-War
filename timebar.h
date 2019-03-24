#ifndef TIMEBAR_H
#define TIMEBAR_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include "bar.h"
#include <iostream>

class TimeBar : public QGraphicsPixmapItem
{
public:
    TimeBar(QGraphicsScene*);
    ~TimeBar();
    void refresh();
    void refresh_time_text();
    QRectF boundingRect() const;

private:
    Bar *time_bar;
    QTimer *counter;

};

#endif // TIMEBAR_H
