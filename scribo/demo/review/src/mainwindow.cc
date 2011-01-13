// Copyright (C) 2009, 2010 EPITA Research and Development Laboratory
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

#include <QtCore>
#include <QtGui>

#include "src/mainwindow.hh"


const char * modes[][4] = {
  { "Text in pictures", "pics", "text_in_picture", "image-x-generic.png" },
  { "Text in docs", "doc", "text_in_doc", "edit-find.png" },
  { (const char *)(1), 0, 0, 0}, // Separator
  { "Handwritten text lines", "hsc", "../../milena/sandbox/icdar/2009/hsc/input_to_lines", "text-x-generic.png" },
  { "Handwritten text words", "hsc", "../../milena/sandbox/icdar/2009/hsc/input_to_words", "text-x-generic.png" },
  { (const char *)(1), 0, 0, 0}, // Separator
  { "Horizontal and Vertical Lines", "hvl", "extract/primitive/find_discontinued_lines", "list-add.png" },
  { "Tables", "hvl", "table/extract", "x-office-spreadsheet.png" },
  { "Pre-processing", "pproc", "preprocess", "format-indent-more.png" },
  { "OCR", "ocr", "recognition", "edit-find.png" },
  { 0, 0, 0 } // Empty line, do NOT delete.
};


// Allow to set up to 5 extra arguments to the binaries.
//
// A program will be launched as follow:
//
//    ./my_program input.pbm output <arg1> <arg2> <arg3> <arg4> <arg5>
//
// Unused arguments are set to 0.
// Each line must be mapped to the related on in the previous
// array "modes". So DO preserve the order.
//
const char *args_list[][6] = {
  { "1", "1", "1", "1", "1", 0 }, // Text in Pictures
  { 0, 0, 0, 0, 0, 0 }, // Text in Docs
  { (const char *)(1), 0, 0, 0, 0, 0 }, // Separator
  { "/dev/null",  0,  0, 0, 0, 0 }, // Handwritten text lines
  { "/dev/null",  0,  0, 0, 0, 0 }, // Handwritten text words
  { (const char *)(1), 0, 0, 0, 0, 0 }, // Separator
  { "51", 0, 0, 0, 0, 0 }, // Horizontal and Vertical lines
  { "/dev/null",  0, 0, 0, 0, 0 }, // Tables
  { 0, 0, 0, 0, 0, 0 }, // Pre-processing
  { 0, 0, 0, 0, 0, 0 }, // OCR
  // Not empty line needed.
};



namespace scribo
{

  namespace demo
  {

    MainWindow::MainWindow(const QString& exec_prefix, QWidget *parent)
      : QMainWindow(parent), exec_prefix_(exec_prefix + "/"), context_(this)
    {
      setupUi(this);

      unsigned nmodes = 0;
      for (unsigned i = 0; modes[i][0] != 0; ++i)
	++nmodes;

      cached_result_.resize(nmodes);
      mainRefItem_ = 0;
      mainResultItem_ = 0;
      base_img_dir_ = QCoreApplication::applicationDirPath();

      pdialog_.setRange(0,0);
      pdialog_.setLabelText(tr("Please wait while computing..."));
      pdialog_.setWindowModality(Qt::WindowModal);

      exec_.setReadChannel(QProcess::StandardOutput);

      qDebug() << "Cache located in " << QDir::tempPath();

      connect(&pdialog_, SIGNAL(canceled()), this, SLOT(compute_canceled()));

      connect(&context_, SIGNAL(triggered(QAction *)),
	      this, SLOT(context_changed(QAction *)));

      connect(imageResult->verticalScrollBar(), SIGNAL(valueChanged(int)),
	      this, SLOT(move_vertical_sliders(int)));
      connect(imageResult->horizontalScrollBar(), SIGNAL(valueChanged(int)),
	      this, SLOT(move_horizontal_sliders(int)));
      connect(imageRef->verticalScrollBar(), SIGNAL(valueChanged(int)),
	      this, SLOT(move_vertical_sliders(int)));
      connect(imageRef->horizontalScrollBar(), SIGNAL(valueChanged(int)),
	      this, SLOT(move_horizontal_sliders(int)));
      connect(&timer_, SIGNAL(timeout()), this, SLOT(timer_timeout()));

      connect_compute_process();

      context_.setExclusive(true);

      // No status bar.
      setStatusBar(0);

      initToolBar();
      setup_scene();
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
	if (modes[i][0] == (const char *)(1))
	{
	  tbar->addSeparator();
	  continue;
	}

        QAction *action = tbar->addAction(modes[i][0]);
	context_.addAction(action);
	action->setCheckable(true);
	action->setData(i);
	action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_0 + i));
	action->setEnabled(QFile::exists(exec_prefix_ +  modes[i][2]));

	if (modes[i][3] != 0)
	  action->setIcon(QIcon(QString(":/icons/") + modes[i][3]));
      }

      addToolBar(Qt::TopToolBarArea, tbar);
      context_.actions().first()->trigger();
    }



    void MainWindow::on_browseBtn_clicked(bool)
    {
      QString current_dir = current_mode() + "/";

      QString
	filename = QFileDialog::getOpenFileName(this,
	  tr("Open Image."),
	  base_img_dir_ + "/" + current_dir,
	  tr("Images (*.pbm *.pgm *.ppm *.jpg *.png *.tiff)"));

      prepare_for_run(filename);
    }

    void MainWindow::prepare_for_run(const QString& filename)
    {
      if (!filename.isEmpty())
      {
        filepath->setText(filename);

	update_pixmap(imageRef, filename);
	update_pixmap(imageResult);

	if (autoRun->isChecked())
	  on_runBtn_clicked();
      }
    }

    void MainWindow::on_runBtn_clicked()
    {
      if (useCache->isChecked()
	  && cached_result_[mode_].contains(filepath->text()))
      {
	last_output_ = cached_result_[mode_][filepath->text()];
	exec_finished(0, QProcess::NormalExit);
	return;
      }

      wait_for_result();

      QStringList args;

      QString input = filepath->text();

      args << input;
      QTemporaryFile f;
      f.open();
      args << f.fileName();
      last_output_ = f.fileName();

      for (unsigned i = 0; args_list[mode_][i]; ++i)
	args << args_list[mode_][i];

//      if (!is_in_ocr_mode())
//      {
//      }

//      reset_progress_dialog();
      qDebug() << "Running " << exec_prefix_ + modes[mode_][2]
	       << " " << args;

      exec_.start(exec_prefix_ + modes[mode_][2], args);
    }


    void MainWindow::on_displayBtn_clicked(bool)
    {
      update_pixmap(imageRef, filepath->text());
    }


    void MainWindow::on_filepath_textChanged()
    {
      bool b = !filepath->text().isEmpty();

      runBtn->setEnabled(b);
      displayBtn->setEnabled(b);
    }


    void MainWindow::on_zoomIn_clicked()
    {
	imageResult->scale(1.2, 1.2);
	imageRef->scale(1.2, 1.2);
    }

    void MainWindow::on_zoomOut_clicked()
    {
	imageResult->scale(1 / 1.2, 1 / 1.2);
	imageRef->scale(1 / 1.2, 1 / 1.2);
    }

    QGraphicsItem *& MainWindow::view_to_item(QGraphicsView *view)
    {
      if (view == imageResult)
	return mainResultItem_;
      else
	return mainRefItem_;
    }

    void MainWindow::update_pixmap(QGraphicsView* view, const QString& name)
    {
      if (name.isEmpty())
      {
	update_status_message("");
	return;
      }

      QGraphicsItem* item = view_to_item(view);

      QPixmap pixmap;
      if (!pixmap.load(name))
      {
	qDebug() << "Invalid file format!";
	update_status_message(tr("Invalid file format!"));
	return;
      }

      QFileInfo finfo(name);
      if (item != mainResultItem_ && current_mode() != "hsc"
	  && current_mode() != "pproc"
	  && (current_mode() != "pics" || finfo.suffix() == "pbm")
	  && ! is_in_doc_mode())
      {
	QImage image(pixmap.toImage());
	image.invertPixels();
	pixmap = QPixmap::fromImage(image);
      }

      if (item != 0)
      {
	view->scene()->removeItem(item);
	delete item;
      }
      item = view->scene()->addPixmap(pixmap);
      view->scene()->setSceneRect(item->boundingRect());
      Q_ASSERT(item != 0);
      if (pixmap.width() > view->maximumViewportSize().width())
	view->fitInView(item->boundingRect(), Qt::KeepAspectRatio);
      view_to_item(view) = item;

      bool b = (mainResultItem_ != 0 || mainRefItem_ != 0);
      zoomLabel->setEnabled(b);
      zoomIn->setEnabled(b);
      zoomOut->setEnabled(b);
      zoomFixed->setEnabled(b);
      zoomOriginal->setEnabled(b);
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
	  if (is_in_ocr_mode() || is_in_doc_mode())
	  {
	    QFile f(last_output_);
	    f.open(QIODevice::ReadOnly);
	    textBrowser->setText(QString::fromUtf8(f.readAll().data()));
//	    textBrowser->setText(exec_.readAllStandardOutput());
	    on_displayBtn_clicked(true);
	    tabWidget->setCurrentIndex(1);
	  }
	  else
	    update_pixmap(imageResult, last_output_);
	}
	// Else : canceled.
      }
      else
	exec_error(tr("Error during last run."));

      imageResult->setFocus();
      if (autoDemo->isChecked())
	timer_.start(timeoutDelay->text().toInt() * 1000);
    }


    void MainWindow::setup_scene()
    {
      QGraphicsScene *scene = new QGraphicsScene();
      imageResult->setScene(scene);

      scene = new QGraphicsScene();
      imageRef->setScene(scene);

      update_status_message(tr("1) Choose an algorithm.\n\
2) Select an image.\n3) Run."));

      imageResult->show();
      imageRef->show();
    }


    void MainWindow::wait_for_result()
    {
      reset_progress_dialog();

      pdialog_.show();
      setEnabled(false);

      if (!is_in_ocr_mode() && !is_in_ocr_mode())
	update_status_message("");
    }


    void MainWindow::update_status_message(const QString& msg)
    {
      if (mainResultItem_ != 0)
      {
	imageResult->scene()->removeItem(mainResultItem_);
	delete mainResultItem_;
      }
      mainResultItem_ = imageResult->scene()->addText(msg);
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
      if (error == QProcess::FailedToStart)
        exec_error(tr("This program does not exist: ")
		    + exec_prefix_ + modes[mode_][2]);
      else
        exec_error(tr("The computation stopped.")
		    + exec_prefix_ + modes[mode_][2]);
    }


    void MainWindow::context_changed(QAction *action)
    {
      mode_ = action->data().toInt();
      filepath->clear();
      tabWidget->setTabEnabled(1, is_in_ocr_mode() || is_in_doc_mode());
    }


    void MainWindow::compute_canceled()
    {
      setEnabled(true);

      exec_.disconnect();
      exec_.kill();
      connect_compute_process();

      update_status_message("Computation canceled.");
    }


    bool MainWindow::is_in_ocr_mode() const
    {
      return current_mode() == "ocr";
    }

    bool MainWindow::is_in_pics_mode() const
    {
      return current_mode() == "pics";
    }

    bool MainWindow::is_in_doc_mode() const
    {
      return current_mode() == "doc";
    }


    void MainWindow::resize_pixmaps(const QRectF& rect)
    {
      if (mainRefItem_ != 0)
	imageRef->fitInView(rect, Qt::KeepAspectRatio);
      if (mainResultItem_ != 0)
	imageResult->fitInView(rect, Qt::KeepAspectRatio);
    }

    void MainWindow::move_vertical_sliders(int value)
    {
      imageRef->verticalScrollBar()->setValue(value);
      imageResult->verticalScrollBar()->setValue(value);
    }

    void MainWindow::move_horizontal_sliders(int value)
    {
      imageRef->horizontalScrollBar()->setValue(value);
      imageResult->horizontalScrollBar()->setValue(value);
    }


    void MainWindow::on_zoomFixed_clicked()
    {
      resize_pixmaps(mainRefItem_->boundingRect());
    }

    void MainWindow::on_zoomOriginal_clicked()
    {
      resize_pixmaps(imageResult->viewport()->geometry());
    }

    void MainWindow::on_autoDemo_clicked(bool checked)
    {
      if (checked)
	timer_.start(100);
      else
	timer_.stop();
    }

    void MainWindow::resizeEvent(QResizeEvent * event)
    {
      if (mainRefItem_ != 0)
	resize_pixmaps(mainRefItem_->boundingRect());
      event->ignore();
    }

    void MainWindow::keyPressEvent(QKeyEvent *event)
    {
      if (event->text() == QString("+"))
      {
	on_zoomIn_clicked();
	event->accept();
      }
      else if (event->text() == QString("-"))
      {
	on_zoomOut_clicked();
	event->accept();
      }
      else
	event->ignore();
    }

    QString MainWindow::current_mode() const
    {
      return modes[mode_][1];
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
      QString tmp_path = base_img_dir_ + "/" + current_mode();
      if (demoDir_.path() != tmp_path)
      {
	demoDir_.setPath(tmp_path);
	demo_index_ = 0;
	demo_files_ = demoDir_.entryList(QDir::NoDotAndDotDot | QDir::Files);
      }
    }


    void MainWindow::update_process_status()
    {
      if (is_in_pics_mode() || is_in_doc_mode())
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
