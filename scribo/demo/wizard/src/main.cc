#include <QtGui>
#include <src/main_window.hh>
#include <src/preprocessing_task.hh>

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);

  qRegisterMetaType<preprocessing_task_set_t>("preprocessing_task_set_t");

  scribo::demo::wizard::main_window win;
  win.show();

  return app.exec();
}
