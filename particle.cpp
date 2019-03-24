#include "particle.h"

Particle::Particle()
{

}

void Particle::set_type(int x)
{
    type = x;
    // set the life span of each particle.
    switch(type)
    {
    // cure
    case 0:
        life_span = 60;
        max_life_span = 60;
        pix = QPixmap(":/Graphics/Animation/cure.png");
        break;
    // flash
    case 1:
        life_span = 30;
        max_life_span = 30;
        pix = QPixmap(":/Graphics/Animation/flash.png");
        break;
    // bullet
    case 2:
        life_span = 15;
        max_life_span = 15;
        pix = QPixmap(":/Graphics/Animation/flame.png");
        break;
    // flame
    case 3:
        life_span = 120;
        max_life_span = 120;
        pix = QPixmap(":/Graphics/Animation/flame.png");
        break;
    // ice
    case 4:
        life_span = 80;
        max_life_span = 80;
        pix = QPixmap(":/Graphics/Animation/ice.png");
        break;
    // lightning
    case 5:
        life_span = 15;
        max_life_span = 15;
        pix = QPixmap(":/Graphics/Animation/lightning.png");
        break;
    // light point
    case 6:
        life_span = 120;
        max_life_span = 120;
        pix = QPixmap(":/Graphics/Animation/cure.png");
        break;
    // light explosion
    case 7:
        life_span = 120;
        max_life_span = 120;
        pix = QPixmap(":/Graphics/light_point.png");
        break;
    // lightning explosion
    case 8:
        life_span = 15;
        max_life_span = 15;
        pix = QPixmap(":/Graphics/Animation/lightning.png");
        break;
    // ice air
    case 9:
        life_span = 60;
        max_life_span = 60;
        pix = QPixmap(":/Graphics/Animation/ice_air.png");
        break;
    // snow_flake
    case 10:
        life_span = 60;
        max_life_span = 60;
        pix = QPixmap(":/Graphics/Animation/snow_flake.png");
        break;
    // flame flake 1
    case 11:
        life_span = 120;
        max_life_span = 120;
        pix = QPixmap(":/Graphics/Animation/flame_light.png");
        break;
    // flame flake 2
    case 12:
        life_span = 120;
        max_life_span = 120;
        pix = QPixmap(":/Graphics/Animation/flame_light.png");
        break;
    }
}

void Particle::property_refresh()
{
    double t = static_cast<double>(life_span) / max_life_span;
    switch(type)
    {
    case 0:
        x_velocity = x_velocity + x_velocity / 1000;
        y_velocity = y_velocity + y_velocity / 100;
        pic_rotation = pic_rotation + x_velocity / 5;
        pic_opacity = sin(3.14159*t);
        pic_scale = 0.1*cos(3.14159*t) + 0.9;
        //blur_effect->setBlurRadius(3 + 2*static_cast<double>(max_life_span - life_span) / max_life_span);
        break;
    case 1:
        y_velocity = y_velocity - 2;
        pic_rotation = 45;
        pic_scale = sin(3.14159*t) + 1;
        pic_opacity = pic_opacity - 0.1;
        break;
    case 2:
        y_velocity = y_velocity + 0.5*y_velocity*cos(3.14159*t);
        pic_rotation = pic_rotation + 2;
        pic_scale = pic_scale + t * 0.02;
        pic_opacity = pic_opacity - 0.07;
        break;
    case 3:
        //pic_rotation = pic_rotation + 0.1;
        x_velocity = x_velocity + x_velocity / 1000*cos(3.14159*t);
        //x_velocity = x_velocity + x_velocity / 50;//x_velocity + x_velocity / 50;
        y_velocity = y_velocity + y_velocity / 100;
        //if (t > 0.5)
        pic_scale = t / 1000 + pic_scale - 0.001;
        if (abs(pic_rotation) < 45)
            pic_rotation = pic_rotation + x_velocity;
        else
        {
            pic_rotation = abs(pic_rotation) - 45;
            x_velocity = -x_velocity;
        }
        pic_scale = 1.5*sin(3.14159*(t/2));
        pix = pix.scaled(18, 48*t);
        pic_opacity = t - t / 3;
        break;
    case 4:
        if (t > 0.5)
        {
            pic_rotation = pic_rotation + 6;
            x_velocity = x_velocity + 0.001;
            pic_scale = (-1)*cos(3.14159*(t));
            pix = QPixmap(":/Graphics/Animation/snow_flake.png");
        }
        else
        {
            pos_x = 15;
            pos_y = 15;
            x_velocity = -0.1;
            y_velocity = -0.1;
            pic_rotation = pic_rotation + 1;//static_cast<double>(10*cos(3.14159*(t+0.4)));
            pic_scale = 1*sin(3.14159*(t+0.2)) + 1.2;
            pic_opacity = sin(3.14159*t);
            pix = QPixmap(":/Graphics/Animation/ice.png");
        }
        break;
    case 5:
        pic_opacity =  pic_opacity - 0.07;
        pic_scale = pic_scale + 0.01;
        break;
    case 6:
        t -= 4;
        x_velocity = x_velocity + 0.01*t*t*t*cos(3.14159*t);
        y_velocity = y_velocity + 0.01*t*t*t*sin(3.14159*t) - 0.5;
        pic_scale = pic_scale + 0.01;
        //add_velocity(0.1, 0.1);
        //setRotation(rotation() + static_cast<double>(x_velocity)/y_velocity);
        pic_opacity =  pic_opacity - 0.02;
        break;
    case 7:
        y_velocity = y_velocity + y_velocity / 25;
        pic_scale = 3*sin(3.14159*t) + 1;
        pic_opacity = t - t / 2;
        pic_rotation = pic_rotation + static_cast<double>(10*cos(3.14159*t)/y_velocity);
        break;
    case 8:
        pic_opacity =  pic_opacity - 0.07;
        pic_scale = pic_scale + 0.01;
        break;
    case 9:
        if (t > 0.5)
        {
            pic_rotation = pic_rotation + 6;
            x_velocity = x_velocity + 0.01;
            //y_velocity = y_velocity + 1;//0.8*sin(3.14159*t);
            pic_scale = 0.5*sin(3.14159*(t - 0.5)) + 0.6;
            pic_opacity = sin(3.14159*t);
        }
        else
        {
            x_velocity = static_cast<double>(pos_x) / 5;
            y_velocity = x_velocity;
            pic_rotation = pic_rotation + 1;//static_cast<double>(10*cos(3.14159*(t+0.4)));
            pic_scale = 1.5*sin(3.14159*t);
            pic_opacity = pic_opacity - 0.04;//sin(3.14159*t);
        }
        break;
    case 10:
        if (t > 0.5)
        {
            pic_rotation = pic_rotation + 6;
            pic_scale = sin(3.14159*t);
            pic_opacity = 0;
        }
        else
        {
            x_velocity = x_velocity + 0.001;
            pic_rotation = pic_rotation + 6;
            //pic_rotation = pic_rotation + 10;//static_cast<double>(10*cos(3.14159*(t+0.4)));
            pic_scale = cos(3.14159*(t));
            pic_opacity = sin(3.14159*t);
        }
        break;
    case 11:
        pic_rotation = pic_rotation + static_cast<double>(360)/max_life_span/4;
        pos_y = pos_y + rand() % 10 - 5;
        pic_scale = pic_scale + 0.0001;
        pic_opacity = t - t / 2;
        break;
    case 12:
        pic_rotation = pic_rotation - static_cast<double>(360)/max_life_span/4;
        pos_y = pos_y + rand() % 10 - 5;
        pic_scale = pic_scale + 0.0001;
        pic_opacity = t - t/2;
        break;
    }
}

void Particle::refresh()
{
    pos_x = pos_x + x_velocity;
    pos_y = pos_y + y_velocity;
    life_span = life_span - 1;
    property_refresh();
    if (pos_x < -1280 || pos_x > 1280)
        dead = true;
    if (pos_y < -720 || pos_y > 720)
        dead = true;
    if (opacity() <= 0 && life_span < 0)
        dead = true;
}

int Particle::get_type()
{
    return type;
}

bool Particle::is_dead()
{
    return dead;
}

int Particle::get_life_span()
{
    return life_span;
}

int Particle::x()
{
    return pos_x;
}

int Particle::y()
{
    return pos_y;
}

double Particle::scale()
{
    return pic_scale;
}

QPixmap Particle::pixmap()
{
    return pix;
}

double Particle::opacity()
{
    return pic_opacity;
}

double Particle::rotation()
{
    return pic_rotation;
}

void Particle::setPos(int value_x, int value_y)
{
    pos_x = value_x;
    pos_y = value_y;
}

void Particle::setOpacity(double value)
{
    pic_opacity = value;
}

void Particle::add_velocity(double dax, double day)
{
    x_velocity += dax;
    y_velocity += day;
}

void Particle::operate_to_span(int value)
{
    life_span += value;
}

void Particle::setRotation(double value)
{
    pic_rotation = value;
}

void Particle::setScale(double value)
{
    pic_scale = value;
}

Particle::~Particle()
{
}
