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
# include <QDomNode>

class ImageScene;
class DomModel;
class KeyWidget;
class ImageRegion;

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
  void load(QString filename, bool b);
  void help();
  void load_xml(QString filename);
  void maybeChangeCacheMode(qreal scale);
  void useCache(bool b);
  void useExtended(bool b);
  void useText(bool b);
  void useImage(bool b);
  void change_base(bool b);

signals:
  void updated();
  void key_updated(int key, bool checked);
  void setOutline(bool b);
  void setPrecise(bool b);
  void setFill(bool b);
  void mode_changed(bool b);
  void fill_xml(QString);

private:
  Viewer(int &argc, char** argv);
  Viewer();

  void xml_to_layout();
  QPixmap  load_base64(QString xml);
  void add_region(QDomNode father, QString attr_id);
  void add_text(QDomNode line, QDomNode region);

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

  bool no_cache_;
  bool extended_mode_;
  QString xml_file_;
  bool base64_;
  bool text_;
  bool use_image_;
  QVector<QGraphicsTextItem *> text_vector_;
  QVector<QGraphicsPixmapItem*> image_vector_;
};

#include "viewer.hxx"

#endif	    /* !VIEWER_HH_ */
