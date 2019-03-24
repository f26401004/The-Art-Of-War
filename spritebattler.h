#ifndef SPRITEBATTLER_H
#define SPRITEBATTLER_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QList>
#include <iostream>
#include <ctime>
#include <QDebug>
#include "gameminon.h"
#include "bar.h"
#include "emitter.h"

class SpriteBattler : public QGraphicsPixmapItem
{
public:
    SpriteBattler();
    ~SpriteBattler();
    //SpriteBattler(const SpriteBattler&);
    //SpriteBattler &operator= (const SpriteBattler&);
    //SpriteBattler(SpriteBattler&&);
    //SpriteBattler &operator= (SpriteBattler&&);
    SpriteBattler(int, int, GameMinon, bool, int);
    QImage move_pic[4];
    QImage attack_pic[4];
    GameMinon battler;
    void refresh();
    QRectF boundingRect() const;
    static int frame_count;
    QVector<int> path_x;
    QVector<int> path_y;
    int get_std_x() const;
    int get_std_y() const;
    //GameMinon get_battler() const;
    int get_target_x() const;
    int get_target_y() const;
    int get_pattern_id() const;
    int get_move_type() const;
    int get_anima_count() const;
    void set_target_x(int);
    void set_target_y(int);
    void set_move_type(int);
    QList<Emitter*> animation;
    QList<SpriteBattler*> recievers;

private:
    int pattern_id = 0;
    int std_x;
    int std_y;
    int target_x;
    int target_y;
    int move_type;
    int anima_count;
    int phase_x, phase_y;
    int intercept_x1, intercept_y1, intercept_x2, intercept_y2;
    double a1, a2, theta;
    double speed;
    int move_phase;

    void move();

    Bar *hp_bar;


};

#endif // SPRITEBATTLER_H
