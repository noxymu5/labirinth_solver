#include "labirintcell.h"
#include <QDebug>

labirintCell::labirintCell(int labSize, qreal posX, qreal posY)
{
    size = (qreal)res / (qreal)labSize;

    topWall = QLineF(0, 0, size, 0);
    bottomWall = QLineF(0, size, size, size);
    leftWall = QLineF(0, 0, 0, size);
    rightWall = QLineF(size, 0, size, size);
    cellColor = Qt::white;

    setPos(mapToParent(posX - res, posY - res));
}

QRectF labirintCell::boundingRect() const
{
    return QRectF(0, 0, size, size);
}

void labirintCell::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    colorBorder = boundingRect();

    painter->setPen(cellColor);
    QBrush brush(cellColor);
    painter->fillRect(colorBorder, brush);
    painter->drawRect(colorBorder);

    painter->setPen(Qt::NoPen);

    painter->setPen(Qt::black);

    if(walls[0])
        painter->drawLine(topWall);
    if(walls[1])
        painter->drawLine(rightWall);
    if(walls[2])
        painter->drawLine(bottomWall);
    if(walls[3])
        painter->drawLine(leftWall);
}

void labirintCell::setWall(int wall, bool existance)
{
    walls[wall] = existance;
}

bool labirintCell::getWallState(int wall)
{
    return walls[wall];
}

void labirintCell::setColor(QColor newColor)
{
    cellColor = newColor;
}

QColor labirintCell::checkColor()
{
    return cellColor;
}

void labirintCell::advance(int phase)
{
    if(!phase) return;
}
