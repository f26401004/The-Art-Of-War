#include "spritebattler.h"

// initialize the frame_count.
int SpriteBattler::frame_count = 0;

SpriteBattler::SpriteBattler()
{
}
SpriteBattler::SpriteBattler(int init_x, int init_y, GameMinon minon, bool our, int type) :
    battler(minon), std_x(init_x), std_y(init_y)
{
    anima_count=0;
    battler.ourside_enemy = our;
    move_type = type;
    //setPixmap(QPixmap(":/Graphics/MinonPixmap/" + minon.get_bitmap_string() + "_move_1.png"));
    // load all bitmap when create a new minion.
    for (int i = 0 ; i < 4 ; ++i)
    {
        move_pic[i].load(":/Graphics/MinonPixmap/" + minon.get_bitmap_string() + "_move_" + QString::number(i+1) +".png");
        attack_pic[i].load(":/Graphics/MinonPixmap/" + minon.get_bitmap_string() + "_attack_" + QString::number(i+1) +".png");
        if(!our)
        {
            move_pic[i] = move_pic[i].mirrored(true, false);
            attack_pic[i] = attack_pic[i].mirrored(true, false);
        }
    }
    setPixmap(QPixmap::fromImage(move_pic[0]));
    setPos(std_x * 32, std_y * 32);
    setOffset((-1)*pixmap().width() / 2, (-1)*pixmap().height() / 2);
    setZValue(std_y + 100);
    // set the target location
    srand((unsigned)time(NULL));
    if (our)
    {
        target_x = 1190;
        target_y = 352;
        // set the phase 1 location
        if(y() < 360)
        {
            switch(rand() % 3)
            {
                case 0:
                    phase_x = 16 * 32 + 16;
                    phase_y = 3 * 32 - 8 + 32;
                    break;
                case 1:
                    phase_x = 16 * 32 + 16;
                    phase_y = 4 * 32 - 8 + 32;
                    break;
                case 2:
                    phase_x = 16 * 32 + 16;
                    phase_y = 5 * 32 - 8 + 32;
                    break;
            }
        }
        else
        {
            switch(rand() % 3)
            {
                case 0:
                    phase_x = 16 * 32 + 16;
                    phase_y = 16 * 32 - 8 + 32;
                    break;
                case 1:
                    phase_x = 16 * 32 + 16;
                    phase_y = 17 * 32 - 8 + 32;
                    break;
                case 2:
                    phase_x = 16 * 32 + 16;
                    phase_y = 18 * 32 - 8 + 32;
                    break;
            }
        }
    }
    else
    {
        target_x = 96;
        target_y = 352;
        // set the phase 1 location
        if(y() < 360)
        {
            switch(rand() % 3)
            {
                case 0:
                    phase_x = 21 * 32 + 16;
                    phase_y = 3 * 32 - 8 + 32;
                    break;
                case 1:
                    phase_x = 21 * 32 + 16;
                    phase_y = 4 * 32 - 8 + 32;
                    break;
                case 2:
                    phase_x = 21 * 32 + 16;
                    phase_y = 5 * 32 - 8 + 32;
                    break;
            }
        }
        else
        {
            switch(rand() % 3)
            {
                case 0:
                    phase_x = 21 * 32 + 16;
                    phase_y = 16 * 32 - 8 + 32;
                    break;
                case 1:
                    phase_x = 21 * 32 + 16;
                    phase_y = 17 * 32 - 8 + 32;
                    break;
                case 2:
                    phase_x = 21 * 32 + 16;
                    phase_y = 18 * 32 - 8 + 32;
                    break;
            }
        }
    }
    // set the move phase to 0
    move_phase = 0;
    // compute the speed
    speed = static_cast<double>(battler.get_SPD()) / 25;
    intercept_x1 = phase_x - x();
    intercept_y1 = phase_y - y();
    // compute the a1
    a1 = static_cast<double>((intercept_x1*intercept_x1)) / 4.0 / static_cast<double>(intercept_y1);
    //a = static_cast<double>(intercept_y) / 4.0 / static_cast<double>((intercept_x*intercept_x));


    if (minon.get_id() < 16)
    {
        hp_bar = new Bar(64, 10, battler.get_hp(), battler.get_max_hp());
        hp_bar->setParentItem(this);
        hp_bar->setX(-16);
        hp_bar->setY(pixmap().height() / 4);
        hp_bar->setColorType(0);
        hp_bar->value_display = false;
    }
    else
    {
        hp_bar = new Bar(120, 10, battler.get_hp(), battler.get_max_hp());
        hp_bar->setParentItem(this);
        hp_bar->setX(-120 / 4);
        hp_bar->setY(pixmap().height() / 4);
        hp_bar->setColorType(0);
        hp_bar->value_display = false;
    }
    //Emitter *test = new Emitter(0);
    //animation.push_back(test);
}

QRectF SpriteBattler::boundingRect() const
{
    return QRectF(offset().rx(), offset().ry(), move_pic[0].width(), move_pic[0].height());
}

void SpriteBattler::refresh()
{
    // refresh the hp bar;
    hp_bar->refresh(battler.get_hp());
    // count the animation frame count;
    ++anima_count;
    // refresh the animation
    foreach(Emitter*iter, animation)
        iter->refresh();
    /*
    if (battler == NULL)
        return;
    */
    if (battler.judge_hp_zero())
        return;
    if (battler.attacking)
    {
        if (anima_count > 15 - static_cast<double>(battler.get_SPD()) / 5)
        {
            ++pattern_id;
            pattern_id = pattern_id % 4;
            anima_count = 0;
        }
        setPixmap(QPixmap::fromImage(attack_pic[pattern_id]));
    }
    else
    {
        move();
        if (anima_count > 18 - static_cast<double>(battler.get_SPD()) / 10)
        {
            ++pattern_id;
            pattern_id = pattern_id % 4;
            anima_count = 0;
        }
        setPixmap(QPixmap::fromImage(move_pic[pattern_id]));
    }
}
/*
GameMinon SpriteBattler::get_battler() const
{
    return battler;
}
*/
int SpriteBattler::get_std_x() const
{
    return std_x;
}

int SpriteBattler::get_std_y() const
{
    return std_y;
}

int SpriteBattler::get_target_x() const
{
    return target_x;
}

int SpriteBattler::get_target_y() const
{
    return target_y;
}

int SpriteBattler::get_pattern_id() const
{
    return pattern_id;
}

int SpriteBattler::get_anima_count() const
{
    return anima_count;
}

int SpriteBattler::get_move_type() const
{
    return move_type;
}

void SpriteBattler::set_target_x(int x)
{
    target_x = x;
}

void SpriteBattler::set_target_y(int y)
{
    target_y = y;
}

void SpriteBattler::set_move_type(int type)
{
    move_type = type;
}

void SpriteBattler::move()
{
    // if the minon get to the target location the return
    switch(battler.ourside_enemy)
    {
        case true:
            if (x() > target_x)
                return;
            break;
        case false:
            if (x() < target_x)
                return;
            break;
    }
    // if the minon move type is stay
    if(move_type)
        return;
    // if the minon is attacking
    if (battler.attacking)
        return;

    // if the minon is ourside
    if (battler.ourside_enemy)
    {
        // check the move phase and move
        switch(move_phase)
        {
            case 0:
                // if it is going to pass the bridge.
                if(x() > 544)
                    // change the move phase to 1
                    move_phase = 1;
                theta = atan(((2*(intercept_x1-phase_x+x())*(-1)*speed+speed*speed) / 4.0 / a1) / speed);
                moveBy(speed, (-1)*speed * sin(theta));
                break;
            case 1:
                if(x() > 704)
                {
                    intercept_x2 = target_x - x();
                    intercept_y2 = target_y - y();
                    phase_x = x();
                    phase_y = y();
                    // compute the a2
                    a2 = static_cast<double>((intercept_x2*intercept_x2)) / 4.0 / static_cast<double>(intercept_y2);
                    // change the move phase to 2
                    move_phase = 2;
                }
                moveBy(speed, 0);
                break;
            case 2:
                theta = atan(((2*(intercept_x1-phase_x+x())*(-1)*speed+speed*speed) / 4.0 / a2) / speed);
                moveBy(speed + 0.01, (-1)*speed * sin(theta));
                break;
        }
    }
    else
    {
        // check the move phase and move
        switch(move_phase)
        {
            case 0:
                // if it is going to pass the bridge.
                if(x() < 704)
                    // change the move phase to 1
                    move_phase = 1;
                //if ((2*(intercept_x1-phase_x+x())*speed+speed*speed) / 4.0 / a1 < speed * 2)
                theta = atan(((2*(intercept_x1-phase_x+x())*(-1)*speed+speed*speed) / 4.0 / a1) / speed);
                moveBy((-1)*speed * cos(theta), ((2*(intercept_x1-phase_x+x())*(-1)*speed+speed*speed) / 4.0 / a1) > 0 ? speed * sin(theta) : (-1)*speed * sin(theta));
                //else
                    //moveBy((-1)*speed, speed*1.2);
                break;
            case 1:
                if(x() < 544)
                {
                    intercept_x2 = target_x - x();
                    intercept_y2 = target_y - y();
                    phase_x = x();
                    phase_y = y();
                    // compute the a2
                    a2 = static_cast<double>((intercept_x2*intercept_x2)) / 4.0 / static_cast<double>(intercept_y2);
                    // change the move phase to 2
                    move_phase = 2;
                }
                moveBy((-1)*speed, 0);

                break;
            case 2:
                theta = atan(((2*(intercept_x1-phase_x+x())*(-1)*speed+speed*speed) / 4.0 / a2) / speed);
                moveBy((-1)*speed, speed * sin(theta));
                break;
        }
    }
}

SpriteBattler::~SpriteBattler()
{
    delete hp_bar;
    foreach(Emitter* iter, animation)
    {
        delete iter;
    }
    animation.clear();
}
