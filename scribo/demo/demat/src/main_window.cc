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

#include <src/main_window.hh>

#include <src/doc_type.hh>
#include <src/preprocessing_task.hh>
#include <src/process_args.hh>

#include <mln/data/convert.hh>
#include <mln/convert/to_qimage_nocopy.hh>
#include <mln/core/routine/duplicate.hh>
#include <mln/make/box2d.hh>
#include <mln/data/paste.hh>
#include <mln/io/magick/load.hh>

namespace scribo
{

  namespace demo
  {

    namespace demat
    {

      // FIXME: move it as attribute.
      static mln::image2d<mln::value::rgb8> input_;


      main_window::main_window()
      {
	setupUi(this);

	text_->hide();

	connect(action_Open, SIGNAL(activated()),
		this, SLOT(open_file_slot()));

	connect(run_btn, SIGNAL(clicked()), this, SLOT(run()));


	// Connections to/from task runner.
	connect(&runner_, SIGNAL(new_progress_label(const QString&)),
		this, SLOT(update_progress_label_slot(const QString&)));

	connect(&runner_, SIGNAL(progress(unsigned)),
		this, SLOT(progress_slot(unsigned)));
	connect(&runner_, SIGNAL(finished()),
		this, SLOT(process_finished_slot()));

	// Connections for dock windows.
	connect(advanced_dock_, SIGNAL(visibilityChanged(bool)),
		actionAdvanced_options, SLOT(setChecked(bool)));
	connect(options_dock_, SIGNAL(visibilityChanged(bool)),
		actionOptions, SLOT(setChecked(bool)));

	connect(actionAdvanced_options, SIGNAL(triggered(bool)),
		advanced_dock_, SLOT(setVisible(bool)));
	connect(actionOptions, SIGNAL(triggered(bool)),
		options_dock_, SLOT(setVisible(bool)));

	progress_ = new QProgressDialog();
	progress_->setLabelText("Processing...");
	progress_->setCancelButton(0);
	progress_->setAutoClose(true);

	// Setup crop tool.
	viewer_->set_selection_enabled(true);
	connect(viewer_, SIGNAL(ready_for_crop()), this, SLOT(crop_slot()));
      }


      void main_window::open_file_slot()
      {
	QFileDialog dialog;
	QStringList filters;
	filters << tr("Image (*.ppm *.pbm *.pgm *.png *.jpg)");
	dialog.setNameFilters(filters);

	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setLabelText(QFileDialog::LookIn,
			     tr("Choose an existing file"));

	if (dialog.exec() && current_image_ != dialog.selectedFiles().at(0))
	{
	  current_image_ = dialog.selectedFiles().at(0);
	  load();
	}

      }


      void main_window::load()
      {
	text_->hide();

	mln::io::magick::load(input_, current_image_.toStdString());

	input_dsp_ = QPixmap::fromImage(mln::convert::to_qimage_nocopy(input_));

	viewer_->draw_image(input_dsp_);
      }


      void main_window::run()
      {
	preprocessing_task_set_t tasks = get_tasks();

	prepare_progress_bar(tasks.size());

	process_args args; // Nothing for now.

	runner_.start(current_image_, input_, Text_Doc, tasks, args);
      }


      preprocessing_task_set_t main_window::get_tasks()
      {
	preprocessing_task_set_t tasks;

	if (removeBg->isChecked())
	  tasks.insert(RemoveBg);

	if (unskew->isChecked())
	  tasks.insert(Unskew);

	if (removeNoise->isChecked())
	  tasks.insert(RemoveNoise);

	if (improveContrast->isChecked())
	  tasks.insert(ImproveContrast);

	if (reduceSize->isChecked())
	  tasks.insert(ReduceSize);

	if (binarizationCBox->currentIndex() == 0)
	  tasks.insert(BinarizationSimple);
	else if (binarizationCBox->currentIndex() == 1)
	  tasks.insert(BinarizationSauvola);
	else if (binarizationCBox->currentIndex() == 2)
	  tasks.insert(BinarizationSauvolaMs);

	return tasks;
      }


      void main_window::prepare_progress_bar(unsigned max)
      {
	progress_->show();
	progress_->setValue(0);
	progress_->setMaximum(max + 3);
      }


      void main_window::progress_slot(unsigned i)
      {
	progress_->setValue(progress_->value() + i);
      }


      void main_window::update_progress_label_slot(const QString& msg)
      {
	progress_->setLabelText(msg);
      }


      void main_window::process_finished_slot()
      {
	text_->clear();

	QFile file("/tmp/out.txt");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	  return;

	QTextStream in(&file);
	in.setCodec("UTF-8");
	while (!in.atEnd())
	{
	  QString line = in.readLine();
	  text_->append(line);
	}


	text_->show();
      }


      void main_window::crop_slot()
      {
	mln::box2d b = input_.domain();
	QRectF selection = viewer_->selection();

	mln::box2d sbox = mln::make::box2d(selection.topLeft().y(),
					   selection.topLeft().x(),
					   selection.bottomRight().y(),
					   selection.bottomRight().x());

	sbox.crop_wrt(b);

	if (sbox.is_valid())
	{
	  // Update underlying data.
	  mln::image2d<mln::value::rgb8> output(sbox);
	  mln::data::fill(output, input_);
	  input_ = output;

	  // Update display
	  viewer_->draw_image(input_);
	}

	viewer_->enable_crop_tool(false); // Disable crop tool
      }

    } // end of namespace scribo::demo::demat

  } // end of namespace scribo::demo

} // end of namespace scribo
