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
