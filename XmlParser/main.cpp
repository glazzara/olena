#undef MLN_WO_GLOBAL_VARS

#include <mln/labeling/colorize.hh>
#include <mln/math/pi.hh>
#include <mln/io/magick/load.hh>
#include <mln/literal/colors.hh>
#include <mln/debug/filename.hh>
#include <scribo/make/debug_filename.hh>
#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char **argv)
{
    Magick::InitializeMagick(*argv);
    QApplication::setGraphicsSystem("raster");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
