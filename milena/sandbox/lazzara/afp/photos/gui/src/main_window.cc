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
#include <src/main_window.hh>
#include <src/launch_dialog.hh>
#include <src/custom_delegate.hh>

namespace mln
{

  namespace demo
  {

    main_window::main_window(const QString& text_in_photo_ppm, QWidget *parent)
      : QMainWindow(parent), text_in_photo_ppm_(text_in_photo_ppm)
    {
      setupUi(this);

      // Temporary output directory
      output_dir_ = "/tmp/";

      // Setup directory listing
      QStringList filters;
      filters << "*.ppm"
	      << "*.pgm"
	      << "*.pbm"
	      << "*.jpg"
	      << "*.png"
	      << "*.tiff";
      dir_.setNameFilters(filters);
      dir_.setFilter(QDir::Files);
      dir_.setSorting(QDir::Name);

      // Setup Signal/Slots connections
      connect(&process_, SIGNAL(error(QProcess::ProcessError)),
	      this, SLOT(process__error_slot(QProcess::ProcessError)));
      connect(&process_,
	      SIGNAL(finished(int, QProcess::ExitStatus)),
	      this,
	      SLOT(process__finished_slot(int, QProcess::ExitStatus)));

      // Setup graphicsView
      graphicsView->setScene(new QGraphicsScene());


      // Customize item view delegate for picture with text.
      withTextListWidget->setItemDelegate(new custom_delegate());

      // Setup Toolbar.
      toolbar_ = new QToolBar(tr("Action"));
      toolbar_->setToolButtonStyle(Qt::ToolButtonIconOnly);
      QAction *removeImage = new QAction(QIcon(":/icons/remove.png"),
					 tr("Tag as image without text"),
					 toolbar_);
      connect(removeImage, SIGNAL(triggered(bool)),
	      this, SLOT(remove_image(bool)));
      toolbar_->addAction(removeImage);

      addToolBar(toolbar_);


      // Setup StatusBar
      progressToolBar_ = new QToolBar(tr("ProgressBar"));

      progressToolBar_->addWidget(&progressLabel_);
      progressToolBar_->addWidget(&progressBar_);

      QPushButton *cancelBtn = new QPushButton(tr("&Cancel"));
      connect(cancelBtn, SIGNAL(clicked(bool)),
	      this, SLOT(cancel_clicked(bool)));
      progressToolBar_->addWidget(cancelBtn);

      addToolBar(progressToolBar_);
      progressToolBar_->hide();



      // Hide visualization buttons
      textOnlyButton->hide();
      fullImageButton->hide();
      textBoxesButton->hide();
      textMaskButton->hide();

      // Set default visualization.
      textBoxesButton->setChecked(true);
      actionText_Boxes->setChecked(true);
      QActionGroup * actionGroup = new QActionGroup(this);
      actionGroup->addAction(actionText_Boxes);
      actionGroup->addAction(actionFull_Image);
      actionGroup->addAction(action_Text_Only);
      actionGroup->addAction(actionText_Mask);
      actionGroup->setExclusive(true);
    }

    main_window::~main_window()
     {
      if (process_.state() == QProcess::Running)
      {
	process_.disconnect();
	process_.close();
      }
    }

    void
    main_window::on_actionScan_directory_triggered()
    {
      launch_dialog *dialog = new launch_dialog(this);
      connect(dialog, SIGNAL(ok_clicked(const QString&, const QString&)),
	      this, SLOT(init_and_start_process(const QString&, const QString&)));
      dialog->show();
    }

   void
   main_window::init_and_start_process(const QString& dirname,
				       const QString& options)
   {
     dir_.setPath(dirname);

     file_list_ = dir_.entryInfoList();
     current_file_ = 0;
     withTextListWidget->clear();
     withoutTextListWidget->clear();

     progressToolBar_->show();
     progressLabel_.setText(tr("Processing images..."));
     progressBar_.setRange(0, file_list_.size());
     progressBar_.setValue(0);

     options_ = options;

     if (current_file_ < file_list_.size())
       start_process();
     else
     {
       QMessageBox::critical(this, "No image found!", QString("This application could not find any supported images in the provided directory. Supported file types are: %1").arg(dir_.nameFilters().join(" ")));
       stop_process();
     }
   }

    void
    main_window::next_process()
    {
      ++current_file_;
      progressBar_.setValue(current_file_);

      tabWidget->setTabText(0,
			    QString(tr("With text (%1)"))
			    .arg(withTextListWidget->count()));
      tabWidget->setTabText(1,
			    QString(tr("Without text (%1)"))
			    .arg(withoutTextListWidget->count()));

      if (current_file_ < file_list_.size())
	start_process();
      else
	stop_process();
    }


    QString
    main_window::output_file(const QString& basename)
    {
      return output_dir_ + basename + "_out_text.ppm";
    }

    QString
    main_window::textBoxes_file(const QString& basename)
    {
      return output_dir_ + basename + "_input_with_bboxes.ppm";
    }

    QString
    main_window::textMask_file(const QString& basename)
    {
      return output_dir_ + basename + "_out.ppm";
    }


    void
    main_window::start_process()
    {
      QStringList args;
      args << file_list_.at(current_file_).absoluteFilePath()    // input file.
	   << textMask_file(file_list_.at(current_file_).baseName()); // output file.

      // Enable/Disable remove background + Enable/Disable sauvola_ms.
      QStringList options = options_.split(' ');
      foreach(QString option, options)
	args << option;

      args << "1" << "1" << "1" // Enable all steps.
	   << "/tmp/" + file_list_.at(current_file_).baseName();

      process_.start(text_in_photo_ppm_, args);
    }

    void
    main_window::stop_process()
    {
      progressToolBar_->hide();
      emit process_finished();
    }

    void
    main_window::process__error_slot(QProcess::ProcessError)
    {
      qDebug() << "error";
    }

    void
    main_window::process__finished_slot(int exitCode,
					QProcess::ExitStatus exitStatus)
    {
      QString filename = file_list_.at(current_file_).fileName();

      if (exitStatus == QProcess::CrashExit)
      {
	qDebug() << "Process crashed with file "
		 << file_list_.at(current_file_).fileName();
      }
      else
      {
	QFileInfo f(filename);
	QFileInfo text_image();
	QPixmap pixmap(output_file(f.baseName()));

	QListWidgetItem *item;
	if (! pixmap.isNull())
	  item = new QListWidgetItem(QIcon(pixmap.scaled(150, 150, Qt::KeepAspectRatio)), filename + QString(" (%1)").arg(exitCode));
	else
	  item = new QListWidgetItem(filename + QString(" (%1)").arg(exitCode));

	item->setData(Qt::UserRole, QVariant(current_file_));

	if (exitCode)
	{
	  item->setCheckState(Qt::Unchecked);
	  item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

	  item->setSizeHint(QSize(170, 200));
	  withTextListWidget->addItem(item);
	  // Fixme: update tab title with number of photos.
	  if (withTextListWidget->count() == 1)
	  {
	    withTextListWidget->setCurrentRow(0, QItemSelectionModel::SelectCurrent);
	    on_withTextListWidget_itemClicked(withTextListWidget->item(0));
	  }
	}
	else
	  // Fixme: update tab title with number of photos.
	  withoutTextListWidget->addItem(item);
      }

      next_process();
    }


    void
    main_window::display_image(const QString& filename)
    {
      graphicsView->scene()->clear();

      QPixmap pixmap(filename);
      QGraphicsPixmapItem *item = graphicsView->scene()->addPixmap(pixmap);

      graphicsView->setSceneRect(item->boundingRect());

      if (item->boundingRect().contains(graphicsView->viewport()->geometry()))
	graphicsView->fitInView(item->boundingRect(), Qt::KeepAspectRatio);
      else
	graphicsView->fitInView(graphicsView->viewport()->geometry(),
				Qt::KeepAspectRatio);
    }


    void
    main_window::on_withTextListWidget_itemClicked(QListWidgetItem * item)
    {
      if (fullImageButton->isChecked())
	on_fullImageButton_toggled(true);
      else if (textOnlyButton->isChecked())
	on_textOnlyButton_toggled(true);
      else if (textBoxesButton->isChecked())
	on_textBoxesButton_toggled(true);
      else if (textMaskButton->isChecked())
	on_textMaskButton_toggled(true);
      else
      {
	QFileInfo f(item->text());
	display_image(output_file(f.baseName()));
      }
    }

    void
    main_window::on_withoutTextListWidget_itemClicked(QListWidgetItem * item)
    {
      display_image(file_list_.at(item->data(Qt::UserRole).toInt()).absoluteFilePath());
    }

    void
    main_window::on_fullImageButton_toggled(bool)
    {
      if (withTextListWidget->count())
      {
	QString filename = file_list_.at(withTextListWidget->currentItem()->data(Qt::UserRole).toInt()).absoluteFilePath();
	display_image(filename);
      }
    }

    void
    main_window::on_textBoxesButton_toggled(bool)
    {
      if (withTextListWidget->count())
      {
	QFileInfo f(withTextListWidget->currentItem()->text());
	display_image(textBoxes_file(f.baseName()));
      }
    }

    void
    main_window::on_textMaskButton_toggled(bool)
    {
      if (withTextListWidget->count())
      {
	QFileInfo f(withTextListWidget->currentItem()->text());
	display_image(textMask_file(f.baseName()));
      }
    }

    void
    main_window::on_textOnlyButton_toggled(bool)
    {
      if (withTextListWidget->count())
      {
	QFileInfo f(withTextListWidget->currentItem()->text());
	display_image(output_file(f.baseName()));
      }
    }



    void
    main_window::on_tabWidget_currentChanged(int index)
    {
      bool b = (index == 0);
      fullImageButton->setEnabled(b);
      textOnlyButton->setEnabled(b);
      textBoxesButton->setEnabled(b);
      textMaskButton->setEnabled(b);
      toolbar_->setEnabled(b);

      if (index == 1)
      {
	graphicsView->scene()->clear();
	if (withoutTextListWidget->count())
	{
	  withoutTextListWidget->setCurrentRow(0, QItemSelectionModel::Select);
	  on_withoutTextListWidget_itemClicked(withoutTextListWidget->item(0));
	}
      }
    }

    void
    main_window::cancel_clicked(bool)
    {
      process_.disconnect();
      process_.kill();
      process_.waitForFinished();
      progressBar_.setValue(progressBar_.maximum());
      connect_process();
      progressToolBar_->hide();
    }

    void
    main_window::remove_image(bool)
    {
      QList<QListWidgetItem *> items = withTextListWidget->selectedItems();
      foreach(QListWidgetItem *item, items)
	delete withTextListWidget->takeItem(withTextListWidget->row(item));

      if (withTextListWidget->count())
	withTextListWidget->setCurrentRow(0, QItemSelectionModel::Select);
    }


    void main_window::connect_process()
    {
      connect(&process_, SIGNAL(error(QProcess::ProcessError)),
	      this, SLOT(process__error_slot(QProcess::ProcessError)));
      connect(&process_,
	      SIGNAL(finished(int, QProcess::ExitStatus)),
	      this,
	      SLOT(process__finished_slot(int, QProcess::ExitStatus)));
    }


    void
    main_window::keyPressEvent(QKeyEvent * event)
    {
      if (tabWidget->currentIndex() == 0)
      {
	switch(event->key())
	{
	  case Qt::Key_F1:
	    on_textOnlyButton_toggled(true);
	    break;
	  case Qt::Key_F2:
	    on_fullImageButton_toggled(true);
	    break;
	  case Qt::Key_F3:
	    on_textBoxesButton_toggled(true);
	    break;
	  case Qt::Key_F4:
	    on_textMaskButton_toggled(true);
	    break;
	}
      }
      else
	event->ignore();
    }

    void
    main_window::resizeEvent(QResizeEvent* event)
    {
      if (graphicsView->scene()->items().size())
	graphicsView->fitInView(graphicsView->scene()->items().at(0)->boundingRect(),
				Qt::KeepAspectRatio);

      event->ignore();
    }

  } // end of namespace scribo::demo

} // end of namespace scribo


