//
// Document layout viewer.
//
// Copyright (C) 2009 Florent D'Halluin.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//

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
		    " - Choose an image file in the file browser\n"
		    " - If the document layout is present\n"
		    "   (XML file with the same name),\n"
		    "   select regions to display their properties.\n"
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
