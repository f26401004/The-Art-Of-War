#ifndef GAMEMINON_H
#define GAMEMINON_H

#include <QString>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <QFile>
#include <sstream>
#include <string>
#include <QDebug>

class GameMinon
{
public:
    GameMinon();
    GameMinon(int);
    void operation_to_HP(int);
    void operation_to_AP(int);
    int get_damage();
    bool judge_hp_zero();
    void attack_effect(GameMinon&);
    void S_attack_effect(GameMinon&);
    void cure_effect(GameMinon&);
    void add_reciever(GameMinon*);
    void set_ATK_plus(bool);
    int get_ATK();
    int get_STR();
    int get_DEX();
    int get_AGI();
    int get_SPD();
    int get_range();
    int get_hp();
    int get_max_hp();
    int get_cost_mana();
    int get_id();
    int get_attack_type();
    QString get_bitmap_string();
    bool attacking = false;
    bool ourside_enemy;
    bool defense = false;

private:
    int id;
    int maxHP;
    int HP;
    int AP;
    int cost_mana;
    static const int maxAP = 100;
    QString bitmap;
    double move_speed;
    int range;
    int ATK;
    int STR;
    int DEF;
    int SPD;
    int DEX;
    int AGI;
    int damage;
    int attack_type;
    bool ATK_plus;



};

#endif // GAMEMINON_H
