#ifndef WINDOWBASE_H
#define WINDOWBASE_H

#include <QObject>
#include <QGraphicsPixmapItem>

class WindowBase : public QGraphicsPixmapItem
{
   //Q_OBJECT
public:
    WindowBase();
    WindowBase(int, int, int, int);
    QRectF boundingRect() const;
    void update();

private:
    int x, y, width, height;
    void draw_content();

};

#endif // WINDOWBASE_H
