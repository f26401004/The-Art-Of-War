#ifndef PIBAR_H
#define PIBAR_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QColor>
#include <QPainter>
#include <qmath.h>
#include <iostream>

class PiBar : public QGraphicsPixmapItem
{
public:
    PiBar(int, int, int);
    void refresh(int);
    QRectF boundingRect() const;
    bool value_display;
    void setValue(int);
    void setColorType(int);
private:
    int radius;
    int value;
    int max;
    int color_type = -1;

protected:
    virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);

};

#endif // PIBAR_H

