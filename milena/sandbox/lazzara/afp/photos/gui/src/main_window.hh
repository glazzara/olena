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

#ifndef MLN_DEMO_SRC_MAIN_WINDOW_HH

# include <QtGui>
# include <ui_main_window.h>

namespace mln
{

  namespace demo
  {


    class main_window : public QMainWindow, private Ui::MainWindow
    {
      Q_OBJECT

    public:
      main_window(const QString& text_in_photo_ppm, QWidget *parent = 0);
      ~main_window();


    private slots:
      void process__error_slot(QProcess::ProcessError);
      void process__finished_slot(int exitCode,
				  QProcess::ExitStatus exitStatus);
      void on_withTextListWidget_itemClicked(QListWidgetItem * item);
      void on_withoutTextListWidget_itemClicked(QListWidgetItem * item);

      void on_fullImageButton_toggled(bool b);
      void on_textBoxesButton_toggled(bool);
      void on_textMaskButton_toggled(bool);
      void on_textOnlyButton_toggled(bool);

      void on_tabWidget_currentChanged(int index);
      void on_actionScan_directory_triggered();
      void init_and_start_process(const QString& dirname, bool bgfg);
      void cancel_clicked(bool);

      void remove_image(bool);

    signals:
      void process_finished();

    private: // Members
      void keyPressEvent(QKeyEvent* event);
      void resizeEvent(QResizeEvent* event);

      QString output_file(const QString& basename);
      QString textBoxes_file(const QString& basename);
      QString textMask_file(const QString& basename);

      void display_image(const QString& filename);
      void start_process();
      void next_process();

      void connect_process();

    private: // Attributes
      QString output_dir_;
      QDir dir_;
      QString text_in_photo_ppm_;
      QProcess process_;
      bool bgfg_;

      QFileInfoList file_list_;
      int current_file_;

      // Status bar
      QLabel progressLabel_;
      QProgressBar progressBar_;

      // ToolBar
      QToolBar *toolbar_;
    };


  } // end of namespace mln::demo

} // end of namespace mln

#endif // ! MLN_DEMO_SRC_MAIN_WINDOW_HH
