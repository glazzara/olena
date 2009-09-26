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

#ifndef   	VIEWER_HH_
# define   	VIEWER_HH_

# include <QtGui>
# include "common.hh"

class ImageScene;
class DomModel;
class KeyWidget;

class Viewer
  : public QObject
{
  Q_OBJECT

public:
  ~Viewer();

  static Viewer* Instance(int &argc, char** argv);

  int exec();

public slots:
  // Load the file as an image, load the layout if xml with the same
  // name is found.
  void load(QString filename);
  void help();

  void setAltCache(bool b);

signals:
  void updated(DomModel* model);
  void key_updated(int key, bool checked);
  void setOutline(bool b);
  void setPrecise(bool b);
  void setFill(bool b);

private:
  Viewer(int &argc, char** argv);
  Viewer();

  QApplication* app_;
  QMainWindow* win_;

  QGraphicsPixmapItem* image_;

  ImageScene* scene_;
  QDirModel* files_;
  DomModel* doc_layout_;

  QAction* outline_action_;
  QAction* fill_action_;
  QAction* precise_action_;

  KeyWidget* key_wgt_;

  QMap<QString, int> region_ids_;
  region::KeyMap key_map_;

  bool alt_cache_;
};

#include "viewer.hxx"

#endif	    /* !VIEWER_HH_ */
