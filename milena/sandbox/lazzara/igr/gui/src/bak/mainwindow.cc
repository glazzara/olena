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

#include <QApplication>
#include <QtCore>
#include <QtGui>

# define INCLUDE_MLN_FILES
#include <src/mainwindow.hh>

#include <src/image_viewer.hh>

#include <mln/core/image/dmorph/slice_image.hh>
#include <mln/core/image/image3d.hh>
#include <mln/border/resize.hh>
#include <mln/logical/not.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/int_u12.hh>
#include <mln/value/int_u8.hh>
#include <mln/data/convert.hh>
#include <mln/data/stretch.hh>
#include <mln/io/dicom/all.hh>
#include <mln/io/dump/all.hh>


#include <mln/io/ppm/save.hh>


const char * modes[][4] = {
//  { "Text in pictures", "pics", "../src/text_in_photo_ppm", "image-x-generic.png" },
  { "Segmentation 3d", "seg", "../watershed3d", 0 },
  { 0, 0, 0 } // Empty line, do NOT delete.
};


// Allow to set up to 3 extra arguments to the binaries.
//
// A program will be launched as follow:
//
//    ./my_program input.pbm <arg1> <arg2> <arg3> output.ppm
//
// Unused arguments are set to 0.
// Each line must be mapped to the related on in the previous
// array "modes". So DO preserve the order.
//
const char *args_list[][3] = {
  { "51", 0, 0 }
  // Not empty line needed.
};



namespace mln
{

  namespace demo
  {

    MainWindow::MainWindow(QWidget *parent)
      : QMainWindow(parent)
    {
      setupUi(this);

      viewer_ = new image_viewer();
      gridLayout_2->addWidget(viewer_, 0, 0);

      unsigned nmodes = 0;
      for (unsigned i = 0; modes[i][0] != 0; ++i)
	++nmodes;

      cached_result_.resize(nmodes);
      exec_prefix_ = "";
      base_img_dir_ = "/lrde/stockholm/lazzara/svn/olena/git/oln/milena/sandbox/lazzara/igr/gui";
// QCoreApplication::applicationDirPath();

      pdialog_.setRange(0,0);
      pdialog_.setLabelText(tr("Please wait while computing..."));
      pdialog_.setWindowModality(Qt::WindowModal);

      exec_.setReadChannel(QProcess::StandardOutput);

      qDebug() << "Cache located in " << QDir::tempPath();

      connect(&pdialog_, SIGNAL(canceled()), this, SLOT(compute_canceled()));

      connect(viewer_, SIGNAL(slider_valueChanged(int)),
	      this, SLOT(compute_image(int)));

      connect_compute_process();

      // No status bar.
      setStatusBar(0);

      initToolBar();
    }

    MainWindow::~MainWindow()
    {
      for (int i = 0; i < cached_result_.size(); ++i)
	foreach(QString value, cached_result_[i])
	{
	  QFile f(value);
	  f.remove();
	}
    }


    void MainWindow::set_base_img_dir(const QString& dir)
    {
      QDir d(dir);
      base_img_dir_ = d.absolutePath();
    }


    void MainWindow::initToolBar()
    {
      QToolBar *tbar = new QToolBar("Tools");
      tbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

      for (unsigned i = 0; modes[i][0]; ++i)
      {
	QListWidgetItem *item = new QListWidgetItem(modes[i][0], listWidget);
	item->setData(Qt::UserRole, i);
      }

      listWidget->setCurrentItem(listWidget->item(0));
    }



    void MainWindow::on_browseBtn_clicked(bool)
    {
      QString
	current_dir = mode_ + QString("/");

      qDebug() << (base_img_dir_ + "/" + current_dir);
      QString
	filename = QFileDialog::getOpenFileName(this,
	  tr("Open Image."),
	  base_img_dir_ + "/" + current_dir,
	  tr("Images (*.dcm)"));

      prepare_for_run(filename);
    }

    void MainWindow::prepare_for_run(const QString& filename)
    {
      if (!filename.isEmpty())
      {
        filepath->setText(filename);

	load_dicom(filename);

      }
    }

    void MainWindow::load_dicom(const QString& filename)
    {
      image3d<value::int_u12> tmp;
      io::dicom::load(tmp, filename.toStdString().c_str());
      dcm_ima = to_rgb8(tmp);

      viewer_->set_image_size(geom::nrows(dcm_ima), geom::ncols(dcm_ima),
			      geom::nslis(dcm_ima));
    }


    void MainWindow::compute_image(int sli)
    {
      QImage ima = this->convert(duplicate(slice(dcm_ima, sli)));
      viewer_->updated_image(ima);
    }


    template <typename I>
    mln_ch_value(I, value::rgb8) MainWindow::to_rgb8(const Image<I>& ima_)
    {
      const I& ima = exact(ima_);
      mln_precondition(ima.is_valid());

      mln_ch_value(I, value::rgb8)
	tmp = data::convert(value::rgb8(),
			    data::stretch(value::int_u8(), ima));

      return tmp;
    }


    template <typename I>
    QImage MainWindow::convert(const Image<I>& ima_)
    {
      const I& ima = exact(ima_);
      mln_precondition(ima.is_valid());

      const int
	nrows = geom::nrows(ima),
	ncols = geom::ncols(ima);

      // Required by a one-shot data copy:
      mln::border::resize(ima, 0);

      QImage qima(ncols, nrows, QImage::Format_RGB888);
      std::memcpy(qima.scanLine(0),
		  ima.buffer(),
		  ima.nelements() * 3);
      return qima;
    }


    void MainWindow::on_runBtn_clicked()
    {
//       if (useCache->isChecked()
// 	  && cached_result_[mode_].contains(filepath->text()))
//       {
// 	last_output_ = cached_result_[mode_][filepath->text()];
// 	exec_finished(0, QProcess::NormalExit);
// 	return;
//       }

      wait_for_result();

      QStringList args;

      QString input = filepath->text();

      args << input;
      for (int i = 0; i < args_values_.size(); ++i)
 	args << QString::number(args_values_[i]->value());

      QTemporaryFile f;
      f.open();
      args << f.fileName();
      last_output_ = f.fileName();
      qDebug() << last_output_;


      qDebug() << exec_prefix_ + modes[mode_][2]
	       << " - "
	       << args;
      exec_.start(exec_prefix_ + modes[mode_][2], args);
    }


    void MainWindow::on_listWidget_currentItemChanged(QListWidgetItem *current,
						      QListWidgetItem *previous)
    {
      (void) previous;
      mode_ = current->data(Qt::UserRole).toInt();
      update_options(current);
    }


    void MainWindow::update_options(QListWidgetItem */*current*/)
    {
      QFormLayout *formLayout = new QFormLayout;

      QProcess process;
      process.start(modes[mode_][2]);
      process.waitForFinished();

      QByteArray usage = process.readAllStandardOutput();
      QList<QByteArray> list = usage.split('<');

      delete frame->layout();
      args_values_.clear();

      for (int i = 2; i < list.size() - 1; ++i)
      {
	args_values_.append(new QSpinBox());
	args_values_.last()->setValue(atoi(args_list[mode_][i - 2]));
	formLayout->addRow(list[i].replace('>', ' '), args_values_.last());
      }

      frame->setLayout(formLayout);
    }


    void MainWindow::on_displayBtn_clicked(bool)
    {
      load_dicom(filepath->text());
    }


    void MainWindow::on_filepath_textChanged()
    {
      bool b = !filepath->text().isEmpty();

      runBtn->setEnabled(b);
      displayBtn->setEnabled(b);
    }


    void MainWindow::exec_finished(int rvalue, QProcess::ExitStatus status)
    {
      pdialog_.hide();
      if (rvalue == 0)
      {
 	if (status != QProcess::CrashExit)
 	{
 	  if (useCache->isChecked())
 	    cached_result_[mode_][filepath->text()] = last_output_;
 	  setEnabled(true);
	  prepare_and_send_result(last_output_);
 	}
 	// Else : canceled.
      }
      else
	 exec_error(tr("Error during last run."));

      if (autoDemo->isChecked())
	timer_.start(timeoutDelay->text().toInt() * 1000);
    }


    void MainWindow::prepare_and_send_result(const QString& output)
    {
      QImage ima(output);
      viewer_->updated_image(ima);
    }

    void MainWindow::wait_for_result()
    {
      reset_progress_dialog();

      pdialog_.show();
      setEnabled(false);
    }

    void MainWindow::exec_error(const QString& msg)
    {
      setEnabled(true);
      QMessageBox::critical(0, tr("Error!"),
			    tr("The result cannot be computed!"));
      qDebug() << "exec_error : " << msg;
      qDebug() << exec_.readAllStandardOutput();
    }

    void MainWindow::exec_error(QProcess::ProcessError error)
    {
      if (error == QProcess::FailedToStart)
        exec_error(tr("This program does not exist: ")
		    + exec_prefix_ + modes[mode_][2]);
      else
        exec_error(tr("The computation stopped.")
		    + exec_prefix_ + modes[mode_][2]);
    }



    void MainWindow::compute_canceled()
    {
      setEnabled(true);

      exec_.disconnect();
      exec_.kill();
      connect_compute_process();

    }


    void MainWindow::on_autoDemo_clicked(bool checked)
    {
      if (checked)
	timer_.start(100);
      else
	timer_.stop();
    }


    void MainWindow::timer_timeout()
    {
      update_auto_demo_dir();
      timer_.stop();
      prepare_for_run(demoDir_.absolutePath() + "/"
		      + demo_files_[demo_index_]);
      demo_index_ = (demo_index_ + 1) % demo_files_.size();
    }

    void MainWindow::update_auto_demo_dir()
    {
//       QString tmp_path = base_img_dir_ + "/" + current_mode();
//       if (demoDir_.path() != tmp_path)
//       {
// 	demoDir_.setPath(tmp_path);
// 	demo_index_ = 0;
// 	demo_files_ = demoDir_.entryList(QDir::NoDotAndDotDot | QDir::Files);
//       }
    }


    void MainWindow::update_process_status()
    {
      pdialog_.setLabelText(tr(exec_.readAllStandardOutput()));
    }


    void MainWindow::reset_progress_dialog()
    {
      pdialog_.reset();
      pdialog_.setLabelText(tr("Please wait while computing..."));
    }


    void MainWindow::connect_compute_process()
    {
      connect(&exec_, SIGNAL(finished(int, QProcess::ExitStatus)),
	      this, SLOT(exec_finished(int, QProcess::ExitStatus)));
      connect(&exec_, SIGNAL(error(QProcess::ProcessError)),
	      this, SLOT(exec_error(QProcess::ProcessError)));
      connect(&exec_, SIGNAL(readyReadStandardOutput()),
	      this, SLOT(update_process_status()));
    }


  } // end of namespace scribo::demo

} // end of namespace scribo


