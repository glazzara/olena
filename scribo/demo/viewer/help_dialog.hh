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

#ifndef   	HELP_DIALOG_HH_
# define   	HELP_DIALOG_HH_

# include <QtGui>

class HelpDialog
  : public QDialog
{
  Q_OBJECT

public:
  HelpDialog();

public slots:
  void done();
};

#endif	    /* !HELP_DIALOG_HH_ */
