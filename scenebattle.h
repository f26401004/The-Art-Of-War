#ifndef SCENEBATTLE_H
#define SCENEBATTLE_H

#include <QObject>
#include <QGraphicsScene>
#include <QPixmap>
#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QPainter>
#include <QKeyEvent>
#include <QTimer>
#include <cstdlib>
#include <QVector>
#include <QSound>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QGraphicsSceneMouseEvent>
#include "flashcommand.h"
#include "enemyai.h"
#include "spritebattler.h"
#include "bar.h"
#include "pibar.h"
#include "gamecontroller.h"
#include "graphicstransition.h"

class GameController;
class SceneBattle : public QGraphicsScene
{
    Q_OBJECT
public:
    SceneBattle(GameController *parent, int = 0, int = 0, int = 0);
    ~SceneBattle();
    int frame_count = 0;
    void set_map_and_level(int, int);
    void set_card(QList<int>);
    void battle_start();
    int get_winner();
    int get_rate(int);
    int get_interval();
    void reset();
    GameController *gc;

signals:
    void transfer_to_result();

private:
    int player_card[3];
    int player_rest_card[2];
    int phase = 0;
    int player_mana = 0;
    int card_index = 0;
    int x_index = 0;
    int y_index = 0;
    int type_index = 0;
    int AI_level = 0;
    int map = 0;
    int camp_type = 0;
    int winner = 0;
    int last_index = 0;
    FlashCommand *card[3];
    FlashCommand *movement_type[2];
    FlashCommand *location_select;
    GraphicsTransition *transition;


    QTimer *counter;
    QGraphicsPixmapItem *time_bar_frame;
    Bar *time_bar;
    QGraphicsTextItem *time_text;

    QGraphicsPixmapItem *mana_bar_frame;
    PiBar *mana_bar;
    QGraphicsTextItem *mana_text;

    void card_active_animation();
    void movement_active_animation();
    void location_active_animation();

    int set_minon(int, int, int, int);
    int set_enemy_minion(int minon_id, int loc_x, int loc_y, int type, int type_ind);


    QList<SpriteBattler*> player_minon, enemy_minon;
    SpriteBattler *player_tower, *enemy_tower;

    void attack_event();
    void animation_event(SpriteBattler *attacker, SpriteBattler *reciever);
    void search_field_in_range(SpriteBattler*);
    bool minon_exist(int , int);
    void hp_event();
    void regain_mana();
    void rand_deal_card();

    EnemyAI *AI;
    QTimer *AI_timer;

    QMediaPlayer *battle_music;
    QMediaPlaylist *music_list;

private slots:
    void refresh();
    void emit_signal();
    //void transfer_to_result();
    //void AI_make_decision();
    void AI_setup();
    void p2p_setup();
    void readCommand();

protected:
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

};

#endif // SCENEBATTLE_H
