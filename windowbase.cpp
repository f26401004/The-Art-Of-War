#include "windowbase.h"

WindowBase::WindowBase()
{

}

WindowBase::WindowBase(int x, int y, int width, int height)
{

}

QRectF WindowBase::boundingRect() const
{
    return pixmap().rect();
}

void WindowBase::update()
{

}

void WindowBase::draw_content()
{

}
