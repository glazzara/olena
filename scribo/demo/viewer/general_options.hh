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

#ifndef SCRIBO_DEMO_VIEWER_GENERAL_OPTIONS_HH
# define SCRIBO_DEMO_VIEWER_GENERAL_OPTIONS_HH

# include <QtGui>
# include <general_options.ui.h>
# include "option_widget.hh"

class general_options : public OptionWidget, private Ui::GeneralOptions
{
  Q_OBJECT;

public:
  general_options(QWidget *parent = 0);
  ~general_options();

  virtual void load_config();
  virtual void save_config();

private slots:
  void on_customDirBrowseBtn_clicked();

};

#endif // ! SCRIBO_DEMO_VIEWER_GENERAL_OPTIONS_HH
