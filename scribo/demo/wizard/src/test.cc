#include <QtGui>
#include <shared/src/image_viewer.hh>
#include <shared/src/crop_item.hh>

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);

  scribo::demo::shared::image_viewer win;
  win.show();
  win.draw_image(QPixmap("image.jpg"), QPoint(0,0));

  win.setSelectionEnabled(true);

  return app.exec();
}
