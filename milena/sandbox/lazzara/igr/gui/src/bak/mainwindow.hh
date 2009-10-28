// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_DEMO_SRC_MAINWINDOW_HH

# include <QDir>
# include <QTimer>
# include <QStringList>
# include <QProcess>
# include <QtGui>
# include <QProgressDialog>
# include <ui_mainwindow.h>

# ifndef INCLUDE_MLN_FILES
#  define MLN_INCLUDE_ONLY
# endif

# include <mln/core/image/image3d.hh>
# include <mln/value/rgb8.hh>

namespace mln
{

  namespace demo
  {

    // Forward declaration.
    class image_viewer;


    class MainWindow : public QMainWindow, private Ui::MainWindow
    {
      Q_OBJECT


    public:
      MainWindow(QWidget *parent = 0);
      ~MainWindow();

      void set_base_img_dir(const QString& dir);

    private slots:
      void initToolBar();

      void on_browseBtn_clicked(bool b);
      void on_runBtn_clicked();
      void on_displayBtn_clicked(bool b);
      void on_filepath_textChanged();
      void on_autoDemo_clicked(bool checked);

      void exec_finished(int rvalue, QProcess::ExitStatus status);
      void exec_error(QProcess::ProcessError error);

      void compute_canceled();

      void timer_timeout();

      void update_process_status();

      void on_listWidget_currentItemChanged(QListWidgetItem *current,
					    QListWidgetItem *previous);

      void compute_image(int sli);

    private: // Members

      void update_pixmap(QGraphicsView* view, const QString& name = QString());

      void load_dicom(const QString& filename);

      template <typename I>
      mln_ch_value(I, value::rgb8) to_rgb8(const Image<I>& ima_);

      template <typename I>
      QImage convert(const Image<I>& ima_);

      void prepare_and_send_result(const QString& output);

      void exec_error(const QString& msg);

      void wait_for_result();

      void prepare_for_run(const QString& filename);

      void update_auto_demo_dir();

      void reset_progress_dialog();

      void connect_compute_process();

      void update_options(QListWidgetItem *item);

    private: // Attributes
      image_viewer *viewer_;

      QString exec_prefix_;

      QString last_output_;
      QProcess exec_;

      QProgressDialog pdialog_;

      QString base_img_dir_;

      int mode_;
      image3d<value::rgb8> dcm_ima;
      QList<QSpinBox*> args_values_;


      QVector<QHash<QString, QString> > cached_result_;

      /// Related to auto demo.
      QTimer timer_;
      QDir demoDir_;
      unsigned demo_index_;
      QStringList demo_files_;
    };


  } // end of namespace mln::demo

} // end of namespace mln

#endif // ! MLN_DEMO_SRC_MAINWINDOW_HH
