#undef MLN_WO_GLOBAL_VARS

//#include <scribo/make/debug_filename.hh>
//#include <mln/labeling/colorize.hh>
//#include <mln/io/magick/load.hh>
//#include <mln/literal/colors.hh>
//#include <mln/debug/filename.hh>
#include <QtGui/QApplication>
//#include <mln/math/pi.hh>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    //Magick::InitializeMagick(*argv);
    // On Linux, we NEED to use the raster graphics system.
    // Linux don't really support openGL graphics system (the default one on Linux).
    QApplication::setGraphicsSystem("raster");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
