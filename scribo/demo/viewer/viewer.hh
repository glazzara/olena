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

#ifndef SCRIBO_DEMO_VIEWER_VIEWER_HH
# define SCRIBO_DEMO_VIEWER_VIEWER_HH

# include <QtGui>
# include <QDomNode>
# include "common.hh"
# include "runner.hh"

class ImageScene;
class DomModel;
class KeyWidget;
class ImageRegion;
class StepWidget;
class BrowserWidget;


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

private slots:
  void on_preferences();
  void run_process();
  void run_progress();
  void on_xml_saved(const QString& filename);
  void export_as();

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
  void add_text(QDomNode line);
  QAction *create_action(QString name, QMenu* menu, QString status, QString shortcut);
  void reset_progress_dialog();

  QApplication* app_;
  QMainWindow* win_;
  StepWidget* step_widget_;
  BrowserWidget* browser_wgt_;

  QGraphicsPixmapItem* image_;

  ImageScene* scene_;
  QDirModel* files_;
  DomModel* doc_layout_;

  QAction* outline_action_;
  QAction* fill_action_;
  QAction* precise_action_;
  QAction *export_action_;

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
  QString current_image_;

  QProgressDialog pdialog_;
  runner runner_;

  QSet<QString> tmp_files_to_remove_;
};

#include "viewer.hxx"

#endif // ! SCRIBO_DEMO_VIEWER_VIEWER_HH
