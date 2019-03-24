#ifndef PARTICLE_H
#define PARTICLE_H

#include <QGraphicsPixmapItem>
#include <QGraphicsBlurEffect>
#include <QGraphicsColorizeEffect>
#include <QDebug>
#include <ctime>

class Particle
{
public:
    Particle();
    ~Particle();
    void refresh();
    void add_velocity(double, double);
    void set_type(int);
    int get_type();
    void operate_to_span(int);
    int get_life_span();
    bool is_dead();
    int x();
    int y();
    void setPos(int, int);
    void setOpacity(double);
    void setRotation(double);
    void setScale(double);
    double scale();
    double opacity();
    double rotation();
    QPixmap pixmap();


private:
    int type = 0;
    int pos_x = 0;
    int pos_y = 0;
    int life_span = 0;
    int max_life_span = 0;
    double x_velocity = 0;
    double y_velocity = 0;
    double pic_opacity = 1;
    double pic_scale = 1;
    double pic_rotation = 0;
    QPixmap pix;
    bool dead = false;
    void property_refresh();



};

#endif // PARTICLE_H
