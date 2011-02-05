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

# include "step_widget.hh"
# include "config.hh"

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

StepWidget::~StepWidget()
{
}


void StepWidget::activate(QListWidgetItem* item)
{
  QString key, value;

  StepQMap::iterator iter = map_.find(item->text());

  if (iter != map_.end())
  {
    view_->setCurrentItem(item);
    step_ = item->text();
    key = iter.key();
    value = iter.value();

    qDebug() << "Loading " << value;
    emit load_xml(value);
  }
  else
    qDebug() << "Step not found!";

  emit step_selected(view_->count());
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


      QFileInfo f(file);
      file_with_no_ext_ = f.baseName();

      QStringList dirlist;
      dirlist << f.absolutePath() << QDir::tempPath();

      // Set directories to look at according to settings.
      config * const conf = config::get_instance();
      if (conf->general_save_xml_custom_dir())
	dirlist << conf->general_save_xml_custom_dir_path();

      foreach(QString path, dirlist)
      {
	QDir dir(path);

	if (dir.isReadable())
	{
	  QStringList filter;
	  filter << "*.xml";
	  QFileInfoList xml_list = dir.entryInfoList(filter);
	  for (int i = 0; i < xml_list.size(); ++i)
	    insert_new_entry(xml_list.at(i));
	}
      }

      if ( (step && step_ != QString::Null()))
	{
	  QList<QListWidgetItem*> list = view_->findItems(step_, Qt::MatchContains);

	  if (!list.isEmpty())
	    emit activated(list.first());
	}
      else
      {
	step_ = QString::Null();
	if (view_->count())
	  emit activated(view_->item(0));
      }
    }
}


QListWidgetItem* StepWidget::insert_new_entry(const QFileInfo& file)
{
  QListWidgetItem *item  = 0;

  if (file.fileName().startsWith(file_with_no_ext_))
  {
    QString key = file.baseName();
    key.replace(file_with_no_ext_ + QString("_"), QString(""));
    key.replace(QRegExp("^step([0-9])"), "Step \\1");
    key.replace(QRegExp("^Step ([0-9])_"), "Step \\1 : ");
    key.replace("_", " ");

    bool exists = (map_.find(key) != map_.end());

    map_.insertMulti(key, file.absoluteFilePath());

    if (!exists)
    {
      item = new QListWidgetItem(key);
      view_->addItem(item);
    }
    else
      item = view_->findItems(key, Qt::MatchCaseSensitive).at(0);
  }

  return item;
}


QListWidgetItem* StepWidget::add_element(const QString& element)
{
  QListWidgetItem *item = new QListWidgetItem(element);
  view_->addItem(item);

  return item;
}

QString StepWidget::current() const
{
  return map_.value(step_);
}






