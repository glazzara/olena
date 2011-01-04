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


#ifndef SCRIBO_DEMO_VIEWER_STEP_WIDGET_HH_
# define SCRIBO_DEMO_VIEWER_STEP_WIDGET_HH_

# include <QtGui>

typedef QMap<QString, QString> StepQMap;

class StepWidget
  : public QWidget
{
  Q_OBJECT

public:
  StepWidget();
  ~StepWidget();
  QListWidgetItem* add_element(const QString& element);

  QString current() const;

signals:
  void load_image(QString, bool);
  void load_xml(QString);
  void activated(QListWidgetItem*);
  void change_base(bool);
  void step_selected(bool);

public slots:
  void fill_steps(QString file, bool step = false, bool container = false);
  void activate(QListWidgetItem* item);
  QListWidgetItem* insert_new_entry(const QFileInfo& file);

private:
  QListWidget* view_;
  StepQMap map_;
  QString step_;
  QString file_with_no_ext_;
};

#endif // ! SCRIBO_DEMO_VIEWER_STEP_WIDGET_HH_
