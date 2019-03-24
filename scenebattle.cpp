#include "scenebattle.h"

SceneBattle::SceneBattle(GameController *parent, int level, int camp, int) :
    counter(new QTimer()),
    gc(parent)
{
    // initialize the scene size
    setSceneRect(0, 0, 1280, 720);
    // create the time bar
    time_bar_frame = new QGraphicsPixmapItem(QPixmap(":/Graphics/TimeBarFrame.png"));
    time_bar_frame->setOffset(-505, -26);
    time_bar_frame->setPos(640, 32);
    time_bar_frame->setZValue(998);
    time_bar = new Bar(998, 10, 300000, 300000);
    time_bar->setPos(70.5, 5.5);
    time_bar->setZValue(999);
    time_bar->setColorType(3);
    time_text = new QGraphicsTextItem("03:00");
    time_text->setFont(QFont("Bookman", 16));
    time_text->setDefaultTextColor(QColor(255, 255, 255, 200));
    time_text->setPos(610, 27);
    time_text->setZValue(1000);
    addItem(time_bar_frame);
    addItem(time_bar);
    addItem(time_text);
    // create the mana bar
    mana_bar_frame = new QGraphicsPixmapItem(QPixmap(":/Graphics/ManaFrame.png"));
    mana_bar_frame->setPos(850, 520);
    mana_bar_frame->setZValue(999);
    mana_bar = new PiBar(80, 0, 100);
    mana_bar->setPos(540, 262);
    mana_bar->setZValue(998);
    mana_bar->value_display = false;
    mana_text = new QGraphicsTextItem("Mana " + QString::number(player_mana));
    mana_text->setFont(QFont("Bookman", 22));
    mana_text->setDefaultTextColor(QColor(255, 255, 255, 200));
    mana_text->setPos(925, 625);
    mana_text->setZValue(998);
    addItem(mana_bar_frame);
    addItem(mana_bar);
    addItem(mana_text);
    // set camp type
    camp_type = camp;
    // rand the card and initialize the card;
    srand(static_cast<unsigned>(time(NULL)));
    for (int i = 0 ; i < 3 ; i++)
    {
        //player_card[i] = rand() % 5 + camp_type * 5 + 1;
        player_card[i] = rand() % 3 + 1;
        card[i] = new FlashCommand(10 + 100 * i, 680, QPixmap(":/Graphics/" + QString::number(player_card[i]) + ".png"));
        card[i]->setZValue(998);
        addItem(card[i]);
    }
    // create the movement type command
    movement_type[0] = new FlashCommand(40, 515, QPixmap(":/Graphics/movement_attack.png"));
    movement_type[0]->setZValue(999);
    movement_type[0]->setOpacity(0);
    addItem(movement_type[0]);
    movement_type[1] = new FlashCommand(40, 560, QPixmap(":/Graphics/movement_stay.png"));
    movement_type[1]->setZValue(999);
    movement_type[1]->setOpacity(0);
    addItem(movement_type[1]);
    // create the location command;
    location_select = new FlashCommand(0, 0, QPixmap(":/Graphics/active_location.png"));
    location_select->setZValue(1000);
    location_select->setOpacity(0);
    addItem(location_select);

    connect(gc, SIGNAL(isAI()), this, SLOT(AI_setup()));
    connect(gc, SIGNAL(isp2p()), this, SLOT(p2p_setup()));
    // set the AI level
    AI_level = level;

    // initialize the music
    battle_music = new QMediaPlayer();
    music_list = new QMediaPlaylist();
    music_list->setPlaybackMode(QMediaPlaylist::Loop);
    battle_music->setPlaylist(music_list);

    // initialize graphic transtion
    transition = new GraphicsTransition(0, 0, 1280, 720);
    addItem(transition);

}

void SceneBattle::set_map_and_level(int map_id, int level)
{
    map = map_id;
    AI_level = level;
    // set the background pic
    QImage bg;
    switch(map_id)
    {
    case 0:
        bg.load(":/Graphics/forest_map.png");
        break;
    case 1:
        bg.load(":/Graphics/desert_map.png");
        break;
    case 2:
        bg.load(":/Graphics/ruins_map.png");
        break;
    }
    setBackgroundBrush(QBrush(bg));
}

void SceneBattle::set_card(QList<int> deck)
{
    for (int i = 0 ; i < 5 ; ++i)
    {
        if (i < 3)
            player_card[i] = deck.at(i);
        else
            player_rest_card[i-3] = deck.at(i);
    }
}

void SceneBattle::battle_start()
{
    // set the time and count to zero
    counter->setSingleShot(true);
    counter->start(300000);
    connect(counter, SIGNAL(timeout()), this, SLOT(emit_signal()));
    // play the bgm
    switch(AI_level)
    {
        case 0:
            music_list->addMedia(QMediaContent(QUrl("qrc:/Audio/BGM/easy_music_" + QString::number(rand() % 3 + 1) + ".mp3")));
            break;
        case 1:
            music_list->addMedia(QMediaContent(QUrl("qrc:/Audio/BGM/hard_music_" + QString::number(rand() % 3 + 1) + ".mp3")));
            break;
        case 2:
            music_list->addMedia(QMediaContent(QUrl("qrc:/Audio/BGM/nightmare_music_" + QString::number(rand() % 3 + 1) + ".mp3")));
            break;
    }

    music_list->setCurrentIndex(0);
    battle_music->play();
}

int SceneBattle::get_winner()
{
   return winner;
}

int SceneBattle::get_rate(int type)
{
    switch(type)
    {
        case 0:
            return static_cast<int>(player_minon.at(0)->battler.get_hp()) / 50;
        case 1:
            if(gc->mode=="AI")
                return static_cast<int>(AI->enemy_minon.at(0)->battler.get_hp()) / 50;
            else
                return static_cast<int>(enemy_minon.at(0)->battler.get_hp())/50;
    }
}

int SceneBattle::get_interval()
{
    return counter->remainingTime() / 1000;
}

void SceneBattle::emit_signal()
{
    battle_music->stop();
    // emit the signal to gc and transfer to result screen.
    transition->start_out();
}

void SceneBattle::AI_setup()
{
    disconnect(gc, SIGNAL(isAI()), this, SLOT(AI_setup()));
    disconnect(gc, SIGNAL(isp2p()), this, SLOT(p2p_setup()));
    // create the AI
    AI = new EnemyAI(AI_level, camp_type);
    addItem(AI);
    AI_timer = new QTimer();
    AI_timer->start((10 - AI_level * 1.5) * 1000);
    connect(AI_timer, SIGNAL(timeout()), AI, SLOT(AI_make_decision()));
}

void SceneBattle::p2p_setup()
{
    disconnect(gc, SIGNAL(isAI()), this, SLOT(AI_setup()));
    disconnect(gc, SIGNAL(isp2p()), this, SLOT(p2p_setup()));
}

void SceneBattle::readCommand()
{
    qDebug()<<"in readCommand()!!";
    QDataStream in;
    if(gc->modeChar == "host")  in.setDevice(gc->clientConnection);
    else in.setDevice(gc->tcpSocket);

    in.startTransaction();

    int minon_id, loc_x, loc_y, type, type_ind;
    in>>minon_id>>loc_x>>loc_y>>type>>type_ind;
    qDebug()<<"read : "<<minon_id<<" "<<loc_x<<" "<<loc_y<<" "<<type<<" "<<type_ind;

    set_enemy_minion(minon_id, (1240-loc_x*32)/32, loc_y, type, type_ind);

    in.abortTransaction();
}
void SceneBattle::refresh()
{
    // update the transition
    transition->update();
    // regain the mana and refresh mana bar
    regain_mana();
    // refresh and update the time bar
    time_bar->refresh(counter->remainingTime());
    time_bar->update(time_bar->boundingRect());
    time_text->setPlainText("0" + QString::number(counter->remainingTime() / 60000) + ":" + ((counter->remainingTime() / 1000) % 60 > 9 ? QString::number((counter->remainingTime() / 1000) % 60) : "0" + QString::number((counter->remainingTime() / 1000) % 60)));

    mana_bar->refresh(player_mana);
    mana_bar->update(mana_bar->boundingRect());

    SpriteBattler::frame_count = frame_count;

    // refresh the sprite
    if(gc->mode == "AI"){
        foreach (SpriteBattler*iter, player_minon+AI->enemy_minon)
        {
            iter->refresh();
            iter->update();
            // if the minon's hp is zero, then remove the minon from scene
            if(iter->battler.judge_hp_zero())
            {
                // if the minon is castle, then transfer to result
                if (iter->battler.get_id() == 16)
                {
                    battle_music->stop();
                    transition->start_out();
                    winner = iter->battler.ourside_enemy == true ? 0 : 1;
                    break;
                }
                if(iter->battler.ourside_enemy)
                    player_minon.removeOne(iter);
                else
                    AI->enemy_minon.removeOne(iter);
                removeItem(iter);
                delete iter;
            }
        }
    }
    else{
        foreach (SpriteBattler*iter, player_minon+enemy_minon)
        {
            iter->refresh();
            iter->update();
            // if the minon's hp is zero, then remove the minon from scene
            if(iter->battler.judge_hp_zero())
            {
                // if the minon is castle, then transfer to result
                if (iter->battler.get_id() == 16)
                {
                    battle_music->stop();
                    transition->start_out();
                    winner = iter->battler.ourside_enemy == true ? 0 : 1;
                    break;
                }
                if(iter->battler.ourside_enemy)
                    player_minon.removeOne(iter);
                else
                    enemy_minon.removeOne(iter);
                removeItem(iter);
                delete iter;
                iter = NULL;
            }
        }
    }
    // update the attack event
    attack_event();


    switch(phase)
    {
        // card choose phase
        case 0:
            card_active_animation();
            break;
        // movement choose phase
        case 1:
            movement_active_animation();
            break;
        // location choose phase
        case 2:
            location_select->setOpacity(0.8 * sin(static_cast<double>(frame_count) / 40 * 3.14159));
            location_select->setPos(x_index * 32, y_index * 32);
            break;
        case 3:
            break;
    }
    if (transition->opacity() == 1)
        emit transfer_to_result();
}

void SceneBattle::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    switch(phase)
    {
        // card choose phase
        case 0:
            if (event->lastScenePos() != event->scenePos())
            {
                if (event->scenePos().y() > 480 && event->scenePos().y() < 720)
                {
                    for (int i = 0 ; i < 3 ; ++i)
                    {
                        if (event->scenePos().x() > 10 + 100 * i && event->scenePos().x() < 110 + 100 * i)
                        {
                            card_index = i;
                            // if the index changed
                            if (last_index != card_index)
                            {
                                // play the se.
                                QSound::play(":/Audio/SE/move.wav");
                                last_index = card_index;
                            }
                        }
                    }
                }
            }
            break;
        // movement choose phase
        case 1:
            if (event->lastScenePos() != event->scenePos())
            {
                if (event->scenePos().x() > 40 + card_index*100 && event->scenePos().x() < 172 + card_index*100)
                {
                    if (event->scenePos().y() > 515 && event->scenePos().y() < 539)
                    {
                        type_index = 0;
                        // if the index changed
                        if (last_index != type_index)
                        {
                            // play the se.
                            QSound::play(":/Audio/SE/move.wav");
                            last_index = type_index;
                        }
                    }
                    else if (event->scenePos().y() > 560 && event->scenePos().y() < 594)
                    {
                        type_index = 1;
                        // if the index changed
                        if (last_index != type_index)
                        {
                            // play the se.
                            QSound::play(":/Audio/SE/move.wav");
                            last_index = type_index;
                        }
                    }
                }
            }
            break;
        // location choose phase
        case 2:
            if (event->lastScenePos() != event->scenePos())
            {
                if (event->scenePos().x() < 534)
                {
                    if (!(event->scenePos().x() < x_index * 32 && event->scenePos().x() < (x_index + 1) * 32) ||
                            !(event->scenePos().y() < y_index * 32 && event->scenePos().y() < (y_index + 1) * 32))
                    {
                        x_index = event->scenePos().x() / 32;
                        y_index = event->scenePos().y() / 32;
                        if (last_index != x_index + y_index * 16)
                        {
                            // play the se.
                            QSound::play(":/Audio/SE/move.wav");
                            last_index = x_index + y_index * 16;
                        }
                    }
                }
            }
            break;
    }
}


void SceneBattle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    switch(phase)
    {
        // card choose phase
        case 0:
            if (event->button() == Qt::LeftButton)
            {
                // play the se.
                QSound::play(":/Audio/SE/enter.wav");
                // enter the movement choose phase
                phase = 1;
                // display the movement command
                movement_type[0]->setOpacity(1);
                movement_type[1]->setOpacity(1);
                movement_type[0]->setX(40 + card_index * 100);
                movement_type[1]->setX(40 + card_index * 100);
                // set the choosed card be bright
                card[card_index]->flash->setColor(QColor(255, 255, 255, 200));
            }
            break;
        // movement choose phase
        case 1:
            if (event->button() == Qt::LeftButton)
            {
                // play the se.
                QSound::play(":/Audio/SE/enter.wav");
                // enter the location choose phase
                phase = 2;
                // hide the movement command
                movement_type[0]->setOpacity(0);
                movement_type[1]->setOpacity(0);
                // display the location select command
                location_select->setOpacity(0.8);
            }
            else if (event->button() == Qt::RightButton)
            {
                // play the se.
                QSound::play(":/Audio/SE/cancel.wav");
                // hide the movement command
                movement_type[0]->setOpacity(0);
                movement_type[1]->setOpacity(0);
                // back to card choose phase
                phase = 0;
            }
            break;
        // location choose phase
        case 2:
            if (event->button() == Qt::LeftButton)
            {
                // set the result
                if (set_minon(player_card[card_index], x_index, y_index, type_index))
                {
                    // play the se.
                    QSound::play(":/Audio/SE/enter.wav");
                    // deal the card
                    rand_deal_card();
                    // back to card choose phase
                    phase = 0;
                    location_select->setOpacity(0);
                }
                else
                    // play the se.
                    QSound::play(":/Audio/SE/cancel.wav");
            }
            else if (event->button() == Qt::RightButton)
            {
                // play the se.
                QSound::play(":/Audio/SE/cancel.wav");
                // back to movement choose phase
                phase = 1;
                // hide the location select command
                location_select->setOpacity(0);
                // display the movement command
                movement_type[0]->setOpacity(1);
                movement_type[1]->setOpacity(1);
            }
            break;
    }
}

void SceneBattle::keyPressEvent(QKeyEvent *event)
{
    switch(phase)
    {
        // card choose phase
        case 0:
            switch(event->key())
            {
                case Qt::Key_Right :
                    // play the se.
                    QSound::play(":/Audio/SE/move.wav");
                    card_index = (card_index + 1) % 3;
                    last_index = card_index;
                    break;
                case Qt::Key_Left :
                    // play the se.
                    QSound::play(":/Audio/SE/move.wav");
                    card_index = (card_index - 1 > -1 ? card_index - 1 : 2);
                    last_index = card_index;
                    break;
                case Qt::Key_Enter :
                case Qt::Key_Return :
                    // play the se.
                    QSound::play(":/Audio/SE/enter.wav");
                    // enter the movement choose phase
                    phase = 1;
                    // display the movement command
                    movement_type[0]->setOpacity(1);
                    movement_type[1]->setOpacity(1);
                    movement_type[0]->setX(40 + card_index * 100);
                    movement_type[1]->setX(40 + card_index * 100);
                    // set the choosed card be bright
                    card[card_index]->flash->setColor(QColor(255, 255, 255, 200));
                    break;
            }
            break;
        // movement choose phase
        case 1:
            movement_active_animation();
            switch(event->key())
            {
                case Qt::Key_Up :
                case Qt::Key_Down :
                    // play the se.
                    QSound::play(":/Audio/SE/move.wav");
                    type_index = 1 - type_index;
                    last_index = type_index;
                    break;
                case Qt::Key_Escape :
                    // play the se.
                    QSound::play(":/Audio/SE/cancel.wav");
                    // hide the movement command
                    movement_type[0]->setOpacity(0);
                    movement_type[1]->setOpacity(0);
                    // back to card choose phase
                    phase = 0;
                    break;
                case Qt::Key_Return :
                case Qt::Key_Enter :
                    // play the se.
                    QSound::play(":/Audio/SE/enter.wav");
                    // enter the location choose phase
                    phase = 2;
                    // hide the movement command
                    movement_type[0]->setOpacity(0);
                    movement_type[1]->setOpacity(0);
                    // display the location select command
                    location_select->setOpacity(0.8);
                    break;
            }
            break;
        // location choose phase
        case 2:
            switch(event->key())
            {
                case Qt::Key_Escape :
                    // play the se.
                    QSound::play(":/Audio/SE/cancel.wav");
                    // back to movement choose phase
                    phase = 1;
                    // hide the location select command
                    location_select->setOpacity(0);
                    // display the movement command
                    movement_type[0]->setOpacity(1);
                    movement_type[1]->setOpacity(1);
                    break;
                case Qt::Key_Up :
                    // play the se.
                    QSound::play(":/Audio/SE/move.wav");
                    if (y_index - 1 > -1)
                        --y_index;
                    else
                        y_index = 21 - y_index;
                    last_index = x_index + y_index * 16;
                    break;
                case Qt::Key_Down :
                    // play the se.
                    QSound::play(":/Audio/SE/move.wav");
                    if (y_index + 1 < 22)
                        ++y_index;
                    else
                        y_index = 21 - y_index;
                    last_index = x_index + y_index * 16;
                    break;
                case Qt::Key_Left :
                    // play the se.
                    QSound::play(":/Audio/SE/move.wav");
                    if (x_index - 1 > -1)
                        --x_index;
                    else
                        x_index = 16 - x_index;
                    last_index = x_index + y_index * 16;
                    break;
                case Qt::Key_Right :
                    // play the se.
                    QSound::play(":/Audio/SE/move.wav");
                    if (x_index + 1 < 17)
                        ++x_index;
                    else
                        x_index = 16 - x_index;
                    last_index = x_index + y_index * 16;
                    break;
                case Qt::Key_Enter :
                case Qt::Key_Return :

                    // set the result
                    if (set_minon(player_card[card_index], x_index, y_index, type_index))
                    {
                        // play the se.
                        QSound::play(":/Audio/SE/enter.wav");
                        // deal the card
                        rand_deal_card();
                        // back to card choose phase
                        phase = 0;
                        location_select->setOpacity(0);
                    }
                    else
                        // play the se.
                        QSound::play(":/Audio/SE/cancel.wav");
                    break;
            }
            break;
    }
}

void SceneBattle::card_active_animation()
{
    card[card_index]->refresh(frame_count);
    card[(card_index + 1) % 3]->flash->setStrength(0);
    card[((card_index - 1) > -1 ? card_index - 1 : 2)]->flash->setStrength(0);
    if (card[card_index]->y() > 480)
        //card[card_index]->setY(680 - 200 * sin(static_cast<double>(frame_count) / 80 * 3.14159));
        card[card_index]->moveBy(0, -20);
    if (card[(card_index + 1) % 3]->y() < 680)
        //card[(card_index + 1) % 3]->setY(500 + 200 * sin(static_cast<double>(frame_count) / 80 * 3.14159));
        card[(card_index + 1) % 3]->moveBy(0, 20);
    if (card[((card_index - 1) > -1 ? card_index - 1 : 2)]->y() < 680)
        //card[((card_index - 1) > -1 ? card_index - 1 : 2)]->setY(500 + 200 * sin(static_cast<double>(frame_count) / 80 * 3.14159));
        card[((card_index - 1) > -1 ? card_index - 1 : 2)]->moveBy(0, 20);
}

void SceneBattle::movement_active_animation()
{
    movement_type[type_index]->refresh(frame_count);
    movement_type[1 - type_index]->flash->setStrength(0);
}

void SceneBattle::regain_mana()
{
    if (counter->remainingTime() % 200 >= 0)
    {
        if (player_mana < 100)  ++player_mana;
        if (gc->mode=="AI" && AI->mana<100)     ++AI->mana;
        mana_text->setPlainText("Mana "+QString::number(player_mana));
    }
}

int SceneBattle::set_minon(int minon_id, int loc_x, int loc_y, int type)
{
    GameMinon minon(minon_id);
    if (minon_exist(loc_x, loc_y))
        return 0;
    if (player_mana < minon.get_cost_mana())
        return 0;


    SpriteBattler *sprite = new SpriteBattler(loc_x, loc_y, minon, true, type_index);
    sprite->battler.ourside_enemy = true;
    sprite->set_move_type(type);

    player_mana -= minon.get_cost_mana();
    player_minon.push_back(sprite);
    addItem(sprite);


    //send data
    QByteArray byte;
    QDataStream stream(&byte, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_7);

    if(gc->modeChar == "client"){
        stream<<minon_id<<loc_x<<loc_y<<type<<type_index;
        qDebug()<<"send: "<<minon_id<<loc_x<<loc_y<<type<<type_index;
        gc->tcpSocket->write(byte);
    }
    else if(gc->modeChar == "host"){
        stream<<minon_id<<loc_x<<loc_y<<type<<type_index;
        qDebug()<<"send: "<<minon_id<<loc_x<<loc_y<<type<<type_index;
        gc->clientConnection->write(byte);
    }


    return 1;
}

int SceneBattle::set_enemy_minion(int minon_id, int loc_x, int loc_y, int type, int type_ind)
{

    GameMinon minon(minon_id);
    if (minon_exist(loc_x, loc_y))
        return 0;

    SpriteBattler *sprite = new SpriteBattler(loc_x, loc_y, minon, false, type_ind);
    sprite->battler.ourside_enemy = false;
    sprite->set_move_type(type);

    enemy_minon.push_back(sprite);
    addItem(sprite);

    return 1;

}

void SceneBattle::rand_deal_card()
{
    int rand_index = rand() % 2;
    // change the card.
    int buffer = player_card[card_index];
    player_card[card_index] = player_rest_card[rand_index];
    player_rest_card[rand_index] = buffer;
    // change the card pic.
    card[card_index]->setPixmap(QPixmap(":/Graphics/" + QString::number(player_card[card_index]) + ".png"));
}

void SceneBattle::animation_event(SpriteBattler *attacker, SpriteBattler *reciever)
{
    switch(attacker->battler.get_id())
    {
        case 3:
            if (attacker->get_anima_count() == 1 && attacker->get_pattern_id() == 3)
            {
                Emitter *emitter = new Emitter(reciever->x(), reciever->y(), 0);
                emitter->setZValue(9999);
                reciever->animation.push_back(emitter);
                addItem(emitter);
            }
            break;
        case 2:
            if (attacker->get_anima_count() == 1 && attacker->get_pattern_id() == 3)
            {
                Emitter *emitter = new Emitter(reciever->x(), reciever->y(), 2);
                emitter->setZValue(9999);
                reciever->animation.push_back(emitter);
                addItem(emitter);
            }
            break;
        case 1:
        case 4:
        case 5:
        case 6:
            if (attacker->get_anima_count() == 1 && attacker->get_pattern_id() == 3)
            {
                Emitter *emitter = new Emitter(reciever->x(), reciever->y(), 1);
                emitter->setZValue(9999);
                reciever->animation.push_back(emitter);
                addItem(emitter);
            }
            break;
        case 8:
            switch(attacker->battler.get_attack_type())
            {
            case 0:
                if (attacker->get_anima_count() == 1 && attacker->get_pattern_id() == 3)
                {
                    Emitter *emitter = new Emitter(reciever->x(), reciever->y(), 3);
                    emitter->setZValue(9999);
                    reciever->animation.push_back(emitter);
                    addItem(emitter);
                }
                break;
            case 1:
                if (attacker->get_anima_count() == 1 && attacker->get_pattern_id() == 3)
                {
                    Emitter *emitter = new Emitter(reciever->x(), reciever->y(), 4);
                    emitter->setZValue(9999);
                    reciever->animation.push_back(emitter);
                    addItem(emitter);
                }
                break;
            case 2:
                if (attacker->get_pattern_id() == 3)
                {
                    Emitter *emitter = new Emitter(reciever->x(), reciever->y(), 5);
                    emitter->setZValue(9999);
                    reciever->animation.push_back(emitter);
                    addItem(emitter);
                }
                break;
            }
            break;
    }
}

void SceneBattle::attack_event()
{
    if(gc->mode == "AI"){
        foreach(SpriteBattler *attacker, player_minon+AI->enemy_minon)
        {
            // search the target
            search_field_in_range(attacker);
            // if there are recievers exist
            if (attacker->recievers.size() != 0)
            {
                // attacker animation tag turn on
                attacker->battler.attacking = true;
                if (attacker->get_pattern_id() == 3)
                {
                    foreach(SpriteBattler*iter, attacker->recievers)
                    {
                        // if the minon id is nun, then cure the other minon.
                        if (attacker->battler.get_id() == 3)
                            // cure the reciever
                            iter->battler.cure_effect(attacker->battler);
                        else if (attacker->battler.get_id() == 9)
                            iter->battler.set_ATK_plus(true);
                        else
                        {
                            switch(iter->battler.get_id())
                            {
                                // warrior
                                case 1:
                                    if (rand() % 100 > 25)
                                        // attack the reciever
                                        iter->battler.attack_effect(attacker->battler);
                                    break;
                                default:
                                    // attack the reciever
                                    iter->battler.attack_effect(attacker->battler);
                                    break;
                            }
                        }
                        // deal the animation
                        animation_event(attacker, iter);
                    }
                    // if attack done, clear the list
                    if (attacker->get_anima_count() == 15 - static_cast<double>(attacker->battler.get_SPD()) / 5) - 1;
                    {
                        foreach(SpriteBattler*iter, attacker->recievers)
                        {
                            iter->battler.set_ATK_plus(false);
                            iter = NULL;
                        }
                        attacker->recievers.clear();
                    }
                }
            }
            else
                // attacker animation tag turn off
                attacker->battler.attacking = false;
        }
    }
    else{
        foreach(SpriteBattler *attacker, player_minon+enemy_minon)
        {
            // search the target
            search_field_in_range(attacker);
            // if there are recievers exist
            if (attacker->recievers.size() != 0)
            {
                // attacker animation tag turn on
                attacker->battler.attacking = true;
                if (attacker->get_pattern_id() == 3)
                {
                    foreach(SpriteBattler*iter, attacker->recievers)
                    {
                        // if the minon id is nun, then cure the other minon.
                        if (attacker->battler.get_id() == 3)
                            // cure the reciever
                            iter->battler.cure_effect(attacker->battler);
                        else if (attacker->battler.get_id() == 9)
                            iter->battler.set_ATK_plus(true);
                        else
                        {
                            switch(iter->battler.get_id())
                            {
                                // warrior
                                case 1:
                                    if (rand() % 100 > 25)
                                        // attack the reciever
                                        iter->battler.attack_effect(attacker->battler);
                                    break;
                                default:
                                    // attack the reciever
                                    iter->battler.attack_effect(attacker->battler);
                                    break;
                            }
                        }
                        // deal the animation
                        animation_event(attacker, iter);
                    }
                    // if attack done, clear the list
                    if (attacker->get_anima_count() == 15 - static_cast<double>(attacker->battler.get_SPD()) / 5) - 1;
                    {
                        foreach(SpriteBattler*iter, attacker->recievers)
                        {
                            iter->battler.set_ATK_plus(false);
                            iter = NULL;
                        }
                        attacker->recievers.clear();
                    }
                }
            }
            else
                // attacker animation tag turn off
                attacker->battler.attacking = false;
        }
    }
}

void SceneBattle::search_field_in_range(SpriteBattler *attacker)
{
    if (attacker->battler.ourside_enemy)
    {
        if (attacker->battler.get_id() != 3 && attacker->battler.get_id() != 9)
        {
            if(gc->mode == "AI"){
                foreach(SpriteBattler *iter, AI->enemy_minon)
                {
                    int delta_x = iter->x() - attacker->x();
                    int delta_y = iter->y() - attacker->y();
                    // if one is in the attack range , then return him.
                    if(sqrt(delta_x*delta_x + delta_y*delta_y) < attacker->battler.get_range() * 32)
                    {
                        // if minion is not Archer, need to break the for loop
                        if (attacker->battler.get_id() != 7)
                        {
                            if (attacker->recievers.size() == 0)
                            {
                                attacker->recievers.push_back(iter);
                                break;
                            }
                        }
                        else
                        {
                            if (!attacker->recievers.contains(iter))
                                attacker->recievers.push_back(iter);
                        }
                    }
                }
            }
            else{
                foreach(SpriteBattler *iter, enemy_minon)
                {
                    int delta_x = iter->x() - attacker->x();
                    int delta_y = iter->y() - attacker->y();
                    // if one is in the attack range , then return him.
                    if(sqrt(delta_x*delta_x + delta_y*delta_y) < attacker->battler.get_range() * 32)
                    {
                        // if minion is not Archer, need to break the for loop
                        if (attacker->battler.get_id() != 7)
                        {
                            if (attacker->recievers.size() == 0)
                            {
                                attacker->recievers.push_back(iter);
                                break;
                            }
                        }
                        else
                        {
                            if (!attacker->recievers.contains(iter))
                                attacker->recievers.push_back(iter);
                        }
                    }
                }
            }
        }
        else
        {
            foreach(SpriteBattler *iter, player_minon)
            {
                int delta_x = iter->x() - attacker->x();
                int delta_y = iter->y() - attacker->y();
                // if one is in the cure range , then add him in one's reciever list.
                if(sqrt(delta_x*delta_x + delta_y*delta_y) < attacker->battler.get_range() * 32 && iter->battler.get_hp() < iter->battler.get_max_hp() && iter != attacker && iter->battler.get_id() != 16)
                {
                    // if the minion is nun, then break for loop when search one person
                    if (attacker->battler.get_id() == 3)
                    {
                        if (attacker->recievers.size() == 0)
                        {
                            attacker->recievers.push_back(iter);
                            break;
                        }
                    }
                    // if the minion is poet, then add all person in range into reciever
                    else if (attacker->battler.get_id() == 9)
                    {
                        if (!attacker->recievers.contains(iter))
                            attacker->recievers.push_back(iter);
                    }
                }
            }
        }
    }
    else
    {
        if (attacker->battler.get_id() != 3 && attacker->battler.get_id() != 9)
        {
            foreach(SpriteBattler *iter, player_minon)
            {
                int delta_x = iter->x() - attacker->x();
                int delta_y = iter->y() - attacker->y();
                // if one is in the attack range , then add him to one's reciever.
                if(sqrt(delta_x*delta_x + delta_y*delta_y) < attacker->battler.get_range() * 32)
                {
                    // if minion is not Archer, need to break the for loop
                    if (attacker->battler.get_id() != 7)
                    {
                        if (attacker->recievers.size() == 0)
                        {
                            attacker->recievers.push_back(iter);
                            break;
                        }
                    }
                    else
                    {
                        if (!attacker->recievers.contains(iter))
                            attacker->recievers.push_back(iter);
                    }
                }
            }
        }
        else
        {
            if(gc->mode == "AI"){
                foreach(SpriteBattler *iter, AI->enemy_minon)
                {
                    int delta_x = iter->x() - attacker->x();
                    int delta_y = iter->y() - attacker->y();
                    // if one is in the cure range , then add him in one's reciever list.
                    if(sqrt(delta_x*delta_x + delta_y*delta_y) < attacker->battler.get_range() * 32 && iter->battler.get_hp() < iter->battler.get_max_hp() && iter != attacker && iter->battler.get_id() != 16)
                    {
                        // if the minion is nun, then break for loop when search one person
                        if (attacker->battler.get_id() == 3)
                        {
                            if (attacker->recievers.size() == 0)
                            {
                                attacker->recievers.push_back(iter);
                                break;
                            }
                        }
                        // if the minion is poet, then add all person in range into reciever
                        else if (attacker->battler.get_id() == 9)
                        {
                            if (!attacker->recievers.contains(iter))
                                attacker->recievers.push_back(iter);
                        }
                    }
                }
            }
            else{
                foreach(SpriteBattler *iter, enemy_minon)
                {
                    int delta_x = iter->x() - attacker->x();
                    int delta_y = iter->y() - attacker->y();
                    // if one is in the cure range , then add him in one's reciever list.
                    if(sqrt(delta_x*delta_x + delta_y*delta_y) < attacker->battler.get_range() * 32 && iter->battler.get_hp() < iter->battler.get_max_hp() && iter != attacker && iter->battler.get_id() != 16)
                    {
                        // if the minion is nun, then break for loop when search one person
                        if (attacker->battler.get_id() == 3)
                        {
                            if (attacker->recievers.size() == 0)
                            {
                                attacker->recievers.push_back(iter);
                                break;
                            }
                        }
                        // if the minion is poet, then add all person in range into reciever
                        else if (attacker->battler.get_id() == 9)
                        {
                            if (!attacker->recievers.contains(iter))
                                attacker->recievers.push_back(iter);
                        }
                    }
                }
            }
        }
    }
}

bool SceneBattle::minon_exist(int x, int y)
{
    if(gc->mode == "AI"){
        foreach(SpriteBattler*iter, player_minon+AI->enemy_minon)
        {
            if ((iter->x() == (x * 32 + 16) && iter->y() == (y * 32 + 16)))
                return true;
        }
    }
    else{
        foreach(SpriteBattler*iter, player_minon+enemy_minon)
        {
            if ((iter->x() == (x * 32 + 16) && iter->y() == (y * 32 + 16)))
                return true;
        }
    }
    return false;
}

void SceneBattle::reset()
{
    //for (int i = 0 ; i < 3 ; ++i)
        //player_card[i] = rand() % 3 + 1;
    phase = 0;
    player_mana = 0;
    card_index = 0;
    x_index = 0;
    y_index = 0;
    type_index = 0;
    winner = 0;
    last_index = 0;
    for (int i = 0 ; i < 3 ; ++i)
        card[i]->setPixmap(QPixmap(":/Graphics/" + QString::number(player_card[i]) + ".png"));
    if(gc->mode == "p2p"){
        foreach ( SpriteBattler *iter, player_minon+enemy_minon)   delete iter;        
        player_minon.clear();
        enemy_minon.clear();
    }
    if(gc->mode == "AI"){
        foreach ( SpriteBattler *iter, player_minon+AI->enemy_minon)   delete iter;
        AI->enemy_minon.clear();
        player_minon.clear();
    }
    if(gc->mode == "AI")    AI->reset();
    // add the tower
    GameMinon minon(16);
    player_tower = new SpriteBattler(4, 11, minon, true, 1);
    enemy_tower = new SpriteBattler(36, 11, minon, false, 1);
    // add in minon list
    player_minon.push_back(player_tower);
    if(gc->mode == "AI")    AI->enemy_minon.push_back(enemy_tower);
    else enemy_minon.push_back(enemy_tower);
    addItem(player_tower);
    addItem(enemy_tower);
    transition->start_in();
}

SceneBattle::~SceneBattle()
{
    delete [] card;
    delete [] movement_type;
    delete location_select;
    delete counter;
    delete time_bar_frame;
    delete time_bar;
    delete time_text;
    delete mana_bar_frame;
    delete mana_bar;
    delete mana_text;
    foreach(SpriteBattler* iter, player_minon)
        delete iter;
    player_minon.clear();
    delete player_tower ;
    delete enemy_tower;
    if(gc->mode == "AI"){
        delete AI;
        delete AI_timer;
    }
    delete battle_music;
    delete music_list;
}
