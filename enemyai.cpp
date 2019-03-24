#include "enemyai.h"

EnemyAI::EnemyAI(int level, int camp) :
    AI_level(level),
    camp_type(camp)
{
    // rand the card
    for (int i = 0 ; i < 3 ; ++i)
        card[i] = rand() % 5 + camp_type * 5 + 1;
    // initialize nightmare set
    nightmare_set = 0;

}

QRectF EnemyAI::boundingRect() const
{

}
void EnemyAI::paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *)
{

}

void EnemyAI::rand_deal_card(int card_index)
{
    srand((unsigned)time(NULL));
    // find the rest card
    int rand_card[2] = {0}, n = 0;
    for(int i = camp_type * 5 + 1 ; i < camp_type * 5 + 6 ; ++i)
    {
        if (i != card[0] && i != card[1] && i != card[2])
        {
            rand_card[n] = i;
            ++n;
        }
    }
    // change the card.
    card[card_index] = rand_card[rand() % 2] ;
}

void EnemyAI::set_minon(int id, int loc, int type, int index)
{
    GameMinon minon(id);
    SpriteBattler *sprite;
    switch(loc)
    {
        case 0:
            sprite = new SpriteBattler(rand() % 4 + 25, rand() % 3 + 2, minon, false, type);
            break;
        case 1:
            sprite = new SpriteBattler(rand() % 4 + 25, rand() % 3 + 16, minon, false, type);
            break;
    }
    enemy_minon.push_back(sprite);
    mana -= minon.get_cost_mana();
    scene()->addItem(sprite);
    rand_deal_card(index);
    ++number_minon;
}

void EnemyAI::AI_make_decision()
{
    switch(3 - AI_level)
    {
        // easy
        case 3:
            easy_decision();
            break;
        // medium
        case 2:
            hard_decision();
            break;
        // hard
        case 1:
            nightmare_decision();
            break;
    }
}


void EnemyAI::easy_decision()
{
    switch(camp_type)
    {
        case 0:
            for (int i = 0 ; i < 3 ; ++i)
            {
                switch(card[i])
                {
                    case 1:
                        if (mana > 65)
                            set_minon(1, 0, 0, i);
                        break;
                    case 2:
                        if (mana > 40)
                            set_minon(2, 0, 0, i);
                        break;
                    case 3:
                        if (mana > 30)
                            set_minon(3, 0, 0, i);
                        break;
                    case 4:
                        if (mana > 30)
                            set_minon(4, 1, 0, i);
                        break;
                    case 5:
                        if (mana > 70)
                            set_minon(5, 1, 0, i);
                        break;
                }
            }
            break;
        case 1:
            break;
        case 2:
            break;
    }
}

void EnemyAI::hard_decision()
{
    switch(camp_type)
    {
        case 0:
            for (int i = 0 ; i < 3 ; ++i)
            {
                if (card[i] == 1 && mana > 65)
                    set_minon(1, 0, 0, i);
                else if(card[i] == 5 && mana > 70)
                    set_minon(5, 0, 0, i);
            }
            for (int i = 0 ; i < 3 ; ++i)
            {
                if (card[i] == 2 && mana > 40)
                    set_minon(2, 0, 0, i);
                else if(card[i] == 4 && mana > 40)
                    set_minon(4, 1, 0, i);
            }
            for (int i = 0 ; i < 3 ; ++i)
            {
                if (card[i] == 3 && mana > 30)
                    set_minon(3, 0, 0, i);
            }
            break;
        case 1:
            break;
        case 2:
            break;
    }
}


void EnemyAI::nightmare_decision()
{
    switch(camp_type)
    {
        case 0:
            for (int i = 0 ; i < 3 ; ++i)
            {
                if (card[i] == 1 && mana > 65)
                {
                    GameMinon minon(1);
                    SpriteBattler *sprite;
                    if (nightmare_set)
                         sprite = new SpriteBattler(rand() % 5 + 22, rand() % 3 + 2, minon, false, 0);
                    else
                         sprite = new SpriteBattler(rand() % 5 + 22, rand() % 3 + 16, minon, false, 0);
                    enemy_minon.push_back(sprite);
                    mana -= minon.get_cost_mana();
                    scene()->addItem(sprite);
                    rand_deal_card(i);
                    nightmare_set = 1 - nightmare_set;
                }
                else if(card[i] == 5 && mana > 70)
                {
                    GameMinon minon(5);
                    SpriteBattler *sprite;
                    if (nightmare_set)
                         sprite = new SpriteBattler(rand() % 5 + 22, rand() % 3 + 2, minon, false, 0);
                    else
                         sprite = new SpriteBattler(rand() % 5 + 22, rand() % 3 + 16, minon, false, 0);
                    enemy_minon.push_back(sprite);
                    mana -= minon.get_cost_mana();
                    scene()->addItem(sprite);
                    rand_deal_card(i);
                    nightmare_set = 1 - nightmare_set;
                }
            }
            for (int i = 0 ; i < 3 ; ++i)
            {
                if (card[i] == 2 && mana > 40)
                {
                    GameMinon minon(2);
                    SpriteBattler *sprite;
                    if (nightmare_set)
                         sprite = new SpriteBattler(rand() % 5 + 22, rand() % 3 + 2, minon, false, 0);
                    else
                         sprite = new SpriteBattler(rand() % 5 + 22, rand() % 3 + 16, minon, false, 0);
                    enemy_minon.push_back(sprite);
                    mana -= minon.get_cost_mana();
                    scene()->addItem(sprite);
                    rand_deal_card(i);
                    nightmare_set = 1 - nightmare_set;
                }
                else if(card[i] == 4 && mana > 40)
                {
                    GameMinon minon(4);
                    SpriteBattler *sprite;
                    if (nightmare_set)
                         sprite = new SpriteBattler(rand() % 5 + 22, rand() % 3 + 2, minon, false, 0);
                    else
                         sprite = new SpriteBattler(rand() % 5 + 22, rand() % 3 + 16, minon, false, 0);
                    enemy_minon.push_back(sprite);
                    mana -= minon.get_cost_mana();
                    scene()->addItem(sprite);
                    rand_deal_card(i);
                    nightmare_set = 1 - nightmare_set;
                }
            }
            for (int i = 0 ; i < 3 ; ++i)
            {

                if (card[i] == 3 && mana > 30)
                {
                    GameMinon minon(3);
                    SpriteBattler *sprite;
                    if (nightmare_set)
                         sprite = new SpriteBattler(rand() % 5 + 22, rand() % 3 + 2, minon, false, 0);
                    else
                         sprite = new SpriteBattler(rand() % 5 + 22, rand() % 3 + 16, minon, false, 0);
                    enemy_minon.push_back(sprite);
                    mana -= minon.get_cost_mana();
                    scene()->addItem(sprite);
                    rand_deal_card(i);
                    nightmare_set = 1 - nightmare_set;
                }
            }
            break;
        case 1:
            break;
        case 2:
            break;
    }
}

void EnemyAI::reset()
{
    for (int i = 0 ; i < 3 ; ++i)
        card[i] = rand() % 5 + camp_type * 5 + 1;
    nightmare_set = 0;
    number_minon = 0;
}

EnemyAI::~EnemyAI()
{
    foreach(SpriteBattler* iter, enemy_minon)
        delete iter;
}
