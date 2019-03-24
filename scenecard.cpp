#include "scenecard.h"
#include <iostream>

SceneCard::SceneCard()
{
    // initialize the scene size
    setSceneRect(0, 0, 1280, 720);
    // set the background pic
    QImage bg;
    bg.load(":/Graphics/Scene_Card/background.png");
    setBackgroundBrush(QBrush(bg));
    // initialize the transition
    transition = new GraphicsTransition(0, 0, 1280, 720);
    addItem(transition);
    // initialize the arrow
    arrow_up = new FlashCommand(700, 410, QPixmap(":/Graphics/Scene_Card/arrow_up.png"));
    arrow_up->setOffset(-arrow_up->pixmap().width() / 2, -arrow_up->pixmap().height() / 2);
    arrow_down = new FlashCommand(700, 660, QPixmap(":/Graphics/Scene_Card/arrow_down.png"));
    arrow_down->setOffset(-arrow_down->pixmap().width() / 2, -arrow_down->pixmap().height() / 2);
    addItem(arrow_up);
    addItem(arrow_down);
    // initialize the emitter
}

void SceneCard::reset(QList<int> equip_deck, QList<int> unequip_deck)
{
    equip_card_index = 0;
    unequip_card_index = 0;
    // initialize the deck
    equip_card = equip_deck;
    unequip_card = unequip_deck;
    // delete original card
    foreach (QGraphicsPixmapItem *iter, equip_card_command) {
        //equip_card_command.removeOne(iter);
        delete iter;
    }
    equip_card_command.clear();
    foreach (FlashCommand *iter, unequip_card_command) {
        //unequip_card_command.removeOne(iter);
        delete iter;
    }
    unequip_card_command.clear();
    // rebuild the command
    for (int i = 0 ; i < 5 ; ++i)
    {
        QGraphicsPixmapItem *sprite;
        if (i < equip_card.size() && equip_card.at(i) != 0)
            sprite = new QGraphicsPixmapItem(QPixmap(":/Graphics/Scene_Card/equip_" + QString::number(equip_card.at(i)) + ".png"));
        else
            sprite = new QGraphicsPixmapItem(QPixmap(":/Graphics/Scene_Card/empty.png"));
        sprite->setPos(10 + i*250 + 1280, 0);
        sprite->setZValue(999);
        sprite->setAcceptDrops(true);
        equip_card_command.push_back(sprite);
        addItem(sprite);
    }
    for (int i = 0 ; i < unequip_card.size() ; ++i)
    {
        FlashCommand *sprite = new FlashCommand(-100 + 100 * cos(3.14159 * (i) / unequip_card.size()), 400 + 280 * sin(3.14159 * (i) / unequip_card.size()), QPixmap(":/Graphics/Scene_Card/unequip_" + QString::number(unequip_card.at(i)) + ".png"));
        unequip_card_command.push_back(sprite);
        sprite->flash->setStrength(0);
        addItem(sprite);
    }
    transition->start_in();
    // set the background pic
    QImage bg;
    bg.load(":/Graphics/Scene_Card/background.png");
    setBackgroundBrush(QBrush(bg));
    unequip_card_index = 0;
    step = 0;
    // reset the location unequip card command
    double d1 = 2 * 3.14159 / unequip_card_command.size();
    double d2 = d1 / 20;
    for (int i = 0 ; i < unequip_card_command.size() ; ++i)
    {
        double j = i - unequip_card_index + 2 ;
        double d = d1 * j + d2 * step;
        unequip_card_command.at(i)->setPos(-100 + 150*sin(d), 400+240*cos(d));
        unequip_card_command.at(i)->setZValue(unequip_card_command.at(i)->x());
        unequip_card_command.at(i)->setOpacity((unequip_card_command.at(i)->x() + 100) / 150);
    }
}

void SceneCard::refresh()
{
    // set the transtion update
    transition->update();
    // detect the move mode and move the unequip card comment
    refresh_move(counter_clock);
    // turn on the active card
    for (int i = 0 ; i < unequip_card.size() ; ++i)
    {
        if (i == unequip_card_index)
            unequip_card_command.at(i)->refresh(frame_count);
        else
            unequip_card_command.at(i)->flash->setStrength(0);
    }
    // deal the equip card command in animation
    card_in_animation();
    if (transition->opacity() == 1)
        emit transfer_to_choose();
}

void SceneCard::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key_Escape:
            // play the se.
            QSound::play(":/Audio/SE/cancle.wav");
            // start the transition
            transition->start_out();
            // write the card data
            write_data();
            break;
        case Qt::Key_Down:
            // play the se.
            QSound::play(":/Audio/SE/move.wav");
            unequip_card_index = (unequip_card_index + 1) % unequip_card_command.size();
            counter_clock = false;
            step += 20;
            break;
        case Qt::Key_Up:
            // play the se.
            QSound::play(":/Audio/SE/move.wav");
            unequip_card_index = (unequip_card_index - 1 > -1 ? unequip_card_index - 1 : unequip_card_command.size()-1);
            counter_clock = true;
            step += 20;
            break;

        case Qt::Key_Enter:
        case Qt::Key_Return:
            break;
    }
}

void SceneCard::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton &&
            event->scenePos().x() < 730 && event->scenePos().x() > 50 &&
            event->scenePos().y() < 560 && event->scenePos().y() > 400 )
    {
        // create a new drag
        QDrag *drag = new QDrag(this);
        // create the drag information
        QMimeData *mimeData = new QMimeData;
        // set the drag information
        mimeData->setText("drag_card");
        drag->setMimeData(mimeData);
        drag->setPixmap(QPixmap(":/Graphics/Scene_Card/equip_" + QString::number(unequip_card.at(unequip_card_index)) + ".png"));
        drag->setHotSpot(QPoint(drag->pixmap().width() / 2, drag->pixmap().height() / 2));
        // tag the drag on the cursor
        drag->exec();
    }
    else if (event->button() == Qt::RightButton)
    {
        // play the se.
        QSound::play(":/Audio/SE/cancle.wav");
        // start the transition
        transition->start_out();
        // write the card data
        write_data();
    }
}

void SceneCard::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

}

void SceneCard::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

}

void SceneCard::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    for (int i = 0 ; i < 5 ; ++i)
    {
        std::cout << event->scenePos().x() << std::endl;
        if (event->scenePos().x() > (10 + i*250) && event->scenePos().x() < (270 + i*250) &&
                event->scenePos().y() > 0 && event->scenePos().y() < 340)
        {
            if (equip_card.at(i) != 0)
            {
                // exchange the card
                int buffer = equip_card.at(i);
                equip_card.replace(i, unequip_card.at(unequip_card_index));
                unequip_card.replace(unequip_card_index, buffer);
                // refresh the command
                equip_card_command.at(i)->setPixmap(QPixmap(":/Graphics/Scene_Card/equip_" + QString::number(equip_card.at(i)) + ".png"));
                unequip_card_command.at(unequip_card_index)->setPixmap(QPixmap(":/Graphics/Scene_Card/unequip_" + QString::number(unequip_card.at(unequip_card_index)) + ".png"));
                //event->
            }
            else
            {

                // put the card on deck
                equip_card.replace(i, unequip_card.at(unequip_card_index));
                // update the unequip card
                unequip_card.removeOne(unequip_card.at(unequip_card_index));

                // refresh the command
                equip_card_command.at(i)->setPixmap(QPixmap(":/Graphics/Scene_Card/equip_" + QString::number(equip_card.at(i)) + ".png"));

                delete unequip_card_command.at(unequip_card_index);
                unequip_card_command.removeOne(unequip_card_command.at(unequip_card_index));
                step = 1;
                refresh_move(0);
            }
        }
    }
}

void SceneCard::card_in_animation()
{
    arrow_down->setScale(0.1*sin(3.14159*static_cast<float>(frame_count) / 40) + 1);
    arrow_down->refresh(frame_count);
    arrow_up->setScale(0.1*sin(3.14159*static_cast<float>(frame_count) / 40) + 1);
    arrow_up->refresh(frame_count);
    for (int i = 0 ; i < 5 ; ++i)
    {
        if (equip_card_command.at(i)->x() > 10 + i*250)
        {
            equip_card_command.at(i)->moveBy(-64, 0);
        }
    }
}


void SceneCard::refresh_move(int mode)
{
    if (mode == 0)
    {
        if (step > 0)
        {
            double d1 = 2 * 3.14159 / unequip_card_command.size();
            double d2 = d1 / 20;
            for (int i = 0 ; i < unequip_card_command.size() ; ++i)
            {
                double j = i - unequip_card_index + 90.0/(360.0/static_cast<double>(unequip_card_command.size()));
                double d = d1 * j + d2 * step;
                unequip_card_command.at(i)->setPos(-100 + 150*sin(d), 400+240*cos(d));
                unequip_card_command.at(i)->setZValue(unequip_card_command.at(i)->x());
                unequip_card_command.at(i)->setOpacity((unequip_card_command.at(i)->x() + 100) / 150);
            }
            --step;
        }
    }
    else
    {
        if (step > 0)
        {
            double d1 = 2 * 3.14159 / unequip_card_command.size();
            double d2 = d1 / 20;
            for (int i = 0 ; i < unequip_card_command.size() ; ++i)
            {
                double j = -(i - unequip_card_index + 90.0/(360.0/static_cast<double>(unequip_card_command.size())));
                double d = d1 * j + d2 * step;
                d = d*(-1);
                unequip_card_command.at(i)->setPos(-100 + 150*sin(d), 400+240*cos(d));
                unequip_card_command.at(i)->setZValue(unequip_card_command.at(i)->x());
                unequip_card_command.at(i)->setOpacity((unequip_card_command.at(i)->x() + 100) / 150);
            }
            --step;
        }
    }
}


void SceneCard::write_data()
{
    QFile wfile("./equip_card.dat");
    QDataStream wloader(&wfile);
    QByteArray byte;
    //QIODevice::Truncate will overwrite the file (clean the file everytime)
    if(!wfile.open(QIODevice::Truncate|QIODevice::WriteOnly)){
        qDebug()<<"file cannot open for writing";
        return ;
    }
    QString str;
    for (int i = 0 ; i < equip_card.size() ; i++)
    {
        str += QString::number(equip_card.at(i)) + " ";
    }
    byte = str.toLocal8Bit().toHex();
    wloader.writeRawData(byte.data(),byte.size());
    wfile.close();
    QFile wfile2("./unequip_card.dat");
    QDataStream wloader2(&wfile2);
    QByteArray byte2;
    QString str2;
    //QIODevice::Truncate will overwrite the file (clean the file everytime)
    if(!wfile2.open(QIODevice::Truncate|QIODevice::WriteOnly)){
        qDebug()<<"file cannot open for writing";
        return ;
    }
    for (int i = 0 ; i < unequip_card.size() ; i++)
    {
        str2 += QString::number(unequip_card.at(i)) + " ";
    }
    byte2 = str2.toLocal8Bit().toHex();
    wloader2.writeRawData(byte2.data(),byte2.size());
    wfile2.close();
}
