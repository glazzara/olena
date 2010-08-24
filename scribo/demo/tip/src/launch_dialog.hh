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

#ifndef MLN_DEMO_SRC_LAUNCH_DIALOG_HH

# include <QtGui>
# include <ui_launch_dialog.h>

namespace mln
{

  namespace demo
  {


    class launch_dialog : public QDialog, private Ui::LaunchDialog
    {
      Q_OBJECT

    public:
      launch_dialog(QWidget *parent = 0);
      ~launch_dialog();

    private slots:
      void on_dialogBtnBox_accepted();
      void on_browseBtn_clicked(bool);
      void on_pathToPics_textChanged(const QString& str);

    signals:
      void ok_clicked(const QString& path, const QString& options);

    private: // members
      QString get_options() const;
    };


  } // end of namespace mln::demo

} // end of namespace mln

#endif // ! MLN_DEMO_SRC_LAUNCH_DIALOG_HH
