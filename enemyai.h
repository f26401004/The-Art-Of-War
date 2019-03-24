#ifndef ENEMYAI_H
#define ENEMYAI_H

#include <QObject>
#include <QGraphicsScene>
#include <QTimer>
#include <QRectF>
#include "spritebattler.h"
#include "gameminon.h"

class EnemyAI : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    EnemyAI(int, int);
    ~EnemyAI();
    QList<SpriteBattler*> enemy_minon;
    int mana = 0;
    QRectF boundingRect() const;
    void reset();

private:
    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);

    int card[3] = {0};
    int consider_time;
    int AI_level;
    int camp_type;
    int nightmare_set;
    int number_minon = 0;

    void set_minon(int, int, int, int);
    void rand_deal_card(int);

    void easy_decision();
    void hard_decision();
    void nightmare_decision();
private slots:
    void AI_make_decision();

};

#endif // EnemyAI_H
