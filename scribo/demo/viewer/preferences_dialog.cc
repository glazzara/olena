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

#include "preferences_dialog.hh"
#include "toolchain_options.hh"
#include "general_options.hh"



preferences_dialog::preferences_dialog(QWidget *parent)
  : QDialog(parent)
{
  setupUi(this);

  // We may want to remove this useless object in the ui file.
  delete widget;

  setAttribute(Qt::WA_DeleteOnClose);

  load_option_list();
  optionList->setCurrentRow(0);
}

preferences_dialog::~preferences_dialog()
{
}


void preferences_dialog::load_option_list()
{
  static const char *options[] = { "General", "Toolchain", 0 };

  int i;
  for (i = 0; options[i]; ++i)
    optionList->insertItem(i, options[i]);

  widgets_.fill(0, i);
}

void preferences_dialog::on_optionList_currentRowChanged(int row)
{
  select_option_widget(row);
}


void preferences_dialog::select_option_widget(int row)
{
  if (row >= widgets_.size())
  {
    qDebug() << "select_option_widget - Hu? Something wrong... Invalid row";
    return;
  }

  if (!widgets_[row])
  {
    switch (row)
    {
      case 0:
	widgets_[0] = new general_options(this);
	break;

      case 1:
	widgets_[1] = new toolchain_options(this);
	break;

      default:
	qDebug() << "select_option_widget - Hu? Something wrong...";
    }
  }

  if (horizontalLayout_2->count() == 2)
  {
    QWidget *current_widget = horizontalLayout_2->itemAt(1)->widget();
    horizontalLayout_2->removeWidget(current_widget);
    current_widget->hide();
  }

  horizontalLayout_2->insertWidget(1, widgets_[row]);
  widgets_[row]->show();
}


void preferences_dialog::accept()
{
  for (int i = 0; i < widgets_.size(); ++i)
    if (widgets_[i])
    {
      static_cast<OptionWidget *>(widgets_[i])->save_config();
      delete widgets_[i];
    }
  QDialog::accept();
}


void preferences_dialog::reject()
{
  for (int i = 0; i < widgets_.size(); ++i)
    delete widgets_[i];
  QDialog::reject();
}
