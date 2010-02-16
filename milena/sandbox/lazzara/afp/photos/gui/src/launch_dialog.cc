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
#include <src/launch_dialog.hh>
#include <src/custom_delegate.hh>

namespace mln
{

  namespace demo
  {

    launch_dialog::launch_dialog(QWidget *parent)
      : QDialog(parent)
    {
      setupUi(this);
    }

    launch_dialog::~launch_dialog()
    {

    }


    void
    launch_dialog::on_browseBtn_clicked(bool)
    {
      QString dirname = QFileDialog::getExistingDirectory(this, "");

      if (!dirname.isEmpty())
      {
	pathToPics->setText(dirname);
      }
    }


    void
    launch_dialog::on_dialogBtnBox_accepted()
    {
      emit ok_clicked(pathToPics->text(), checkBox->isChecked());
    }


    void
    launch_dialog::on_pathToPics_textChanged(const QString& str)
    {
      dialogBtnBox->button(QDialogButtonBox::Ok)->setEnabled(!str.isEmpty());
    }

  } // end of namespace scribo::demo

} // end of namespace scribo


