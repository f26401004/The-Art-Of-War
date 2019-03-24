#ifndef GRAPHICSTRANSITION_H
#define GRAPHICSTRANSITION_H

#include <QGraphicsRectItem>
#include <QBrush>

class GraphicsTransition : public QGraphicsRectItem
{
public:
    GraphicsTransition(qreal, qreal, qreal, qreal);
    void update();
    void start_in();
    void start_out();
    bool get_in();
    bool get_out();


private:
    bool in = false;
    bool out = false;

};

#endif // GRAPHICSTRANSITION_H
