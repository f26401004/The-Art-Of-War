#include "graphicstransition.h"

GraphicsTransition::GraphicsTransition(qreal x, qreal y, qreal width, qreal height) :
    QGraphicsRectItem(x, y, width, height)
{
    // initialize the black screen
    setBrush(QBrush(QColor(0, 0, 0)));
    setOpacity(0);
    setZValue(9999);
    // initialize the state.
    in = false;
    out = false;
}

void GraphicsTransition::update()
{
    // if in bool is open
    if (in)
    {
        // Transparency drops
        setOpacity(opacity() - 0.05);
        if (opacity() == 0)
            in = false;
    }
    else if (out)
    {
        // Transparency rises
        setOpacity(opacity() + 0.05);
        if (opacity() == 1)
            out = false;
    }
}

void GraphicsTransition::start_in()
{
    in = true;
}

void GraphicsTransition::start_out()
{
    out = true;
}

bool GraphicsTransition::get_in()
{
    return in;
}

bool GraphicsTransition::get_out()
{
    return out;
}
