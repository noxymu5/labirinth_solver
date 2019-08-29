#include "labproj.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LabProj w;
    w.show();

    return a.exec();
}
