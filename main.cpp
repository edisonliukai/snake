#include "qtsnake.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtSnake w;
    w.show();

    return a.exec();
}
