#include "flashcommand.h"
#include <iostream>

FlashCommand::FlashCommand(int x, int y, QPixmap pic)
{
    // set the picture and the position.
    setPixmap(pic);
    setPos(x, y);
    flash = new QGraphicsColorizeEffect();
    setGraphicsEffect(flash);
    flash->setColor(QColor(255, 255, 255, 255));
}

QRectF FlashCommand::boundingRect() const
{
    return QRectF(offset().x(), offset().y(), pixmap().width(), pixmap().height());
}

void FlashCommand::refresh(int frame_count)
{
    flash->setStrength(0.8 * sin((float)frame_count / 40 * 3.14159));
    //flash->setColor(QColor(255, 255, 255, 200 * sin((float)frame_count / 40 * 3.14159)));
    flash->update();
}
/*
void CampCard::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(QColor(255, 255, 255, 255 * sin(frame_count / 40 * 3.14159)));
    painter->drawRect(boundingRect());
}
*/

FlashCommand::~FlashCommand()
{
    delete flash;
}
