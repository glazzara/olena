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
    path_(new QLabel(""))
{
  QLabel* title = new QLabel(tr("Images"));
  title->setAlignment(Qt::AlignHCenter);

  QVBoxLayout* layout = new QVBoxLayout;
  layout->addWidget(title);
  layout->addWidget(path_);
  layout->addWidget(view_);

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

  QStringList files_filters;
  files_filters << "*.png" << "*.jpg"
		<< "*.tif" << "*.ppm" << "*.pgm" << "*.pbm";
  files->setNameFilters(files_filters);
}

void
BrowserWidget::activate(const QModelIndex& index)
{
  if (files_->isDir(index))
  {
    view_->setRootIndex(index);
    view_->setRowHidden(0, true);
    path_->setText(files_->filePath(view_->rootIndex()));
    return;
  }
  emit activated(files_->filePath(index));
}

BrowserWidget::~BrowserWidget()
{
}
