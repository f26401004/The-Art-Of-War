#include "timebar.h"

TimeBar::TimeBar(QGraphicsScene* scene) :
    time_bar(new Bar(998, 10, 300000, 300000)),
    counter(new QTimer())
{
    // set time bar frame.
    setPixmap(QPixmap(":/Graphics/TimeBarFrame.png"));

    // initialize the time and count to zero.
    counter->setInterval(300000);
    //connect(counter, SIGNAL(timeout()), scene, SLOT(transfer_to_result()));
    // disappear the value and refresh
    time_bar->value_display = false;
    time_bar->update();
    time_bar->setOffset(-500, -5);
    time_bar->setX(this->x() + 3);
    time_bar->setY(this->y() + 2.5);
    // add the item to scene
    scene->addItem(this);
    scene->addItem(time_bar);

}

QRectF TimeBar::boundingRect() const
{
    return pixmap().rect();
}

void TimeBar::refresh()
{
    time_bar->setValue(counter->remainingTime());
    time_bar->update();
}

void TimeBar::refresh_time_text()
{

}

TimeBar::~TimeBar()
{
    delete time_bar;
    delete counter;
}
