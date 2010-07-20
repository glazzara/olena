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

#include "viewer.hh"
#include "key_widget.hh"
#include "browser_widget.hh"
#include "image_widget.hh"
#include "xml_widget.hh"
#include "step_widget.hh"
#include "image_scene.hh"
#include "image_region.hh"
#include "help_dialog.hh"
#include <limits.h>

#include "common.hh"

Viewer::Viewer(int &argc, char** argv)
  : app_ (new QApplication(argc, argv)),
    win_ (new QMainWindow()),
    image_(0),
    scene_(new ImageScene()),
    files_(new QDirModel()),
    doc_layout_(0),
    key_map_(11),
    no_cache_(false),
    extended_mode_(false),
    xml_file_(QString::Null()),
    base64_(false),
    text_(true),
    use_image_(true)
{
  // Key map

  key_map_[region::Text] = qMakePair(tr("Text Region"), QColor(0, 200, 0));
  key_map_[region::Paragraph] = qMakePair(tr("Paragraph"), QColor(0, 0, 255));
  key_map_[region::Line] = qMakePair(tr("Text line"), QColor(255, 0, 0));

  key_map_[region::Image] = qMakePair(tr("Image"), QColor(255, 120, 0));
  key_map_[region::Noise] = qMakePair(tr("Noise"), QColor(43, 39, 128));
  key_map_[region::Separator] = qMakePair(tr("Separator"), QColor(0, 0, 255));
  key_map_[region::Table] = qMakePair(tr("Table"), QColor(220, 246, 0));
  key_map_[region::LineDrawing] = qMakePair(tr("LineDrawing"),
					    QColor(255, 198, 0));
  key_map_[region::Graphic] = qMakePair(tr("Graphic"), QColor(255, 0, 144));
  key_map_[region::Chart] = qMakePair(tr("Chart"), QColor(0, 204, 255));
  key_map_[region::Maths] = qMakePair(tr("Maths"), QColor(170, 0, 255));

  // Region ids

  region_ids_["text_region"] = region::Text;
  region_ids_["paragraph"] = region::Paragraph;
  region_ids_["line"] = region::Line;
  region_ids_["image_region"] = region::Image;
  region_ids_["noise_region"] = region::Noise;
  region_ids_["separator_region"] = region::Separator;
  region_ids_["table_region"] = region::Table;
  region_ids_["line_drawing_region"] = region::LineDrawing;
  region_ids_["graphic_region"] = region::Graphic;
  region_ids_["chart_region"] = region::Chart;
  region_ids_["maths_region"] = region::Maths;

  // Layout

  win_->resize(1152, 864);
  win_->statusBar();

  QMenu* file_menu = win_->menuBar()->addMenu(tr("File"));
  QAction* quit_action = new QAction(tr("Quit"), file_menu);
  connect(quit_action, SIGNAL(triggered()),
	  app_, SLOT(quit()));
  quit_action->setStatusTip(tr("Exit the program."));
  file_menu->addAction(quit_action);

  QMenu* option_menu = win_->menuBar()->addMenu(tr("Options"));
  outline_action_ = new QAction(tr("Draw outline"), option_menu);
  outline_action_->setStatusTip(tr("Draw region outlines."));
  outline_action_->setCheckable(true);
  outline_action_->setChecked(true);
  connect(outline_action_, SIGNAL(toggled(bool)),
	  this, SIGNAL(setOutline(bool)));
  option_menu->addAction(outline_action_);
  precise_action_ = new QAction(tr("Precise outline"), option_menu);
  precise_action_->setStatusTip(tr("1px outline relative to the image "
				   "(1px relative to the view if off)."));
  precise_action_->setCheckable(true);
  precise_action_->setChecked(false);
  connect(precise_action_, SIGNAL(toggled(bool)),
	  this, SIGNAL(setPrecise(bool)));
  option_menu->addAction(precise_action_);
  fill_action_ = new QAction(tr("Fill regions"), option_menu);
  fill_action_->setStatusTip(tr("Color the inside of regions."));
  fill_action_->setCheckable(true);
  fill_action_->setChecked(true);
  connect(fill_action_, SIGNAL(toggled(bool)),
	  this, SIGNAL(setFill(bool)));
  option_menu->addAction(fill_action_);

  QAction* cache_action = new QAction(tr("Disable cache"), file_menu);
  cache_action->setStatusTip(tr("Disable the image cache (useful for"
				" large images)."));
  cache_action->setCheckable(true);
  cache_action->setChecked(false);
  connect(cache_action, SIGNAL(toggled(bool)),
	  this, SLOT(useCache(bool)));
  option_menu->addAction(cache_action);

  QAction* extended_action = new QAction(tr("Extended mode"), file_menu);
  extended_action->setStatusTip(tr("If enabled, some features not supported by ICDAR"
				" are added such as text regions or text lines"));
  extended_action->setCheckable(true);
  extended_action->setChecked(false);
  connect(extended_action, SIGNAL(toggled(bool)),
	  this, SLOT(useExtended(bool)));
  option_menu->addAction(extended_action);

  QAction* show_image_action = new QAction(tr("Show pictures"), file_menu);
  //show_image_action->setStatusTip(tr(""));

  show_image_action->setCheckable(true);
  show_image_action->setChecked(true);
  connect(show_image_action, SIGNAL(toggled(bool)),
	  this, SLOT(useImage(bool)));
  option_menu->addAction(show_image_action);

  QAction* show_text_action = new QAction(tr("Show text"), file_menu);
  show_text_action->setStatusTip(tr("Show detected text inside boxes."));
  show_text_action->setCheckable(true);
  show_text_action->setChecked(true);
  connect(show_text_action, SIGNAL(toggled(bool)),
	  this, SLOT(useText(bool)));
  option_menu->addAction(show_text_action);

  QMenu* help_menu = win_->menuBar()->addMenu(tr("Help"));
  QAction* about_action = new QAction(tr("About"), help_menu);
  about_action->setStatusTip(tr("About this program."));
  connect(about_action, SIGNAL(triggered()),
	  this, SLOT(help()));
  help_menu->addAction(about_action);

  QSplitter* h_splitter = new QSplitter();
  QSplitter* v_splitter = new QSplitter(Qt::Vertical);
  QSplitter* v_splitter2 = new QSplitter(Qt::Vertical);

  StepWidget* step_widget = new StepWidget();
  XmlWidget* xml_wgt = new XmlWidget();
  BrowserWidget* browser_wgt =
    new BrowserWidget(files_, argc != 2 ? QString() : argv[1]);
  key_wgt_ = new KeyWidget(key_map_);
  ImageWidget* image_wgt = new ImageWidget(scene_);

  scene_->setBackgroundBrush(scene_->palette().window());

  v_splitter->addWidget(step_widget);
  v_splitter->addWidget(key_wgt_);
  v_splitter->addWidget(browser_wgt);

  v_splitter2->addWidget(image_wgt);
  v_splitter2->addWidget(xml_wgt);

  h_splitter->addWidget(v_splitter);
  h_splitter->addWidget(v_splitter2);

  win_->setCentralWidget(h_splitter);

  QList<int> v_sizes;
  v_sizes << 200 << 300 << 300;
  v_splitter->setSizes(v_sizes);

  QList<int> v_sizes2;
  v_sizes2 << 650 << 150;
  v_splitter2->setSizes(v_sizes2);

  QList<int> h_sizes;
  h_sizes << 200 << 900;
  h_splitter->setSizes(h_sizes);

  connect(browser_wgt, SIGNAL(activated(QString, bool, bool)),
	  step_widget, SLOT(fill_steps(QString, bool, bool)));

  connect(step_widget, SIGNAL(change_base(bool)),
	  this, SLOT(change_base(bool)));
  connect(step_widget, SIGNAL(load_image(QString, bool)),
	  this, SLOT(load(QString, bool)));
  connect(step_widget, SIGNAL(load_xml(QString)),
	  this, SLOT(load_xml(QString)));

  connect(this, SIGNAL(mode_changed(bool)),
	  key_wgt_, SLOT(change_mode(bool)));
  connect(this, SIGNAL(updated()),
	  image_wgt, SLOT(update()));
  connect(this, SIGNAL(fill_xml(QString)),
	  xml_wgt, SLOT(fill_widget(QString)));

  connect(key_wgt_, SIGNAL(updated(int, bool)),
	  this, SIGNAL(key_updated(int, bool)));

  connect(scene_, SIGNAL(selected(QString, QString)),
	  xml_wgt, SLOT(select(QString, QString)));
    connect(scene_, SIGNAL(deselected()),
  	  xml_wgt, SLOT(deselect()));

  connect(image_wgt, SIGNAL(scaleUpdated(qreal)),
	  this, SLOT(maybeChangeCacheMode(qreal)));
}


void
Viewer::add_text(QDomNode line, QDomNode region)
{

  int a_height = region.toElement().attribute("a_height", "0").toInt();
  int d_height = region.toElement().attribute("d_height", "0").toInt();
  int x_height = region.toElement().attribute("x_height", "0").toInt();

  if (d_height < 0)
    d_height = -d_height;

  if ( (a_height - x_height) < (d_height))
    a_height = x_height + d_height;

  if ( (a_height - x_height) > (d_height))
    d_height = a_height - x_height;

  QDomNode coords = region.firstChild();

  while (!coords.isNull() && !coords.toElement().tagName().contains("coords"))
    coords = coords.nextSibling();

  if (coords.isNull())
    return;

  QDomNode point = coords.firstChild();

  int x_min = INT_MAX;
  int y_min = INT_MAX;

  while (!point.isNull())
    {
      int x = point.toElement().attribute("x", "0").toInt();
      int y = point.toElement().attribute("y", "0").toInt();

      if (x < x_min)
	x_min = x;

      if (y < y_min)
	y_min = y;

      point = point.nextSibling();
    }

  QString text = line.toElement().attribute("text", "none");
  QFont font("Times");
  font.setPixelSize(a_height + d_height);
  QGraphicsTextItem* text_item  = scene_->addText(text, font);
  text_item->setPos(x_min, y_min);
  text_item->setTextInteractionFlags(Qt::TextSelectableByMouse);
  text_vector_ << text_item;
  if (!text_)
    scene_->removeItem(text_item);

}

void
Viewer::add_region(QDomNode father, QString attr_id)
{
  QDomNode coords = father.firstChild();
  QString name = father.toElement().tagName();
  region::RegionId id = static_cast<region::RegionId>(region_ids_[name]);

  while (!coords.isNull() && !coords.toElement().tagName().contains("coords"))
    coords = coords.nextSibling();

  if (coords.isNull())
    return;

  QDomNode point = coords.firstChild();
  QVector<QPoint> points;

  while (!point.isNull())
    {
      int x = point.toElement().attribute("x", "0").toInt();
      int y = point.toElement().attribute("y", "0").toInt();

      points << QPoint(x, y);
      point = point.nextSibling();
    }

  ImageRegion* r = new ImageRegion(id,
				   key_map_[id].first,
				   key_map_[id].second,
				   attr_id, points,
				   outline_action_->isChecked(),
				   fill_action_->isChecked(),
				   precise_action_->isChecked(),
				   key_wgt_->isChecked(id));

  connect(this, SIGNAL(key_updated(int, bool)),
	  r, SLOT(setDrawIfSameId(int, bool)));
  connect(this, SIGNAL(setOutline(bool)),
	  r, SLOT(setOutline(bool)));
  connect(this, SIGNAL(setPrecise(bool)),
	  r, SLOT(setPrecise(bool)));
  connect(this, SIGNAL(setFill(bool)),
	  r, SLOT(setFill(bool)));

  scene_->addItem(r);
}

void
Viewer::load_xml(QString filename)
{
  text_vector_.clear();
  image_vector_.clear();

  app_->setOverrideCursor(QCursor(Qt::WaitCursor));
  emit fill_xml(filename);

  if (image_ && image_->scene() && image_->scene() == scene_)
    scene_->removeItem(image_);

  scene_->clear();

  if (!base64_ && use_image_)
    scene_->addItem(image_);

  scene_->update();

  xml_file_ = filename;
  QFile f_in(xml_file_);
  f_in.open(QIODevice::ReadOnly);

  QDomDocument doc;
  doc.setContent(&f_in);
  f_in.close();

  QDomElement root = doc.documentElement();
  QDomNode page = root.firstChild();

  while (!page.isNull() && !page.toElement().tagName().contains("page"))
    page = page.nextSibling();

  if (page.isNull())
    return;

  int width = page.toElement().attribute("image_width", "none").toInt();
  int height = page.toElement().attribute("image_height", "none").toInt();

  scene_->setSceneRect(0, 0, width, height);

  QDomNode region = page.firstChild();

  while (!region.isNull())
    {
      if (region.toElement().tagName().contains(QRegExp("(image|graphic|chart|separator|table|text)_region")))
        {
	  QString attr_id = region.toElement().attribute("id", "none");
	  add_region(region, attr_id);

	  if ( base64_ &&
	       region.toElement().tagName().contains(QRegExp("(image|graphic|chart|separator|table)_region")))
	    {
	      QDomNode container = region.firstChild();

	      while (!container.isNull() && !container.toElement().tagName().contains("container"))
		container = container.nextSibling();

	      QDomNode coords = region.firstChild();

	      while (!coords.isNull() && !coords.toElement().tagName().contains("coords"))
		coords = coords.nextSibling();

	      if (!container.isNull() && !coords.isNull())
		{
		  QDomNode child = container.firstChild();

		  while (!child.isNull() && !child.toElement().tagName().contains("data"))
		    child = child.nextSibling();

		  QPixmap pix;
		  QString data = child.toElement().text();
		  QByteArray ba;
		  ba = ba.append(data);
		  QByteArray out_ba = QByteArray::fromBase64(ba);
		  pix.loadFromData(out_ba);

		  QGraphicsPixmapItem* image = new QGraphicsPixmapItem(pix);

		  QDomNode point = coords.firstChild();

		  int x_min = INT_MAX;
		  int y_min = INT_MAX;

		  while (!point.isNull())
		    {
		      int x = point.toElement().attribute("x", "0").toInt();
		      int y = point.toElement().attribute("y", "0").toInt();

		      if (x < x_min)
			x_min = x;

		      if (y < y_min)
			y_min = y;

		      point = point.nextSibling();
		    }

		  image->setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
		  image->setZValue(0);
		  image->setOffset(x_min, y_min);
		  if (use_image_)
		    scene_->addItem(image);
		  image_vector_ << image;

		  if (no_cache_)
		    image->setCacheMode(QGraphicsItem::NoCache);
		  else
		    image->setCacheMode(QGraphicsItem::DeviceCoordinateCache);

		  child = child.nextSibling();
		}
	    }

	  if (extended_mode_ &&
	      region.toElement().tagName().contains("text_region"))
	    {
	      QDomNode para = region.firstChild();
	      while (!para.isNull() && !para.toElement().tagName().contains("paragraph"))
		para = para.nextSibling();

	      if (!para.isNull())
		{
		  add_region(para, attr_id);

		  QDomNode line = para.firstChild();
		  while (!line.isNull() && !line.toElement().tagName().contains("line"))
		    line = line.nextSibling();

		  if (!line.isNull())
		    {
		      add_region(line, attr_id);
		      add_text(line, region);
		    }
		}
	    }

        }

      region = region.nextSibling();
    }

  emit updated();
  scene_->update();
  key_wgt_->update_all();

  app_->restoreOverrideCursor();
}

void
Viewer::load(QString filename, bool b)
{
  app_->setOverrideCursor(QCursor(Qt::WaitCursor));

  scene_->clear();
  scene_->update();
  image_ = 0;

  // Load the image in a pixmap that is directly shown on screen.
  // This is very slow when used with the normal rendering system.
  // OpenGL might speed up things a bit.
  if (b)
    image_ = new QGraphicsPixmapItem(load_base64(filename));
  else
    image_ = new QGraphicsPixmapItem(QPixmap(filename));

  image_->setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
  image_->setZValue(0);
  if (use_image_)
    scene_->addItem(image_);

  if (no_cache_)
    image_->setCacheMode(QGraphicsItem::NoCache);
  else
    image_->setCacheMode(QGraphicsItem::DeviceCoordinateCache);

  app_->restoreOverrideCursor();
  emit updated();

  app_->restoreOverrideCursor();
}

int
Viewer::exec()
{
  win_->show();
  return app_->exec();
}

void
Viewer::help()
{
  HelpDialog dlg;
  dlg.exec();
}

void
Viewer::maybeChangeCacheMode(qreal scale)
{
  if (image_)
  {
    if (scale >= 0.7)
      image_->setCacheMode(QGraphicsItem::NoCache);
    else if (!no_cache_)
      image_->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
  }
}

void Viewer::useExtended(bool b)
{
  extended_mode_ = b;
  key_wgt_->update_all();

  if (xml_file_ != QString::Null())
    load_xml(xml_file_);

  emit mode_changed(b);
}

void
Viewer::useCache(bool b)
{
  no_cache_ = b;
  if (b)
    {
      if (image_)
	image_->setCacheMode(QGraphicsItem::NoCache);
    }
  else
    if (image_)
      image_->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
}

QPixmap Viewer::load_base64(QString xml)
{
  QPixmap pix;
  QFile f_in(xml);
  f_in.open(QIODevice::ReadOnly);

  QDomDocument doc;
  doc.setContent(&f_in);
  f_in.close();

  QDomElement root = doc.documentElement();
  QDomNode child = root.firstChild();

  while (!child.isNull() && !child.toElement().tagName().contains("page"))
    child = child.nextSibling();

  child = child.firstChild();
  while (!child.isNull())
    {
      if (child.toElement().tagName().contains(QRegExp("image_region")))
        {
          QDomNode node = child.firstChild();
          QString id = child.toElement().attribute("id", "none");

          while (!node.isNull() && !node.toElement().tagName().contains("container"))
            node = node.nextSibling();

          if (!node.isNull())
            {
              QString data = node.firstChildElement("data").text();
	      QByteArray ba;
	      ba = ba.append(data);
	      QByteArray out_ba = QByteArray::fromBase64(ba);
	      pix.loadFromData(out_ba);

	      return pix;
            }
        }
      child = child.nextSibling();
    }

  return pix;
}

void
Viewer::useText(bool b)
{
  text_ = b;
  if (!b)
    {
      for (int i = 0; i < text_vector_.size(); ++i)
	scene_->removeItem(text_vector_[i]);
    }
  else
    {
      for (int i = 0; i < text_vector_.size(); ++i)
	scene_->addItem(text_vector_[i]);
    }

  scene_->update();
}

void
Viewer::useImage(bool b)
{
  use_image_ = b;
  if (!b)
    {
      if (image_ && image_->scene() && image_->scene() == scene_)
	scene_->removeItem(image_);

      for (int i = 0; i < image_vector_.size(); ++i)
	scene_->removeItem(image_vector_[i]);
    }
  else
    {
      if (image_ && image_->scene() == 0)
	scene_->addItem(image_);

      for (int i = 0; i < image_vector_.size(); ++i)
	scene_->addItem(image_vector_[i]);
    }

  scene_->update();
}

void
Viewer::change_base(bool b)
{
  base64_ = b;
}
