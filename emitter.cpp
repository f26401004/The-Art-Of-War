#include "emitter.h"

Emitter::Emitter(int pos_x, int pos_y, int type)
{
    setPos(0, 0);
    target_x = pos_x;
    target_y = pos_y;
    setPixmap(QPixmap(1280, 720));
    this->type = type;
    // create the particle
    switch(type)
    {
        // cure
        case 0:
            for (int i = 0 ; i < 20 ; ++i)
            {
                Particle *sprite = new Particle();
                sprite->setPos(rand() % 48 - 24, rand() % 64 - 32);
                sprite->set_type(0);
                sprite->setScale(1);
                sprite->setRotation(0);
                sprite->add_velocity(static_cast<double>(rand() % 300) / 100 - 1.5, 2.5);
                sprite->setOpacity(0.8*sin(rand()%6)+0.2);
                list.push_back(sprite);
            }
            break;
        // flash
        case 1:
            for (int i = 0 ; i < 10 ; ++i)
            {
                Particle *sprite = new Particle();
                sprite->set_type(1);
                sprite->setPos(-16, -(rand() % 32)+16);
                sprite->setRotation(45);
                sprite->setScale(1);
                sprite->add_velocity(0, 0);
                sprite->setOpacity(sin(rand() % 6));
                list.push_back(sprite);
            }
            break;
        // bullet
        case 2:
            for (int i = 0 ; i < 10 ; ++i)
            {
                Particle *sprite = new Particle();
                sprite->set_type(2);
                sprite->setPos(-16, -16);
                sprite->setRotation(rand() % 360);
                sprite->setScale(0.4);
                sprite->add_velocity(0, -5);
                sprite->setOpacity(1);
                list.push_back(sprite);
            }
            break;
        // flame
        case 3:
            for (int i = 0 ; i < 300 ; ++i)
            {
                Particle *sprite = new Particle();
                sprite->set_type(3);
                if (i < 75)
                {
                    sprite->setPos(0, rand() % 10 - 5);
                    sprite->add_velocity(0, static_cast<float>(rand() % 800) / 100);
                }
                else
                {
                    sprite->setPos(rand() % 10 - 5, rand() % 10 - 5);
                    sprite->add_velocity(static_cast<float>(rand() % 1000) / 100 - 5, static_cast<float>(rand() % 800) / 100);
                }
                sprite->setScale(1);
                //sprite->setRotation(static_cast<float>(rand() % 600) / 100 - 3);
                //sprite->setRotation(static_cast<float>(rand() % 400) / 100 - 2);
                sprite->setOpacity(0.5*sin(rand()%6)+0.5);
                list.push_back(sprite);
            }
            for (int i = 0 ; i < 100 ; ++i)
            {
                Particle *sprite = new Particle();
                sprite->set_type(11);
                double angle = 3.14159*(static_cast<float>(rand() % 360) / 360)*2;
                sprite->setPos(10*cos(angle), 5*sin(angle) - 32);
                sprite->setScale(0.5);
                sprite->setRotation(0);
                sprite->add_velocity(40*cos(angle), 20*sin(angle) + 10);
                sprite->setOpacity(1);
                list.push_back(sprite);
            }
            for (int i = 0 ; i < 100 ; ++i)
            {
                Particle *sprite = new Particle();
                sprite->set_type(12);
                double angle = 3.14159*(static_cast<float>(rand() % 360) / 360)*2;
                sprite->setPos(10*cos(angle), 5*sin(angle) - 32);
                sprite->setScale(0.5);
                sprite->setRotation(0);
                sprite->add_velocity(40*cos(angle), 20*sin(angle) + 10);
                sprite->setOpacity(0);
                list.push_back(sprite);
            }
            break;
        // ice
        case 4:
            for (int i = 0 ; i < 20; ++i)
            {
                Particle *sprite = new Particle();
                sprite->setPos(rand() % 32 - 16, -(rand() % 160)+80);
                sprite->set_type(4);
                sprite->setRotation(rand() % 360);
                sprite->setScale(1);
                //sprite->setOffset((-1)*sprite->pixmap().width() / 2, (-1)*sprite->pixmap().height() / 2);
                sprite->add_velocity(0, 5);
                sprite->setOpacity(1);
                list.push_back(sprite);
            }
            for (int i = 0 ; i < 10; ++i)
            {
                Particle *sprite = new Particle();
                sprite->setPos(rand() % 32 - 16, -160);
                sprite->set_type(9);
                sprite->setRotation(rand() % 360);
                sprite->setScale(1);
                //sprite->setOffset((-1)*sprite->pixmap().width() / 2, (-1)*sprite->pixmap().height() / 2);
                sprite->add_velocity(0, 5);
                sprite->setOpacity(1);
                list.push_back(sprite);
            }
            for (int i = 0 ; i < 50; ++i)
            {
                Particle *sprite = new Particle();
                sprite->setPos(rand() % 100 - 50, -(rand() % 100) + 50);
                sprite->set_type(10);
                sprite->setRotation(rand() % 360);
                sprite->setScale(1);
                //sprite->setOffset((-1)*sprite->pixmap().width() / 2, (-1)*sprite->pixmap().height() / 2);
                sprite->add_velocity(0, 5);
                sprite->setOpacity(1);
                list.push_back(sprite);
            }
            break;

        // lightning
        case 5:
            for (int i = 0 ; i < 10 ; ++i)
            {
                Particle *sprite = new Particle();
                sprite->set_type(5);
                sprite->setScale(0.5);
                sprite->setOpacity(0.5*sin(rand()%6)+0.5);
                sprite->setRotation(rand() % 360);
                sprite->setPos(rand() % 100 - 50, rand() % 100 - 50);
                sprite->add_velocity(static_cast<float>(rand() % 100) / 100 - 2, static_cast<float>(rand() % 100) / 100 - 2);
                list.push_back(sprite);
            }
            break;
        // light point
        case 6:
            for (int i = 0 ; i < 20 ; ++i)
            {
                Particle *sprite = new Particle();
                sprite->set_type(6);
                sprite->setOpacity(0.8*sin(rand()%6)+0.2);
                sprite->setRotation(rand() % 360);
                sprite->setPos(rand() % 200 - 100, rand() % 200 - 100);
                sprite->add_velocity(static_cast<float>(rand() % 100) / 100 - 0.5, static_cast<float>(rand() % 100) / 100 - 0.5);
                list.push_back(sprite);
            }
            break;
        // light explosion
        case 7:
            for (int i = 0 ; i < 10 ; ++i)
            {
                Particle *sprite = new Particle();
                sprite->setPos(640, 360);
                sprite->set_type(7);
                sprite->add_velocity(0, (-1)*static_cast<float>(rand() % 100) / 100);
                sprite->setOpacity(0.8*sin(rand()%6)+0.2);
                list.push_back(sprite);
            }
            break;
        // lightning explosion
        case 8:
            for (int i = 0 ; i < 5 ; ++i)
            {
                Particle *sprite = new Particle();
                sprite->setPos(640, 360);
                sprite->set_type(8);
                sprite->setScale(5);
                sprite->add_velocity(0, (-1)*static_cast<float>(rand() % 100) / 100);
                sprite->setOpacity(0.8*sin(rand()%6)+0.2);
                list.push_back(sprite);
            }
            break;
    }
}

void Emitter::refresh()
{
    update(boundingRect());
    foreach( Particle*iter, list)
    {
        iter->refresh();
        if (iter->is_dead())
        {
            list.removeOne(iter);
            delete iter;
        }
    }
}

QRectF Emitter::boundingRect() const
{
    return QRectF(0, 0, 1280, 720);
}

void Emitter::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::HighQualityAntialiasing);
    painter->setCompositionMode(QPainter::CompositionMode_Plus);
    foreach(Particle*iter, list)
    {
        painter->resetTransform();
        painter->translate(target_x, target_y);
        painter->setOpacity(iter->opacity());
        painter->rotate(iter->rotation());
        painter->scale(iter->scale(), iter->scale());
        painter->drawPixmap(-iter->x()/2 - 16, -iter->y()/2 - 16, iter->pixmap());
    }
}

Emitter::~Emitter()
{

}
