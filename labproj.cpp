#include "labproj.h"
#include "ui_labproj.h"
#include <QDebug>
#include <QElapsedTimer>
#include <ctime>

LabProj::LabProj(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LabProj)
{
    ui->setupUi(this);
    srand(time(0));

    cell = 0;
    cell2 = 0;
    renderOrNot = false;
    globalRender = true;
    rnG.seed(time(0));

    labScene = new QGraphicsScene(this);
    labScene2 = new QGraphicsScene(this);

    ui->labField->setScene(labScene);
    ui->labField2->setScene(labScene2);

    ui->labField->setRenderHint(QPainter::Antialiasing);
    ui->labField2->setRenderHint(QPainter::Antialiasing);

    labScene->setSceneRect(-res, -res, res, res);
    labScene2->setSceneRect(-res, -res, res, res);
}

LabProj::~LabProj()
{
    delete ui;
}

void LabProj::on_labBuildButton_clicked()
{
    fieldSize = ui->labSizeField->text().toInt();

    if(fieldSize <= 2)
    {
        QMessageBox error;
        error.setText("Please enter value >= 3");
        error.exec();
        return;
    }
    else
        if(fieldSize > 40)
        {
            QMessageBox error;
            error.setText("Слишком большой размер лабиринта.\nПостроение и решение лабиринта такого размера будет "
                          "производится слишком долго. В связи с этим пошаговая отрисовка будет отключена.");
            error.exec();
            renderOrNot = false;
        }
        else
        {
            renderOrNot = true;
        }

    ui->checkBox->setEnabled(true);
    ui->lab1algList->setEnabled(true);
    ui->lab2AlgList->setEnabled(true);
    ui->solve1->setEnabled(true);
    ui->solve2->setEnabled(true);

    labScene->clear();
    labScene2->clear();
    ui->cellPer1->clear();
    ui->lab1Time->clear();
    ui->cellPer2->clear();
    ui->lab2Time->clear();

    qreal cellSize = (qreal)res / (qreal)fieldSize;


    cell = new labirintCell**[fieldSize];
    cell2 = new labirintCell**[fieldSize];

    for(int i = 0; i < fieldSize; i++)
    {
        cell[i] = new labirintCell*[fieldSize];
        cell2[i] = new labirintCell*[fieldSize];
    }

    for(int i = 0; i < fieldSize; i++)
        for(int j = 0; j < fieldSize; j++)
        {
            cell[i][j] = new labirintCell(fieldSize, i * cellSize, j * cellSize);
            cell2[i][j] = new labirintCell(fieldSize, i * cellSize, j * cellSize);
            labScene->addItem(cell[i][j]);
            labScene2->addItem(cell2[i][j]);

            if(i == 0)
            {
                cell[i][j]->setWall(3, true);
                cell2[i][j]->setWall(3, true);
            }
            if(i == fieldSize - 1)
            {
                cell[i][j]->setWall(1, true);
                cell2[i][j]->setWall(1, true);
            }
            if(j == 0)
            {
                cell[i][j]->setWall(0, true);
                cell2[i][j]->setWall(0, true);
            }
            if(j == fieldSize - 1)
            {
                cell[i][j]->setWall(2, true);
                cell2[i][j]->setWall(2, true);
            }
        }

    Split(0, 0, fieldSize, fieldSize, GetRandom());

    runObj = new runner(fieldSize, cell, ui->labField);
    runObj->currentSolveMethod = ui->lab1algList->currentIndex();
    runObj->globalRender = ui->checkBox->isChecked();
    runObj2 = new runner(fieldSize, cell2, ui->labField2);
    runObj2->currentSolveMethod = ui->lab2AlgList->currentIndex();
    runObj2->globalRender = ui->checkBox->isChecked();

    if(ui->labSizeField->text().toInt() > 40)
    {
        runObj->renderOrNot = false;
        runObj2->renderOrNot = false;
    }
    else
    {
        runObj->renderOrNot = true;
        runObj2->renderOrNot = true;
    }

    labScene->addItem(runObj);
    labScene2->addItem(runObj2);
}

void LabProj::Split(qreal x, qreal y, int width, int height, int orientation)
{
    if(width < 2 || height < 2) return;
    //true - horizontal
    //false - vertical

    int splitX;
    int splitY;

    if(orientation)
    {
        splitY = y + rand() % (height - 1);
        splitX = x;

        for(int i = x; i < x + width; i++)
        {
            cell[i][splitY]->setWall(2, true);
            cell[i][splitY + 1]->setWall(0, true);

            cell2[i][splitY]->setWall(2, true);
            cell2[i][splitY + 1]->setWall(0, true);
        }
        Redraw(renderOrNot);


        int passX = x + rand() % (width - 1);
        cell[passX][splitY]->setWall(2, false);
        cell[passX][splitY + 1]->setWall(0, false);

        cell2[passX][splitY]->setWall(2, false);
        cell2[passX][splitY + 1]->setWall(0, false);

    }
    else
    {
        splitY = y;
        splitX = x + rand() % (width - 1);

        for(int i = y; i < y + height; i++)
        {
            cell[splitX][i]->setWall(1, true);
            cell[splitX + 1][i]->setWall(3, true);

            cell2[splitX][i]->setWall(1, true);
            cell2[splitX + 1][i]->setWall(3, true);
        }
        Redraw(renderOrNot);

        int passY = y + rand() % (height - 1);
        cell[splitX][passY]->setWall(1, false);
        cell[splitX + 1][passY]->setWall(3, false);

        cell2[splitX][passY]->setWall(1, false);
        cell2[splitX + 1][passY]->setWall(3, false);
    }

    Redraw(renderOrNot);

    int newX, newY, newH, newW;
    newX = x;
    newY = y;
    newW = orientation ? width : splitX - x + 1;
    newH = orientation ? splitY - y + 1 : height;
    Split(newX, newY, newW, newH, GetRandom());

    newX = orientation ? x : splitX + 1;
    newY = orientation ? splitY + 1 : y;
    newW = orientation ? width : x + width - splitX - 1;
    newH = orientation ? y + height - splitY - 1 : height;
    Split(newX, newY, newW, newH, GetRandom());
}

int LabProj::GetRandom()
{
    if(hist.size() != 2)
    {
        hist.push_back(rand() % 2);
        return hist.back();
    }
    else
    {
        bool random = rand() % 2;

        if(hist.back() == random && hist.front() == random)
            random = !random;

        hist.pop_front();
        hist.push_back((int)random);

        return random;
    }
}

void LabProj::Redraw(bool render)
{
    if(render && globalRender)
    {
        ui->labField->viewport()->repaint();
        ui->labField2->viewport()->repaint();
    }
    else
        return;
}

float LabProj::VisitedCells(labirintCell ***grid)
{
    float count = 0;
    for(int i = 0 ; i < fieldSize; ++i)
        for(int j = 0 ; j < fieldSize; ++j)
            if(grid[i][j]->checkColor() == Qt::darkMagenta || grid[i][j]->checkColor() == Qt::lightGray)
                ++count;

    return count;
}

void LabProj::DropColor()
{
    for(int i = 0; i < fieldSize; i++)
        for(int j = 0; j < fieldSize; j++)
        {
            if(i != fieldSize && j != fieldSize)
            {
                cell[i][j]->setColor(Qt::white);
                cell2[i][j]->setColor(Qt::white);
            }
        }
}

void LabProj::on_lab1algList_currentIndexChanged(int index)
{
    runObj->currentSolveMethod = index;
}

void LabProj::on_solve1_clicked()
{
    DropColor();
    ui->cellPer1->clear();
    ui->lab1Time->clear();
    runObj->solve();
    ui->lab1Time->setText(QString::number(runObj->getTime()) + " s");

    if(ui->lab1algList->currentIndex() != 2)
        ui->cellPer1->setText(QString::number(VisitedCells(cell) / (fieldSize * fieldSize) * 100) + '%');
    else
        ui->cellPer1->setText("NaN");
}

void LabProj::on_lab2AlgList_currentIndexChanged(int index)
{
    runObj2->currentSolveMethod = index;
}

void LabProj::on_solve2_clicked()
{
    DropColor();
    ui->cellPer2->clear();
    ui->lab2Time->clear();
    runObj2->solve();
    ui->lab2Time->setText(QString::number(runObj2->getTime()) + " s");
    if(ui->lab2AlgList->currentIndex() != 2)
        ui->cellPer2->setText(QString::number(VisitedCells(cell2) / (fieldSize * fieldSize) * 100) + '%');
    else
        ui->cellPer2->setText("NaN");
}

void LabProj::on_checkBox_toggled(bool checked)
{
    globalRender = checked;
    runObj->globalRender = checked;
    runObj2->globalRender = checked;
}
