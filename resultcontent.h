#ifndef RESULTCONTENT_H
#define RESULTCONTENT_H

#include <QGraphicsItem>
#include <QPainter>
#include <iostream>

class ResultContent : public QGraphicsItem
{
public:
    ResultContent();
    void set_content(int, int, double, double, int);
    QRectF boundingRect() const;
    void refresh();
    int paint_stage = 0;
    int anima_count = 0;

private:
    int level_id = 0;
    int remaining_time = 0;
    double enemy_rate = 0;
    double player_rate = 0;
    int g_point = 0;

protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
};

#endif // RESULTCONTENT_H
