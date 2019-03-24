#include "scenetitle.h"
#include <iostream>
#include <cmath>
#include <QSound>

SceneTitle::SceneTitle() : QGraphicsScene::QGraphicsScene()
{
    // initialize the scene size
    setSceneRect(0, 0, 1280, 720);
    // set the background pic
    QImage bg;
    bg.load(":/Graphics/SceneTitle.png");
    setBackgroundBrush(QBrush(bg));
    // add the "press to start" item
    p_back->setBrush(QBrush(QColor(0, 0, 0)));
    p_back->setOpacity(0.5);
    addItem(p_back);
    p_text->setDefaultTextColor(QColor(255, 255, 255));
    p_text->setPos(510, 520);
    p_text->setScale(3.0);
    p_text->setTextWidth(250);
    addItem(p_text);
    /*
    test = new Bar(250, 10, 100, 100);
    test->setX(10);
    test->setY(10);
    addItem(test);
    */
    srand((unsigned)time(NULL));
    for (int i = 0 ; i < 8 ; ++i)
    {
        QGraphicsPixmapItem *sprite = new QGraphicsPixmapItem();
        sprite->setPixmap(QPixmap(":/Graphics/flame.png"));
        sprite->setPos(rand() % 1280, rand() % 300 + 420);
        sprite->setOffset((-1)*sprite->pixmap().width() / 2, (-1)*sprite->pixmap().height() / 2);
        sprite->setOpacity(sin(rand() % 6));
        sprite->setRotation(rand() % 360);
        sprite->setScale(sin(rand() % 6));
        flame.push_back(sprite);
        addItem(sprite);
    }
    transition = new GraphicsTransition(0, 0, 1280, 720);
    addItem(transition);
}

void SceneTitle::reset()
{
    transition->start_in();
}

void SceneTitle::refresh()
{
    transition->update();
    press_animation();
    flame_animation();
    if (transition->opacity() == 1)
    {

        //emit display_command();
        emit transfer_to_choose();
    }
}

void SceneTitle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        // play the se.
        QSound::play(":/Audio/SE/cancel.wav");
        emit quit_signal();
    }
    else if (event->button() == Qt::LeftButton)
    {
        // play the se.
        QSound::play(":/Audio/SE/enter.wav");
        // set the transformation
        transition->start_out();
    }
}

void SceneTitle::keyPressEvent(QKeyEvent *event)
{
    // if press escape button
    if (event->key() == Qt::Key_Escape)
    {
        // play the se.
        QSound::play(":/Audio/SE/cancel.wav");
        emit quit_signal();
    }
    // if press enter button
    else if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        // play the se.
        QSound::play(":/Audio/SE/enter.wav");
        // set the transformation
        transition->start_out();
    }
}

void SceneTitle::press_animation()
{
    int flash = 1;
    if (flash)
    {
        p_back->setOpacity(0.5 + 0.3*sin((float)frame_count / 40 * 3.14159));
        if (frame_count == 0)
            flash = -flash;
    }
    else
    {
        p_back->setOpacity(0.8 - 0.3*sin((float)frame_count / 40 * 3.14159));
        if (frame_count == 0)
            flash = -flash;
    }
}

void SceneTitle::flame_animation()
{
    foreach(QGraphicsPixmapItem *iter, flame)
    {
        // refresh the flame
        //if (frame_count % 5 == 0)
        //{
            iter->moveBy(5, -1);

            iter->setScale(iter->scale() - 0.01);
            iter->setRotation(iter->rotation() + 0.1);
            iter->setOpacity(iter->opacity() - 0.01);
        //}
        // reset the flame if the point opacity <= 0
        if (iter->opacity() <= 0)
        {
            iter->setPos(rand() % 1280, rand() % 300 + 420);
            iter->setScale(sin(rand() % 6));
            iter->setRotation(rand() % 360);
            iter->setOpacity(sin(rand() % 6));
        }
    }
}

SceneTitle::~SceneTitle()
{
    delete p_back;
    delete p_text;
    foreach(QGraphicsPixmapItem* iter, flame)
        delete iter;
    flame.clear();
}
