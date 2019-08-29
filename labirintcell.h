#ifndef LABIRINTCELL_H
#define LABIRINTCELL_H

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>

class labirintCell: public QGraphicsItem
{
public:
    labirintCell(int labSize, qreal posX, qreal posY);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    void setWall(int wall, bool existance);
    bool getWallState(int wall);
    void setColor(QColor newColor);
    QColor checkColor();

protected:
    void advance(int phase);

private:
    int res = 500;
    qreal size;
    QColor cellColor;
    bool walls[4] = {false, false, false, false};
    QLineF topWall;
    QLineF rightWall;
    QLineF bottomWall;
    QLineF leftWall;
    QRectF colorBorder;
};

#endif // LABIRINTCELL_H
