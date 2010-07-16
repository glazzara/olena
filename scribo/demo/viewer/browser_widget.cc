//
// Document layout viewer.
//
// Copyright (C) 2009 Florent D'Halluin.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//

#include "browser_widget.hh"

BrowserWidget::BrowserWidget(QDirModel* files, QString dir)
  : files_(files),
    view_(new QListView()),
    path_(new QLineEdit("")),
    first_time_(true)
{
  QToolButton* next= new QToolButton();
  QToolButton* prev= new QToolButton();
  next->setArrowType(Qt::RightArrow);
  prev->setArrowType(Qt::LeftArrow);

  QLabel* title = new QLabel(tr("Images"));
  title->setAlignment(Qt::AlignHCenter);

  QVBoxLayout* layout = new QVBoxLayout;
  QHBoxLayout *hlayout = new QHBoxLayout;

  hlayout->addWidget(prev);
  hlayout->addWidget(title);
  hlayout->addWidget(next);
  layout->addLayout(hlayout);
  layout->addWidget(path_);
  layout->addWidget(view_);

  //setLayout(hlayout);
  setLayout(layout);

  view_->setModel(files);
  files->setFilter(QDir::AllDirs | QDir::Files
		   | QDir::Readable | QDir::Drives);

  view_->setRootIndex(files->index(QDir::currentPath()));

  QDir d(dir);
  if (d.isReadable())
    view_->setRootIndex(files->index(d.absolutePath()));

  view_->setRowHidden(0, true);

  path_->setText(files->filePath(view_->rootIndex()));

  connect(view_, SIGNAL(activated(const QModelIndex&)),
	  this, SLOT(activate(const QModelIndex&)));
  connect(next, SIGNAL(released ()),
	  this, SLOT(next()));
  connect(prev, SIGNAL(released ()),
	  this, SLOT(prev()));
  connect(path_, SIGNAL(returnPressed()),
	  this, SLOT(path_return_pressed()));

  QStringList files_filters;
  files_filters << "*.png" << "*.jpg"
		<< "*.tif" << "*.ppm"
		<< "*.pgm" << "*.pbm"
		<< "*.pnm" << "*.xmle";

  files->setNameFilters(files_filters);
}

void
BrowserWidget::path_return_pressed()
{
  QString path = path_->text();
  QDir d(path);

  if (d.isReadable())
    activate(files_->index(QString(path)), false);
}

void
BrowserWidget::activate(const QModelIndex& index, bool b)
{
  if (files_->isDir(index))
  {
    view_->setRootIndex(index);
    view_->setRowHidden(0, true);
    path_->setText(files_->filePath(view_->rootIndex()));
    first_time_ = true;
    return;
  }

  first_time_ = false;

  if (files_->filePath(index).endsWith(".xmlc"))
    emit activated(files_->filePath(index), b, true);
  else
    emit activated(files_->filePath(index), b, false);
}

void
BrowserWidget::change_pos(bool next)
{
  QModelIndex index = view_->currentIndex();

  if(next)
    index = files_->index(index.row() + 1, index.column(), index.parent());
  else
    index = files_->index(index.row() - 1, index.column(), index.parent());

  if (index.isValid())
    {
      if (!files_->isDir(index))
	if (!first_time_)
	  activate(index, true);
      view_->setCurrentIndex(index);
    }
}

BrowserWidget::~BrowserWidget()
{
}
