// Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_DEMO_DEMAT_SRC_MAIN_WINDOW_HH
# define SCRIBO_DEMO_DEMAT_SRC_MAIN_WINDOW_HH

# include <QtGui/QMainWindow>
# include <ui_main_window.h>
# include <src/doc_type.hh>
# include <src/preprocessing_task.hh>
# include <src/runner.hh>

# include <mln/core/image/image2d.hh>
# include <mln/core/image/dmorph/image_if.hh>
# include <mln/pw/all.hh>
# include <mln/value/rgb8.hh>


namespace scribo
{

  namespace demo
  {

    namespace demat
    {


      class main_window : public QMainWindow, private Ui::MainWindow
      {
	Q_OBJECT;

      public:
	main_window();

      private slots:
	void run();
	void open_file_slot();
	void progress_slot(unsigned i);
	void update_progress_label_slot(const QString&);
	void process_finished_slot();

	void crop_slot();

	void on_action_Reload_triggered(bool b);

      private: // members
	void load();

	void prepare_progress_bar(unsigned max);
	Doc_Type get_doc_type();
	preprocessing_task_set_t get_tasks();
	mln::image2d<mln::value::rgb8> get_crop_image() const;

      private: // attributes
	QPixmap input_dsp_;

	QString current_image_;

	QProgressDialog* progress_;
	runner runner_;

	mln::image2d<mln::value::rgb8> input_;
      };


    } // end of namespace scribo::demo::demat

  } // end of namespace scribo::demo

} // end of namespace scribo


#endif // ! SCRIBO_DEMO_DEMAT_SRC_MAIN_WINDOW_HH
