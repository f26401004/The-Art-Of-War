#ifndef CAMPCARD_H
#define CAMPCARD_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QColor>
#include <cmath>
#include <QGraphicsColorizeEffect>

class FlashCommand : public QGraphicsPixmapItem
{
    //Q_OBJECT
public:
    FlashCommand(int, int, QPixmap);
    ~FlashCommand();
    QGraphicsColorizeEffect *flash;
    QRectF boundingRect() const;
    //int frame_count;
    void refresh(int);

private:
    //virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private slots:


};

#endif // CAMPCARD_H
