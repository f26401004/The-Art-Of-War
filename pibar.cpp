#include "pibar.h"

PiBar::PiBar(int r, int v, int m) :
    radius(r), value(v), max(m)
{
    setPixmap(QPixmap(2 * r, 2 * r + 16));
}

void PiBar::refresh(int new_value)
{
    //std::cout << new_value << std::endl;
    setValue(new_value);
    update();
}

QRectF PiBar::boundingRect() const
{
    return QRect(x(), y(), 2 * radius, 2 * radius + 16);
}

void PiBar::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    double rate = static_cast<double>(value) / static_cast<double>(max);
    QColor color1(45, 40, 40, 255), color2(255, 255, 255, 255),
            color3(0, 0, 0, 255), color4(0, 0, 0, 255);
    switch(color_type)
    {
        // red
        case 0 :
            color3.setRed(180 - 24 * rate);
            color3.setGreen(14 * rate);
            color3.setBlue(14 * rate);
            color4.setRed(240 - 72 * rate);
            color4.setGreen(62 * rate);
            color4.setBlue(96 * rate);
            break;
        // green
        case 1 :
            color3.setRed(14 * rate);
            color3.setGreen(180 - 14 * rate);
            color3.setBlue(14 * rate);
            color4.setRed(62 * rate);
            color4.setGreen(240 - 72 * rate);
            color4.setBlue(96 * rate);
            break;
        // blue
        case 2 :
            color3.setRed(14 * rate);
            color3.setGreen(14 * rate);
            color3.setBlue(180 - 14 * rate);
            color4.setRed(62 * rate);
            color4.setGreen(96 * rate);
            color4.setBlue(240 - 72 * rate);
            break;
        // time
        case 3 :
            color3.setRed(105 * rate);
            color3.setGreen(186 - 14 * rate);
            color3.setBlue(97 * rate);
            color4.setRed(79 * rate);
            color4.setGreen(234 - 72 * rate);
            color4.setBlue(203 * rate);
            break;
        default :
            color3.setRed(180 - 24 * rate);
            color3.setGreen(14 * rate);
            color3.setBlue(14 * rate);
            color4.setRed(62 * rate);
            color4.setGreen(96 * rate);
            color4.setBlue(240 - 72 * rate);
            break;
    }
    // paint the frame.
    int new_height = 2 * radius * rate;
    painter->setPen(color1);
    painter->drawEllipse(x(), y(), 2 * radius, 2 * radius);
    painter->setPen(color2);
    painter->drawEllipse(x() + 1, y() +1, 2 * radius - 2, 2 * radius - 2);
    /*
    painter->fillRect(x(), y(), width, height, color1);
    painter->fillRect(x() + 1, y() + 1, width - 2, height - 2, color2);
    */
    // paint the content.
    for (double i = 0 ; i < new_height - 2 ; i++)
    {
        int red = color3.red() + (color4.red() - color3.red()) * i / static_cast<double>(2 * radius);
        int green = color3.green() + (color4.green() - color3.green()) * i / static_cast<double>(2 * radius);
        int blue = color3.blue() + (color4.blue() - color3.blue()) * i / static_cast<double>(2 * radius);
        int alpha = color3.alpha() + (color4.alpha() - color3.alpha()) * i / static_cast<double>(2 * radius);
        QColor new_color(red, green, blue, alpha);
        int width = (i < radius-1) ? sqrt((radius-1)*(radius-1) - (radius-i-1) * (radius-i-1)) :
                                        sqrt((radius-1)*(radius-1) - (i-radius+1) * (i-radius+1));
        int current_x = x() + radius - (width-1);
        int current_y = y() + 2 * radius - i - 1;
        painter->fillRect(current_x, current_y, 2 * (width-1), 1, new_color);
    }
    // draw the value.
    if (value_display)
    {
        painter->drawText(x(), y() + radius / 2, 2 * radius, 12, Qt::AlignCenter, QString::number(value) + "/" + QString::number(max));
    }
}

void PiBar::setValue(int v)
{
    if (v > max)
        return;
    value = v;
    update();
}

void PiBar::setColorType(int color)
{
    color_type = color;
}
