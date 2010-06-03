#undef MLN_WO_GLOBAL_VARS
#include <mln/core/image/image2d.hh>
#include <mln/trace/all.hh>

#include <QtGui>
#include <src/main_window.hh>
#include <src/preprocessing_task.hh>

namespace scribo { namespace demo {extern QString basedir_; } }

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);

  qRegisterMetaType<preprocessing_task_set_t>("preprocessing_task_set_t");

  if (argc == 2)
  {
    scribo::demo::basedir_ = argv[1];
  }
  else
  {
    qDebug() << "Usage: " << argv[0] << " <path_to_scribo/src>";
    return 1;
  }

  scribo::demo::demat::main_window win;
  win.show();

  return app.exec();
}
