#include "scenedraw.h"

SceneDraw::SceneDraw()
{
    // initialize the scene size
    setSceneRect(0, 0, 1280, 720);
    // set the background pic
    QImage bg;
    bg.load(":/Graphics/Scene_Draw/SceneDrawBack.png");
    setBackgroundBrush(QBrush(bg));
    // initialize card pic
    card = new QGraphicsPixmapItem(QPixmap(":/Graphics/Scene_Draw/card_back.png"));
    card->setOffset(-card->pixmap().width() / 2, -card->pixmap().height() / 2);
    card->setPos(640, 360);
    card->setZValue(9999);
    card->setOpacity(0);
    addItem(card);
    // initialize black back
    black_rect = new QGraphicsRectItem(0, 0, 500, 300);
    black_rect->setPos(605, 210);
    black_rect->setBrush(QBrush(QColor(0, 0, 0)));
    black_rect->setZValue(9998);
    black_rect->setOpacity(0);
    addItem(black_rect);
    // initialize text
    information = new QGraphicsTextItem();
    information->setDefaultTextColor(QColor(255, 255, 255));
    information->setZValue(9998);
    information->setOpacity(0);
    information->setFont(QFont("bookman", 18));
    information->setPos(625, 230);
    information->setOpacity(0);
    addItem(information);
    // initialize system text
    system_text = new QGraphicsTextItem("請按確認鍵繼續");
    system_text->setDefaultTextColor(QColor(255, 255, 255));
    system_text->setZValue(9998);
    system_text->setFont(QFont("bookman", 12));
    system_text->setPos(600, 600);
    system_text->setOpacity(0);
    addItem(system_text);
    // initialize point text
    point_text = new QGraphicsTextItem();
    point_text->setDefaultTextColor(QColor(255, 255, 255));
    point_text->setZValue(9998);
    point_text->setFont(QFont("bookman", 24));
    point_text->setPos(1000, 600);
    point_text->setOpacity(1);
    addItem(point_text);
    // initialize the graphic transition
    transition = new GraphicsTransition(0, 0, 1280, 720);
    addItem(transition);
}

void SceneDraw::reset(int value)
{
    point = value;
    card_id = 0;
    draw = false;
    card->setOpacity(0);
    black_rect->setOpacity(0);
    information->setOpacity(0);
    system_text->setOpacity(0);
    point_text->setPlainText("戰役成就 " + QString::number(point) + " pt");
    point_text->setPos(1000, 600);
    point_text->moveBy(-QString::number(point).size() * 12, 0);
    transition->start_in();
}

void SceneDraw::refresh()
{
    foreach(Emitter *iter, animation)
        iter->refresh();
    if (frame_count % 2 == 0)
    {
        Emitter *emitter = new Emitter(rand() % 1280, 720, 6);
        animation.push_back(emitter);
        addItem(emitter);
    }
    // set the transtion update
    transition->update();
    // deal the card rotate
    card_animation();
    card->update();
    if (transition->opacity() == 1)
        emit transfer_to_choose();
}

void SceneDraw::keyPressEvent(QKeyEvent *event)
{
    if (!draw)
    {
        if ((event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) && black_rect->opacity() == 0)
        {
            qDebug()<<point;
            // if the achievement point enough
            if (point >= 500)
            {
                qDebug()<<"in";
                // play the se.
                QSound::play(":/Audio/SE/enter.wav");
                point = point - 500;
                // the draw card id
                card_id = rand() % 13 + 1;
                // record the data
                write_data(card_id);
                point_text->setPlainText("戰役成就 " + QString::number(point) + " pt");
                point_text->setPos(1000, 600);
                point_text->moveBy(-QString::number(point).size() * 12, 0);
                // start the card animation;
                draw = true;
                rotate = true;
                card->setOpacity(1);
            }
            else
            {
                // play the se.
                QSound::play(":/Audio/SE/cancel.wav");
            }
        }
        else if (event->key() == Qt::Key_Escape)
        {
            // play the se.
            QSound::play(":/Audio/SE/cancel.wav");
            // start transition
            transition->start_out();
        }
    }
    else
    {
        if ((event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) && black_rect->opacity() >= 0.5)
        {
             // play the se.
             QSound::play(":/Audio/SE/enter.wav");
             // reset the data
             draw = false;
             card_id = 0;
             card->setPixmap(QPixmap(":/Graphics/Scene_Draw/card_back.png"));
        }
    }
}

void SceneDraw::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (!draw)
    {
        if (event->button() == Qt::LeftButton && black_rect->opacity() == 0)
        {
            // if the achievement point enough
            if (point >= 500)
            {
                // play the se.
                QSound::play(":/Audio/SE/enter.wav");
                point = point - 500;
                // the draw card id
                card_id = rand() % 13 + 1;
                // record the data
                write_data(card_id);
                point_text->setPlainText("戰役成就 " + QString::number(point) + " pt");
                point_text->setPos(1000, 600);
                point_text->moveBy(-QString::number(point).size() * 12, 0);
                // read the minion information
                read_information(card_id);
                // start the card animation;
                draw = true;
                rotate = true;
                card->setOpacity(1);

            }
            else
            {
                // play the se.
                QSound::play(":/Audio/SE/cancel.wav");
            }

        }
        else if (event->button() == Qt::RightButton)
        {
            // play the se.
            QSound::play(":/Audio/SE/cancel.wav");
            // start transition
            transition->start_out();
        }
    }
    else
    {
        if (event->button() == Qt::LeftButton && black_rect->opacity() >= 0.5)
        {
             // play the se.
             QSound::play(":/Audio/SE/enter.wav");
             // reset the data
             draw = false;
             card_id = 0;
             card->setPixmap(QPixmap(":/Graphics/Scene_Draw/card_back.png"));
        }
    }
}

void SceneDraw::card_animation()
{
    ++anima_count;
    if (anima_count > 30 + rotate_count * 10)
        anima_count = 1;
    if (draw)
    {
        if (rotate)
        {
            // rotate the card
            card->setTransform(QTransform::fromScale(-cos(2*3.14159*static_cast<double>(anima_count)/(30 + rotate_count * 10)), 1));
            // deal the times of rotate
            if (anima_count == 1)
                ++rotate_count;
            // change the pixmap
            if (anima_count == (30 + rotate_count * 10) / 4)
                card->setPixmap(QPixmap(":/Graphics/Scene_Draw/" +  QString::number(card_id) + ".png"));
            else if (anima_count == (30 + rotate_count * 10) / 4 * 3)
                card->setPixmap(QPixmap(":/Graphics/Scene_Draw/card_back.png"));
            // close the animation if the times of rotate equal to 4
            if (rotate_count == 3 && anima_count == (30 + rotate_count * 10) / 2)
            {
                rotate = false;
                // zero the rotate_count
                rotate_count = 0;
                // play animation
                Emitter *emiiter = new Emitter(640, 320, 7);
                animation.push_back(emiiter);
                addItem(emiiter);
            }
        }
        else
        {
            if (black_rect->opacity() < 0.5 && anima_count > 19)
            {
                black_rect->setOpacity(black_rect->opacity() + 0.05);
                information->setOpacity(information->opacity() + 0.1);
                card->moveBy(-30, 0);
            }
            system_text->setOpacity(sin(3.14159*static_cast<float>(frame_count) / 40));
            //qDebug() << "test";
        }
    }
    else
    {
        if (card->opacity() > 0)
        {
            card->setOpacity(card->opacity() - 0.1);
            card->moveBy(30, 0);
            black_rect->setOpacity(card->opacity());
            information->setOpacity(card->opacity());
        }
        if (system_text->opacity() > 0)
            system_text->setOpacity(system_text->opacity() - 0.05);
    }
}

void SceneDraw::read_information(int id)
{
    // read the minion data
    QFile file("./minion.dat");
    file.open(QIODevice::ReadOnly);
    QByteArray byte = QByteArray::fromHex(file.readAll());
    std::stringstream ss(byte.toStdString());
    std::string num, maxhp, hp, atk, str, def, spd, dex, agi, ran, cos;
    for(int i = 1 ; i <= id ; ++i)
        ss >> num >> maxhp >> hp>> atk >> str >> def >> spd >> dex >> agi >> ran >> cos;
    QString data = "\n\n HP " + QString::fromStdString(maxhp) +
            "  消耗mana " + QString::fromStdString(cos) + "\n ATK " + QString::fromStdString(atk) + "   STR " + QString::fromStdString(str) + "   DEF " + QString::fromStdString(def) +
            "\n SPD " + QString::fromStdString(spd) + "   DEX " + QString::fromStdString(dex) + "   AGI " + QString::fromStdString(agi) + "\n Attack Range " + QString::fromStdString(ran);
    switch(id)
    {
        case 1:
            information->setPlainText("戰士\n不怕困苦好勇善戰的戰士手持劍盾，有一\n定的機率能格檔敵人的攻擊。" + data);
            break;
        case 2:
            information->setPlainText("狙擊手\n攻擊範圍極大的狙擊手在後方支援前線，\n在戰場中往往會造成不容小覷的輸出。" + data);
        case 3:
            information->setPlainText("修女\n經過受洗後接受了上帝的指引，擁有神聖\n的力量能在一定範圍內恢復我方兵種血量。"+ data);
            break;
        case 4:
            information->setPlainText("潛行者\n擅常隱匿行跡與暗殺技巧的兵種，有機會\n暗殺敵人，給予敵人即死。"+data);
            break;
        case 5:
            information->setPlainText("大劍師\n人類陣營中單體傷害最高的兵種，擅長揮舞\n大劍，為後方的夥伴殺出一條安全的道路。"+ data);
            break;
        case 6:
            information->setPlainText("精靈騎士\n從小開始訓練，攻擊與速度兼具兵種，\n在戰場上能給與敵人有效的傷害。 "+ data);
            break;
        case 7:
            information->setPlainText("弓箭手\n長期在森林打獵的射手，擁有良好的視力\n以及精準度，能一次攻擊多個對象。"+ data);
            break;
        case 8:
            information->setPlainText("女巫\n自遠古以來存活的魔女，擁有強力的魔力\n並精通四大元素的魔法。"+ data);
            break;
        case 9:
            information->setPlainText("吟遊詩人\n在精靈部族中擅長創作和吟詠英雄的詩人歌\n手，能在一定範圍內增加我方兵種的攻擊力。"+ data);
            break;
        case 10:
            information->setPlainText("德魯伊\n擅常使用自然之力的術師，自古以來與精\n靈們簽訂契約共同作戰。"+ data);
            break;
        case 11:
            information->setPlainText("激光能力者\n操控光因子的能力者，能有效發出射程極\n遠的雷射光攻擊敵方。"+ data);
            break;
        case 12:
            information->setPlainText("生化機器\n結合高度人類智慧誕生的機器人，具有\n強大的防衛能力受到許多企業青睞。"+ data);
            break;
        case 13:
            information->setPlainText("無人機\n偵測用的無人機，附帶高清攝影機以及微\n型雷射槍，能潛入敵營混淆視聽。"+ data);
            break;
        case 14:
            break;
        case 15:
            break;
    }
    file.close();
}

void SceneDraw::write_data(int id)
{
    QFile wfile("./unequip_card.dat");
    QDataStream out(&wfile);
    QString data = QString::number(id) + " ";
    QByteArray byte = data.toLocal8Bit().toHex();
    out.setVersion(QDataStream::Qt_5_7);
    //QIODevice::Truncate will overwrite the file (clean the file everytime)
    if(!wfile.open(QIODevice::Append)){
        qDebug()<<"file cannot open for writing";
        return ;
    }
    out.writeRawData(byte.data(),byte.size());
    wfile.close();
    QFile wfile2("./achievement.dat");
    QDataStream out2(&wfile2);
    QByteArray byte2;
    //QIODevice::Truncate will overwrite the file (clean the file everytime)
    if(!wfile2.open(QIODevice::Truncate|QIODevice::WriteOnly)){
        qDebug()<<"file cannot open for writing";
        return ;
    }
    QString str2="point " + QString::number(point);
    qDebug()<<str2;
    byte2 = str2.toLocal8Bit().toHex();
    out2.writeBytes(byte2.data(), byte2.size());
    wfile2.close();
}
