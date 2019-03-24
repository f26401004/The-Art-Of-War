#include "gameminon.h"

GameMinon::GameMinon()
{

}

GameMinon::GameMinon(int type)
{
    srand((unsigned)time(NULL));
    id = type;
    attack_type = rand() % 3;
    bitmap = QString::number(type);
    QFile file("./minion.dat");
    file.open(QIODevice::ReadOnly);
    QByteArray byte = QByteArray::fromHex(file.readAll());
    std::stringstream ss(byte.toStdString());
    std::string num,maxhp,hp,atk,str,def,spd,dex,agi,ran,cos;
    qDebug()<<"type = "<<type;
    if(type>14 || type==0) type=14; // if more than 14, then set 14(castle)
    for(int i=1; i<=type; ++i)
    {
        ss>>num>>maxhp>>hp>>atk>>str>>def>>spd>>dex>>agi>>ran>>cos;
    }
    maxHP=atoi(maxhp.c_str());
    HP=atoi(hp.c_str());
    ATK=atoi(atk.c_str());
    STR=atoi(str.c_str());
    DEF=atoi(def.c_str());
    SPD=atoi(spd.c_str());
    DEX=atoi(dex.c_str());
    AGI=atoi(agi.c_str());
    range=atoi(ran.c_str());
    cost_mana=atoi(cos.c_str());
    qDebug()<<maxHP<<" "<<HP<<" "<<ATK<<" "<<STR<<" "<<DEF<<" "<<SPD<<" "<<DEX<<" "<<AGI<<" "<<range<<" "<<cost_mana;
    file.close();
    ATK_plus = 0;
//maxHP  HP ATK STR DEF SPD DEX AGI range cost_mana
//Warrior
//800 800 20 66 54 15 22 23 1 50
//Rifleman
//400 400 30 35 27 28 33 38 8 30
//Nun
//200 200 5 16 33 18 29 35 4 25
//Assassin
//300 300 30 45 22 32 78 68 2 30
//Swordman
//1200 1200 50 71 67 21 31 28 60
//Knight
//850 850 23 60 61 25 31 42 2 40
//Archer
//450 450 23 32 30 31 40 42 6 35
//Witch
//200 200 39 66 18 15 30 21 5 30
//Poet
//250 250 5 14 28 20 30 40 4 25
//Druid
//450 450 35 58 35 30 36 42 6 35
//Laser_blaster
//400 400 45 42 22 15 25 24 10 30
//Robot
//1100 1100 36 45 41 17 33 29 3 40
//UAV
//100 100 20 23 24 42 43 51 5 15
//Castle
//5000 5000 25 52 47 -10 9 8 9 100
}

void GameMinon::operation_to_HP(int value)
{
    HP += value;
}

void GameMinon::operation_to_AP(int value)
{
    AP += value;
}

void GameMinon::attack_effect(GameMinon & attacker)
{
    srand(static_cast<unsigned>(time(NULL)));
    double atk = abs((static_cast<double>(attacker.get_ATK()) - DEF) / 2);
    damage = atk * (20 + static_cast<double>(attacker.get_STR())) / 5;
    damage /= 100;
    if( rand() % 100 < 4 * static_cast<double>(attacker.get_DEX()) / static_cast<double>(attacker.get_AGI()))
        damage *= 1.5;
    int amp = (damage * 15 / 20) > 1 ? rand() % (damage * 15 / 20) : rand() % 2;
    damage += 2 * (amp + 1) - amp;
    HP -= damage + (ATK_plus ? 100 : 0);
}

void GameMinon::cure_effect(GameMinon & attacker)
{
    srand(static_cast<unsigned>(time(NULL)));
    double atk = abs((static_cast<double>(attacker.get_ATK()) - DEF) / 2);
    damage = atk * (20 + static_cast<double>(attacker.get_STR())) / 5;
    damage /= 100;
    if( rand() % 100 < 4 * static_cast<double>(attacker.get_DEX()) / static_cast<double>(attacker.get_AGI()))
        damage *= 1.5;
    int amp = (damage * 15 / 20) > 1 ? rand() % (damage * 15 / 20) : rand() % 2;
    damage += 2 * (amp + 1) - amp;
    HP += damage;
}

void GameMinon::S_attack_effect(GameMinon & attacker)
{
    srand(static_cast<unsigned>(time(NULL)));
    double atk = abs((static_cast<double>(attacker.get_ATK()) - DEF) / 2);
    damage = atk * (20 + static_cast<double>(attacker.get_STR())) / 5;
    damage /= 100;
    if( rand() % 100 < 4 * static_cast<double>(attacker.get_DEX()) / static_cast<double>(attacker.get_AGI()))
        damage *= 1.5;
    int amp = (damage * 15 / 20) > 1 ? rand() % (damage * 15 / 20) : rand() % 2;
    damage += 2 * (amp + 1) - amp;
    HP -= damage * 1.5;
}

int GameMinon::get_damage()
{
    return damage;
}

bool GameMinon::judge_hp_zero()
{
    return (HP < 0 ? true : false);
}

int GameMinon::get_ATK()
{
    return ATK;
}

int GameMinon::get_STR()
{
    return STR;
}

int GameMinon::get_DEX()
{
    return DEX;
}

int GameMinon::get_AGI()
{
    return AGI;
}

int GameMinon::get_SPD()
{
    return SPD;
}


QString GameMinon::get_bitmap_string()
{
    return bitmap;
}

int GameMinon::get_range()
{
    return range;
}

int GameMinon::get_hp()
{
    return HP;
}

int GameMinon::get_max_hp()
{
    return maxHP;
}

int GameMinon::get_cost_mana()
{
    return cost_mana;
}

int GameMinon::get_id()
{
    return id;
}

int GameMinon::get_attack_type()
{
    return attack_type;
}

void GameMinon::set_ATK_plus(bool value)
{
    ATK_plus = value;
}
