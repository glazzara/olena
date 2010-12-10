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

#ifndef SCRIBO_DEMO_VIEWER_PREFERENCES_DIALOG_HH
# define SCRIBO_DEMO_VIEWER_PREFERENCES_DIALOG_HH

# include <QtGui>
# include <preferences_dialog.ui.h>

class preferences_dialog : public QDialog, private Ui::PreferencesDialog
{
  Q_OBJECT;

public:
  preferences_dialog(QWidget *parent = 0);
  ~preferences_dialog();

private slots:
  void on_optionList_currentRowChanged(int row);
  virtual void accept();
  virtual void reject();

private: // Methods
  void load_option_list();
  void select_option_widget(int row);

private: // Attributes
  QVector<QWidget *> widgets_;

};

#endif // ! SCRIBO_DEMO_VIEWER_PREFERENCES_DIALOG_HH
