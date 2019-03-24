#include "bar.h"

Bar::Bar(int w, int h, int v, int m) :
    width(w), height(h), value(v), max(m)
{
    setPixmap(QPixmap(w, h + 16));
}

void Bar::refresh(int new_value)
{
    setValue(new_value);
    update();
}

QRectF Bar::boundingRect() const
{
    return QRectF(x(), y(), width, height + 16);
}

void Bar::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
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
    int new_width = width * rate;
    painter->fillRect(x(), y(), width, height, color1);
    painter->fillRect(x() + 1, y() + 1, width - 2, height - 2, color2);
    // paint the content.
    for (double i = 0 ; i < new_width - 4 ; i++)
    {
        int red = color3.red() + (color4.red() - color3.red()) * i / static_cast<double>(width);
        int green = color3.green() + (color4.green() - color3.green()) * i / static_cast<double>(width);
        int blue = color3.blue() + (color4.blue() - color3.blue()) * i / static_cast<double>(width);
        int alpha = color3.alpha() + (color4.alpha() - color3.alpha()) * i / static_cast<double>(width);
        QColor new_color(red, green, blue, alpha);
        painter->fillRect(x() + 2 + i, y() + 2, 1, height - 4, new_color);

    }
    // draw the value.
    if (value_display)
    {
        painter->drawText(x(), y() + height, width, 12, Qt::AlignRight, QString::number(value) + "/" + QString::number(max));
    }
}

void Bar::setValue(int v)
{
    if (v > max)
        return;
    value = v;
    update();
}

void Bar::setColorType(int color)
{
    color_type = color;
}
