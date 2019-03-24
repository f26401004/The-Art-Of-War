#ifndef BAR_H
#define BAR_H

#include <QGraphicsPixmapItem>
#include <QColor>
#include <QPainter>
#include <iostream>

class Bar : public QGraphicsPixmapItem
{
public:
    Bar(int, int, int, int);
    void refresh(int);
    QRectF boundingRect() const;
    bool value_display;
    void setValue(int);
    void setColorType(int);

private:
    int width;
    int height;
    int value;
    int max;
    int color_type = -1;

protected:
    virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
};

#endif // BAR_H
