// Copyright (C) 2009, 2011 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef SCRIBO_APPS_DEMO_SRC_MAINWINDOW_HH

# include <QDir>
# include <QTimer>
# include <QStringList>
# include <QProcess>
# include <QtGui/QWidget>
# include <QProgressDialog>
# include "mainwindow.ui.h"

namespace scribo
{

  namespace demo
  {

    class MainWindow : public QMainWindow, private Ui::MainWindow
    {
      Q_OBJECT


    public:
      MainWindow(const QString& exec_prefix, QWidget *parent = 0);
      ~MainWindow();

      void set_base_img_dir(const QString& dir);

    private slots:
      void initToolBar();

      void on_browseBtn_clicked(bool b);
      void on_runBtn_clicked();
      void on_displayBtn_clicked(bool b);
      void on_filepath_textChanged();
      void on_zoomIn_clicked();
      void on_zoomOut_clicked();
      void on_zoomFixed_clicked();
      void on_zoomOriginal_clicked();
      void on_autoDemo_clicked(bool checked);

      void exec_finished(int rvalue, QProcess::ExitStatus status);
      void exec_error(QProcess::ProcessError error);

      void context_changed(QAction *action);

      void compute_canceled();

      void resize_pixmaps(const QRectF& rect);

      void move_vertical_sliders(int value);
      void move_horizontal_sliders(int value);
      void timer_timeout();

      void update_process_status();

    private: // Members
      void setup_scene();
      void update_pixmap(QGraphicsView* view, const QString& name = QString());

      void exec_error(const QString& msg);

      void wait_for_result();

      void update_status_message(const QString& msg);

      void prepare_for_run(const QString& filename);

      bool is_in_ocr_mode() const;
      bool is_in_pics_mode() const;
      bool is_in_doc_mode() const;

      QGraphicsItem *& view_to_item(QGraphicsView *view);

      void resizeEvent(QResizeEvent * event);
      void keyPressEvent(QKeyEvent *event);

      QString current_mode() const;

      void update_auto_demo_dir();

      void reset_progress_dialog();

      void connect_compute_process();

    private: // Attributes
      QGraphicsItem *mainRefItem_;
      QGraphicsItem *mainResultItem_;
      QString exec_prefix_;

      QActionGroup context_;
      int mode_;
      QString last_output_;
      QProcess exec_;

      QProgressDialog pdialog_;

      QString base_img_dir_;

      QVector<QHash<QString, QString> > cached_result_;

      /// Related to auto demo.
      QTimer timer_;
      QDir demoDir_;
      unsigned demo_index_;
      QStringList demo_files_;
    };


  } // end of namespace scribo::demo

} // end of namespace scribo

#endif // ! SCRIBO_APPS_DEMO_SRC_MAINWINDOW_HH
