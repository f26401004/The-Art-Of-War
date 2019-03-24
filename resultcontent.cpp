#include "resultcontent.h"
#include <QDebug>

ResultContent::ResultContent()
{

}

void ResultContent::refresh()
{
    switch(paint_stage)
    {
        case 1:
            update(QRectF(0, 0, 1280, 200));
            break;
        case 2:
            update(QRectF(0, 200, 1280, 400));
            break;
        case 3:
            update(QRectF(0, 600, 1280, 150));
            break;
        case 4:
            break;
    }
}

void ResultContent::set_content(int level, int time,  double e_rate, double p_rate, int point)
{
    level_id = level;
    remaining_time = time;
    enemy_rate = e_rate;
    player_rate = p_rate;
    g_point = point;
}

QRectF ResultContent::boundingRect() const
{
    return QRectF(0, 0, 1280, 720);
}

void ResultContent::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0)
{
    std::cout << static_cast<double>(anima_count) / 80 * 3.14159 << std::endl;
    if (paint_stage > 0)
    {
        // draw the level section
        painter->setFont(QFont("Bookman", 24));
        painter->setPen(QPen(QColor(255, 255, 255, 255*sin(static_cast<double>(anima_count) / 80 * 3.14159))));
        painter->drawText(x() + 300, y() + 100, 120, 64, Qt::AlignCenter, "評價 ");
        QPixmap pixmap(":/Graphics/level_" + QString::number(level_id));
        painter->drawPixmap(x() + 370, y() + 50, pixmap.width(), pixmap.height(), pixmap);
        painter->setPen(QPen(QColor(100, 14, 14, 255*sin(static_cast<double>(anima_count) / 80 * 3.14159))));
        // draw the content
        painter->setFont(QFont("Bookman", 32));
        QString text = "耗時 0" +
                QString::number((300000 - remaining_time) / 60000) +
                ":" +
                (((300000 - remaining_time) / 1000) % 60 > 9 ? QString::number(((300000 - remaining_time) / 1000) % 60) : "0" + QString::number(((300000 - remaining_time) / 1000) % 60)) +
                "結束戰役\n" +
                "敵方耗損率 " + QString::number(enemy_rate) + "%\n" +
                "我方耗損率 " + QString::number(player_rate) + "%";
        qDebug()<<"xxxxx"<<text;
        painter->drawText(x() + 200, y() + 300, 400, 400, Qt::AlignLeft, text);
        // draw the point text
        painter->drawText(x() + 650, y() + 600, 400, 64, Qt::AlignLeft, "獲得戰役成就點 " + QString::number(g_point) + " pt");
    }
}
