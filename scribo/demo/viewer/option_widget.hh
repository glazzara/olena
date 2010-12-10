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

#ifndef SCRIBO_DEMO_VIEWER_OPTION_WIDGET_HH
# define SCRIBO_DEMO_VIEWER_OPTION_WIDGET_HH

#include <QWidget>

struct OptionWidget : public QWidget
{
  OptionWidget(QWidget * parent);

  virtual void load_config();
  virtual void save_config();
};

#endif // ! SCRIBO_DEMO_VIEWER_OPTION_WIDGET_HH
