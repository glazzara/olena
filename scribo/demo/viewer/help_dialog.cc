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

#include "help_dialog.hh"

HelpDialog::HelpDialog()
{
  QPushButton *closeButton = new QPushButton(tr("Close"));
  connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

  QHBoxLayout *buttonsLayout = new QHBoxLayout;
  buttonsLayout->addStretch(1);
  buttonsLayout->addWidget(closeButton);

  QVBoxLayout *mainLayout = new QVBoxLayout;

  QString help = tr(
		    "Document layout viewer:\n"
		    "\n"
		    "Usage:\n"
		    " - Choose an image file in the file browser,\n"
	 	    "if XML file(s) whose prefix name is the same as\n"
		    "the image name (i.e.image _name(_ .*)?.xml) is present\n"
		    "then you can choose the one you want in the step\n"
		    "chooser to display it on the scene.\n"
		    "- To load a base 64 encoded XML file,\n"
		    "its extension has to be .xmlc otherwise \n"
		    "the program won't recognize it !\n"
		    " - Select regions to display their properties.\n"
		    " - Use the mouse or keyboard to move and zoom\n"
		    "   (Arrows, PageUp, PageDown, Home, End).\n"
		    "\n");


  QLabel* label = new QLabel(help);

  mainLayout->addWidget(label);
  mainLayout->addLayout(buttonsLayout);

  setLayout(mainLayout);
  setWindowTitle(tr("About"));
}

void HelpDialog::done()
{
  emit close();
}
