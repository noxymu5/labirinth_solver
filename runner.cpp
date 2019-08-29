#include "runner.h"
#include <QDebug>
#include <ctime>

runner::runner(int labSize, labirintCell ***cell, QGraphicsView *gr):grid(cell), grv(gr), labSize(labSize)
{
    size = (qreal)res / (qreal)labSize;

    setPos(mapToParent(-res, -res));
}

QRectF runner::boundingRect() const
{
    return QRect(0, 0, size, size);
}

void runner::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rec = boundingRect();
    QBrush brush(Qt::magenta);
    painter->setPen(Qt::NoPen);

    painter->fillRect(rec, brush);
    painter->drawRect(rec);
}

void runner::solve()
{
    posX = 0;
    posY = 0;
    setPos(-res, -res);
    time = 0;

    switch (currentSolveMethod)
    {
        case 0:
            timer.start();
            solve_Backtracker();
        break;
        case 1:
            timer.start();
            solve_WallFollower();
        break;
        case 2:
            timer.start();
            solve_DeadEndFiller();
        break;
    }
    time = timer.elapsed();
    grv->viewport()->repaint();
}

float runner::getTime()
{
    qDebug() << time;
    return (float)time / (float)1000;

}

void runner::solve_WallFollower()
{
    int direction = 2;

    while((posX != labSize - 1) || (posY != labSize - 1))
    {
        grid[posX][posY]->setColor(Qt::darkMagenta);

        if(!grid[posX][posY]->getWallState(direction))
        {
            if(grid[posX][posY]->getWallState(changeDirection(direction, 2)))
            {
                moveToDirection(direction);
            }
            else
            {
                direction = changeDirection(direction, 2);
                moveToDirection(direction);
            }
        }
        else
        {
            if(!grid[posX][posY]->getWallState(changeDirection(direction, 2)))
            {
                direction = changeDirection(direction, 2);
                moveToDirection(direction);
            }
            else
            {
                if(!grid[posX][posY]->getWallState(changeDirection(direction, 1)))
                {
                    direction = changeDirection(direction, 1);
                    moveToDirection(direction);
                }
                else
                {
                    direction = changeDirection(direction, 3);
                    moveToDirection(direction);
                }
            }
        }
        Redraw(renderOrNot);
    }
}

void runner::solve_Backtracker()
{
    std::vector<QPoint> visitedCells;
    int direction;

    visitedCells.push_back(QPoint(posX, posY));
    while(posX != labSize - 1 || posY != labSize - 1)
    {
        direction = findDirection(false);

        if(direction != -1)
        {
            grid[posX][posY]->setColor(Qt::darkMagenta);
            moveToDirection(direction);
            visitedCells.push_back(QPoint(posX, posY));
        }
        else
        {
            grid[posX][posY]->setColor(Qt::lightGray);
            visitedCells.pop_back();

            QPoint prev = visitedCells.back();
            setPos(mapToParent((prev.x() - posX) * size,(prev.y() - posY) * size));
            posX = prev.x();
            posY = prev.y();
        }
        Redraw(renderOrNot);
    }
}

void runner::solve_DeadEndFiller()
{
    for(int i = 0; i < labSize; ++i)
        for(int j = 0; j < labSize; ++j)
        {
            posX = i;
            posY = j;

            while(!(posX == 0 && posY == 0) && !(posX == labSize - 1 && posY == labSize - 1)
                  && findDirection(true) == 1)
            {
                grid[posX][posY]->setColor(Qt::lightGray);
                moveTo(findDirection(false));
                Redraw(renderOrNot);
            }
        }
}

int runner::findDirection(bool count)
{
    std::vector<int> possibleDirections;

    for(int i = 0; i < 4; i++)
    {
        if(!grid[posX][posY]->getWallState(i))
        {
            int checkX = posX + (i % 2 == 0 ? 0 : -1 * (i - 2));
            int checkY = posY + (i % 2 == 0 ? (i - 1) : 0);
            if(grid[checkX][checkY]->checkColor() != Qt::darkMagenta &&
                    grid[checkX][checkY]->checkColor() != Qt::lightGray)
                possibleDirections.push_back(i);
        }
    }
    if(possibleDirections.size() == 0)
        return -1;
    else
    {
        if(count)
            return possibleDirections.size();
        else
            return possibleDirections[qrand() % possibleDirections.size()];
    }
}

void runner::moveTo(int dir)
{
    posX += dir % 2 == 0 ? 0 : -1 * (dir - 2);
    posY += dir % 2 == 0 ? (dir - 1) : 0;
}

void runner::Redraw(bool render)
{
    if(render && globalRender)
        grv->viewport()->repaint();
}


int runner::changeDirection(int direction, int where)
{
    //1 - to left
    //2 - to right
    //3 - reverse

    switch(where)
    {
        case 1:
            if(direction == 0)
                return 3;
            else
                return direction - 1;
        break;
        case 2:
            if(direction == 3)
                return 0;
            else
                return direction + 1;
        break;
        case 3:
            switch (direction)
            {
                case 0:
                    return 2;
                break;
                case 2:
                    return 0;
                break;
                case 1:
                    return 3;
                break;
                case 3:
                    return 1;
                break;
            }
        break;
    }
    return -1;
}

void runner::moveToDirection(int moveDirecrion)
{
    posX += moveDirecrion % 2 == 0 ? 0 : -1 * (moveDirecrion - 2);
    posY += moveDirecrion % 2 == 0 ? (moveDirecrion - 1) : 0;
    setPos(mapToScene(moveDirecrion % 2 == 0 ? 0 : -1 * (moveDirecrion - 2) * size,
                      moveDirecrion % 2 == 0 ? (moveDirecrion - 1) * size : 0));
}


void runner::advance(int phase)
{
    if(!phase) return;
}

