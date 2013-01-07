#include <QtGui/QApplication>
#include "notepad.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NotePad w;
    w.show();

    return a.exec();
}
