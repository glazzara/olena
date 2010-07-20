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

# include "step_widget.hh"

StepWidget::StepWidget()
  : view_(new QListWidget()),
    step_(QString::Null())
{
  QLabel* title = new QLabel(tr("Steps"));
  title->setAlignment(Qt::AlignHCenter);

  QVBoxLayout* layout = new QVBoxLayout;

  layout->addWidget(title);
  layout->addWidget(view_);

  view_->setSortingEnabled(true);

  setLayout(layout);

  connect(view_, SIGNAL(itemActivated(QListWidgetItem*)),
	  this, SLOT(activate(QListWidgetItem*)));

  connect(this, SIGNAL(activated(QListWidgetItem*)),
	  this, SLOT(activate(QListWidgetItem*)));
}

void StepWidget::activate(QListWidgetItem* item)
{
  QString key, value;

  StepQMap::iterator iter = map_.find(item->text());

  view_->setCurrentItem(item);
  if (iter != map_.end())
    {
      step_ = item->text();
      key = iter.key();
      value = iter.value();

      emit load_xml(value);
    }
  else
    qDebug() << "Step not found!";

}

void StepWidget::fill_steps(QString file, bool step, bool container)
{
  view_->clear();
  map_.clear();

  if (container)
    {
      emit change_base(true);
      //      emit load_image(file, true);
      emit load_xml(file);
    }
  else
    {
      emit change_base(false);
      // image is loaded once
      emit load_image(file, false);

      int cut = file.lastIndexOf(QChar('/'));
      QString path = file.left(cut+1);
      QString filename = file.mid(cut+1);

      cut = filename.lastIndexOf(QChar('.'));

      QString file_with_no_ext = filename.left(cut);
      //  view_->addItem(file_with_no_ext);

      QDir dir(path);

      if (dir.isReadable())
	{
	  QStringList filter;
	  filter << "*.xml";
	  QStringList xml_list = dir.entryList(filter);
	  for (int i = 0; i < xml_list.size(); ++i)
	    {
	      if (xml_list.at(i).startsWith(file_with_no_ext))
		{
		  cut = xml_list.at(i).lastIndexOf(QChar('.'));
		  QString key = xml_list.at(i).left(cut);
		  key.replace(file_with_no_ext + QString("_"), QString(""));
		  key.replace(QRegExp("^step([0-9])"), "Step \\1");
		  key.replace(QRegExp("^Step ([0-9])_"), "Step \\1 : ");
		  key.replace("_", " ");
		  QString value = path;
		  map_.insertMulti(key, value.append(xml_list.at(i)));
		  view_->addItem(key);
		}
	    }
	}

      if ( (step && step_ != QString::Null()))
	{
	  QList<QListWidgetItem*> list = view_->findItems(step_, Qt::MatchContains);

	  if (!list.isEmpty())
	    emit activated(list.first());
	}
      else
	step_ = QString::Null();
    }
}

void StepWidget::add_element(const QString& element)
{
  view_->addItem(element);
}

StepWidget::~StepWidget()
{
}











