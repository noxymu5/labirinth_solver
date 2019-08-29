#ifndef RUNNER_H
#define RUNNER_H

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QElapsedTimer>
#include <vector>
#include <list>
#include <algorithm>
#include "labirintcell.h"

class runner: public QGraphicsObject
{
    Q_OBJECT

public:
    runner(int labSize, labirintCell ***cell, QGraphicsView* gr);
    QRectF boundingRect() const;
    int currentSolveMethod = 0;
    bool renderOrNot = false;
    bool globalRender = true;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void solve();
    float getTime();

protected:
    void advance(int phase);

private:
    qreal res = 500;
    labirintCell ***grid;
    QGraphicsView *grv;
    int labSize;
    int posX = 0;
    int posY = 0;
    QRectF *body;
    qreal size;
    QElapsedTimer timer;
    qint64 time;

    void solve_WallFollower();
    void solve_Backtracker();
    void solve_DeadEndFiller();

    int changeDirection(int direction, int where);
    void moveToDirection(int moveDirecrion);
    int findDirection(bool count);
    void moveTo(int dir);
    void Redraw(bool render);
};

#endif // RUNNER_H
