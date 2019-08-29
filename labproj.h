#ifndef LABPROJ_H
#define LABPROJ_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>
#include <QThread>
#include <QMessageBox>
#include "runner.h"

#include <random>
#include <list>

namespace Ui {
class LabProj;
}

class LabProj : public QDialog
{
    Q_OBJECT

public:
    explicit LabProj(QWidget *parent = 0);
    ~LabProj();

private slots:
    void on_labBuildButton_clicked();
    void on_lab1algList_currentIndexChanged(int index);
    void on_solve1_clicked();

    void on_lab2AlgList_currentIndexChanged(int index);

    void on_solve2_clicked();

    void on_checkBox_toggled(bool checked);

private:
    Ui::LabProj *ui;
    QGraphicsScene *labScene;
    QGraphicsScene *labScene2;
    labirintCell ***cell = 0;
    labirintCell ***cell2 = 0;

    runner *runObj = 0;
    runner *runObj2 = 0;
    std::default_random_engine rnG;

    int res = 500;
    int fieldSize;
    bool renderOrNot;
    bool globalRender;

    std::list<int> hist;
    int rngHist[2] = {-1, -1};
    int last = 0;

    void Split(qreal x, qreal y, int width, int height, int orientation);
    int GetRandom();
    void DropColor();

    void Redraw(bool render);
    float VisitedCells(labirintCell ***grid);
};

#endif // LABPROJ_H
