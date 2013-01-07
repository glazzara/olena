#include <QtGui/QApplication>
#include "test2.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Test2 w;
    w.show();
    return a.exec();
}
