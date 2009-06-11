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

#include <QtCore>
#include <QtGui>

#include <mln/core/image/image2d.hh>
#include <mln/logical/not.hh>
#include <mln/io/pbm/all.hh>

#include <src/mainwindow.hh>


const char * modes[][4] = {
  { "Text in pictures", "pics", "text_in_photo", "image-x-generic.png" },
  { (const char *)(1), 0, 0, 0}, // Separator
  { "Handwritten text lines", "hsc", "input_to_lines", "text-x-generic.png" },
  { "Handwritten text words", "hsc", "input_to_words", "text-x-generic.png" },
  { (const char *)(1), 0, 0, 0}, // Separator
  { "Horizontal and Vertical Lines", "hvl", "extract/primitive/find_discontinued_lines", "list-add.png" },
  { "Tables", "tables", "table/extract", "x-office-spreadsheet.png" },
  { "Pre-processing", "pproc", "preprocess", "format-indent-more.png" },
  { "OCR", "ocr", "recognition", "edit-find.png" },
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
  { 0, 0, 0 }, // Text in Pictures
  { (const char *)(1), 0, 0}, // Separator
  { "/dev/null",  0,  0 }, // Handwritten text lines
  { "/dev/null",  0,  0 }, // Handwritten text words
  { (const char *)(1), 0, 0}, // Separator
  { "51", "6", 0 }, // Horizontal and Vertical lines
  { "/dev/null",  0,  0 }, // Tables
  { 0, 0, 0 }, // Pre-processing
  { 0, 0, 0 }, // OCR
  // Not empty line needed.
};



namespace scribo
{

  namespace demo
  {

    MainWindow::MainWindow(QWidget *parent)
      : QMainWindow(parent), context_(this)
    {
      setupUi(this);

      mainItem_ = 0;
      exec_prefix_ = "../";
      pdialog_.setRange(0,0);
      pdialog_.setLabelText(tr("Please wait while computing..."));
      pdialog_.setWindowModality(Qt::WindowModal);

      connect(&pdialog_, SIGNAL(canceled()), this, SLOT(compute_canceled()));
      connect(&exec_, SIGNAL(finished(int, QProcess::ExitStatus)),
	      this, SLOT(exec_finished(int, QProcess::ExitStatus)));
      connect(&exec_, SIGNAL(error(QProcess::ProcessError)),
	      this, SLOT(exec_error(QProcess::ProcessError)));

      connect(&context_, SIGNAL(triggered(QAction *)),
	      this, SLOT(context_changed(QAction *)));

      context_.setExclusive(true);

      initToolBar();
      setup_scene();
    }


    void MainWindow::initToolBar()
    {
      QToolBar *tbar = new QToolBar("Tools");
      tbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

      for (unsigned i = 0; modes[i][0]; ++i)
      {
	if (modes[i][0] == (const char *)(1))
	{
	  tbar->addSeparator();
	  continue;
	}

        QAction *action = tbar->addAction(modes[i][0]);
	context_.addAction(action);
	action->setCheckable(true);
	action->setData(i);

	if (modes[i][3] != 0)
	  action->setIcon(QIcon(QString(":/icons/") + modes[i][3]));
      }

      addToolBar(Qt::TopToolBarArea, tbar);
      context_.actions().first()->trigger();
    }



    void MainWindow::on_browseBtn_clicked(bool)
    {
      QString current_dir = modes[mode_][1] + QString("/");

      QString
	filename = QFileDialog::getOpenFileName(this,
	  tr("Open Image."),
	  QCoreApplication::applicationDirPath() + "/" + current_dir,
	  tr("Images (*.pbm *.pgm *.ppm)"));

      if (!filename.isEmpty())
      {
        filepath->setText(filename);
        update_pixmap(filepath->text());
      }
    }


    void MainWindow::on_runBtn_clicked(bool)
    {
      wait_for_result();

      QStringList args;
      args << filepath->text();
      for (unsigned i = 0; args_list[mode_][i]; ++i)
	args << args_list[mode_][i];

      if (!is_in_ocr_mode())
      {
        QTemporaryFile f;
        f.open();
        args << f.fileName();
        last_output_ = f.fileName();
      }

      exec_.start(exec_prefix_ + modes[mode_][2], args);
    }


    void MainWindow::on_displayBtn_clicked(bool)
    {
      update_pixmap(filepath->text());
    }


    void MainWindow::on_filepath_textChanged()
    {
      bool b = !filepath->text().isEmpty();

      runBtn->setEnabled(b);
      displayBtn->setEnabled(b);
    }


    void MainWindow::update_pixmap(const QString& name)
    {
      QPixmap pixmap;
      if (!pixmap.load(name))
      {
	qDebug() << "Invalid file format!";
	update_status_message(tr("Invalid file format!"));
	return;
      }

      imageResult->scene()->removeItem(mainItem_);
      mainItem_ = imageResult->scene()->addPixmap(pixmap);
      imageResult->scene()->setSceneRect(mainItem_->boundingRect());
      Q_ASSERT(mainItem_ != 0);
    }


    void MainWindow::exec_finished(int rvalue, QProcess::ExitStatus status)
    {
      pdialog_.hide();
      qDebug() << rvalue;
      if (rvalue == 0)
      {
	if (status != QProcess::CrashExit)
	{
	  setEnabled(true);
	  if (is_in_ocr_mode())
	  {
	    textBrowser->setText(exec_.readAllStandardOutput());
	    on_displayBtn_clicked(true);
	    tabWidget->setCurrentIndex(1);
	  }
	  else
	    update_pixmap(last_output_);
	}
	// Else : canceled.
      }
      else
	exec_error(tr("Error during last run."));
    }


    void MainWindow::setup_scene()
    {
      QGraphicsScene *scene = new QGraphicsScene();
      imageResult->setScene(scene);

      update_status_message(tr("1) Choose an algorithm.\n\
2) Select an image.\n3) Run."));

      imageResult->show();
    }


    void MainWindow::wait_for_result()
    {
      pdialog_.show();
      setEnabled(false);

      if (!is_in_ocr_mode())
	update_status_message(tr("Please wait while computing..."));
    }


    void MainWindow::update_status_message(const QString& msg)
    {
      if (mainItem_ != 0)
	imageResult->scene()->removeItem(mainItem_);
      mainItem_ = imageResult->scene()->addText(msg);
    }


    void MainWindow::exec_error(const QString& msg)
    {
      setEnabled(true);
      QMessageBox::critical(0, tr("Error!"),
			    tr("The result cannot be computed!"));
      update_status_message(msg);
      qDebug() << exec_.readAllStandardOutput();
    }

    void MainWindow::exec_error(QProcess::ProcessError error)
    {
      pdialog_.reset();
      if (error == QProcess::FailedToStart)
        exec_error(tr("This program does not exist: ")
		    + exec_prefix_ + modes[mode_][2]);
      else
	qDebug() << "The computation stopped.";
    }


    void MainWindow::context_changed(QAction *action)
    {
      mode_ = action->data().toInt();
      filepath->clear();
      tabWidget->setTabEnabled(1, is_in_ocr_mode());
    }


    void MainWindow::compute_canceled()
    {
      setEnabled(true);
      exec_.kill();
      update_status_message("Computation canceled.");
    }


    bool MainWindow::is_in_ocr_mode()
    {
      return (QString(modes[mode_][1]) == "ocr");
    }

  } // end of namespace scribo::demo

} // end of namespace scribo
