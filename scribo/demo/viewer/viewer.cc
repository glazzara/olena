// Copyright (C) 2010, 2011 EPITA Research and Development Laboratory
// (LRDE)
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


#include <iostream>
#include <limits.h>

#include "viewer.hh"
#include "key_widget.hh"
#include "browser_widget.hh"
#include "xml_widget.hh"
#include "step_widget.hh"
#include "image_scene.hh"
#include "image_view.hh"
#include "image_region.hh"
#include "help_dialog.hh"
#include "preferences_dialog.hh"
#include "runner.hh"
#include "config.hh"

#include "common.hh"

Viewer::Viewer(int &argc, char** argv)
  : app_ (new QApplication(argc, argv)),
    win_ (new QMainWindow()),
    image_(0),
    scene_(new ImageScene()),
    files_(new QDirModel()),
    doc_layout_(0),
    key_map_(region::RegionIdCount),
    no_cache_(false),
    extended_mode_(false),
    xml_file_(QString::Null()),
    base64_(false),
    text_(true),
    use_image_(true),
    pdialog_(win_)
{
  // Key map
  // --------

  key_map_[region::Text] = qMakePair(tr("Text Region"), QColor(0, 200, 0));

 // Extension
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

  // Extension
  key_map_[region::WhitespaceSeparator] = qMakePair(tr("Whitespace Separator"), QColor(0, 0, 128));

  // Extension
  key_map_[region::Baseline] = qMakePair(tr("Baseline"), QColor(128, 0, 255));
  key_map_[region::Meanline] = qMakePair(tr("Meanline"), QColor(128, 0, 255));



  // Region ids
  // ----------

  region_ids_["text_region"] = region::Text;

  // Extension
  region_ids_["line"] = region::Line;

  region_ids_["image_region"] = region::Image;
  region_ids_["noise_region"] = region::Noise;
  region_ids_["separator_region"] = region::Separator;
  region_ids_["table_region"] = region::Table;
  region_ids_["line_drawing_region"] = region::LineDrawing;
  region_ids_["graphic_region"] = region::Graphic;
  region_ids_["chart_region"] = region::Chart;
  region_ids_["maths_region"] = region::Maths;

  // Extension
  region_ids_["whitespace_separator_region"] = region::WhitespaceSeparator;



  win_->resize(1152, 864);
  win_->statusBar();

  QMenu* file_menu = win_->menuBar()->addMenu(tr("File"));
  QMenu* option_menu = win_->menuBar()->addMenu(tr("Options"));
  QMenu* view_menu = win_->menuBar()->addMenu(tr("View"));
  QMenu* help_menu = win_->menuBar()->addMenu(tr("Help"));


  // File menu

  QAction *doc_seg_action = create_action("Segment document", file_menu,
					  "Segment document", "Ctrl+S");
  connect(doc_seg_action, SIGNAL(triggered()),
	  this, SLOT(run_process()));
  file_menu->addAction(doc_seg_action);

  file_menu->addSeparator();

  export_action_ = create_action("Export as...", file_menu,
				 "Export as...", "Shift+Ctrl+S");
  connect(export_action_, SIGNAL(triggered()),
	  this, SLOT(export_as()));
  file_menu->addAction(export_action_);
  export_action_->setEnabled(false);

  file_menu->addSeparator();

  QAction *preview_print_action = create_action("Printing preview", file_menu,
					"Printint preview", "Ctrl+P");
  connect(preview_print_action, SIGNAL(triggered()),
	  this, SLOT(preview_print()));
  file_menu->addAction(preview_print_action);
  preview_print_action->setEnabled(false);


  QAction *print_action = create_action("Print", file_menu,
					"Print", "Ctrl+P");
  connect(print_action, SIGNAL(triggered()),
	  this, SLOT(print()));
  file_menu->addAction(print_action);
  print_action->setEnabled(false);

  file_menu->addSeparator();


  QAction* quit_action = create_action("Quit", file_menu,
				       "Exit the program.", "Ctrl+q");
  connect(quit_action, SIGNAL(triggered()),
	  app_, SLOT(quit()));
  file_menu->addAction(quit_action);

  // Option menu

  QAction* preferences_action_ = create_action("Preferences", option_menu,
					     "Preferences", "Ctrl+Alt+P");
  preferences_action_->setCheckable(false);
  connect(preferences_action_, SIGNAL(triggered(bool)),
	  this, SLOT(on_preferences()));
  option_menu->addAction(preferences_action_);


  // View menu

  outline_action_ = create_action("Draw outline", view_menu,
				  "Draw region outlines.", "Ctrl+o");
  outline_action_->setCheckable(true);
  outline_action_->setChecked(true);
  connect(outline_action_, SIGNAL(toggled(bool)),
	  this, SIGNAL(setOutline(bool)));
  view_menu->addAction(outline_action_);

  precise_action_ = create_action("Precise outline", view_menu,
				  "1px outline relative to the image "
				  "(1px relative to the view if off).",
				  "Shift+Ctrl+p");
  precise_action_->setCheckable(true);
  precise_action_->setChecked(false);
  connect(precise_action_, SIGNAL(toggled(bool)),
	  this, SIGNAL(setPrecise(bool)));
  view_menu->addAction(precise_action_);

  fill_action_ = create_action("Fill regions", view_menu,
			       "Color the inside of regions.", "Ctrl+f");
  fill_action_->setCheckable(true);
  fill_action_->setChecked(true);
  connect(fill_action_, SIGNAL(toggled(bool)),
	  this, SIGNAL(setFill(bool)));
  view_menu->addAction(fill_action_);

  QAction* cache_action = create_action("Disable cache", view_menu,
					"Disable the image cache (useful for"
					" large images).", "Ctrl+c");
  cache_action->setCheckable(true);
  cache_action->setChecked(false);
  connect(cache_action, SIGNAL(toggled(bool)),
	  this, SLOT(useCache(bool)));
  view_menu->addAction(cache_action);


  QAction* extended_action = create_action("Extended mode", view_menu,
					   "If enabled, some features "
					   "not supported by ICDAR"
					   " are added such as text regions"
					   "or text lines", "Ctrl+e");
  extended_action->setCheckable(true);
  extended_action->setChecked(false);
  connect(extended_action, SIGNAL(toggled(bool)),
	  this, SLOT(useExtended(bool)));
  view_menu->addAction(extended_action);

  QAction* show_image_action = create_action("Show pictures", view_menu,
					     "Display pictures on the scene or not",
					     "Ctrl+i");
  show_image_action->setCheckable(true);
  show_image_action->setChecked(true);
  connect(show_image_action, SIGNAL(toggled(bool)),
	  this, SLOT(useImage(bool)));
  view_menu->addAction(show_image_action);

  QAction* show_text_action = create_action("Show text", view_menu,
					    "Show detected text inside boxes.",
					    "Ctrl+t");
  show_text_action->setCheckable(true);
  show_text_action->setChecked(true);
  connect(show_text_action, SIGNAL(toggled(bool)),
	  this, SLOT(useText(bool)));
  view_menu->addAction(show_text_action);

  key_wgt_ = new KeyWidget(key_map_);
  QAction *show_region_action = create_action("Show regions", view_menu,
					      "Display regions that are present in"
					      " the XML file.",
					      "Ctrl+r");
  show_region_action->setCheckable(true);
  show_region_action->setChecked(true);
  connect(show_region_action, SIGNAL(toggled(bool)),
	  key_wgt_, SLOT(setAll(bool)));
  view_menu->addAction(show_region_action);

  // Help menu

  QAction* about_action = create_action("About", help_menu,
					"About this program",
					"Ctrl+h");
  connect(about_action, SIGNAL(triggered()),
	  this, SLOT(help()));
  help_menu->addAction(about_action);

  // Layout

  QSplitter* h_splitter = new QSplitter();
  QSplitter* v_splitter = new QSplitter(Qt::Vertical);
  QSplitter* v_splitter2 = new QSplitter(Qt::Vertical);

  step_widget_ = new StepWidget();
  XmlWidget* xml_wgt = new XmlWidget();
  browser_wgt_ = new BrowserWidget(files_, argc != 2 ? QString() : argv[1]);
  image_wgt_ = new ImageWidget(scene_);

  connect(step_widget_, SIGNAL(step_selected(bool)),
	  export_action_, SLOT(setEnabled(bool)));
  connect(step_widget_, SIGNAL(step_selected(bool)),
	  print_action, SLOT(setEnabled(bool)));
  connect(step_widget_, SIGNAL(step_selected(bool)),
	  preview_print_action, SLOT(setEnabled(bool)));

  scene_->setBackgroundBrush(scene_->palette().window());

  v_splitter->addWidget(step_widget_);
  v_splitter->addWidget(key_wgt_);
  v_splitter->addWidget(browser_wgt_);

  v_splitter2->addWidget(image_wgt_);
  v_splitter2->addWidget(xml_wgt);

  h_splitter->addWidget(v_splitter);
  h_splitter->addWidget(v_splitter2);

  win_->setCentralWidget(h_splitter);

  QList<int> v_sizes;
  v_sizes << 200 << 250 << 350;
  v_splitter->setSizes(v_sizes);

  QList<int> v_sizes2;
  v_sizes2 << 725 << 175;
  v_splitter2->setSizes(v_sizes2);

  QList<int> h_sizes;
  h_sizes << 200 << 900;
  h_splitter->setSizes(h_sizes);

  connect(browser_wgt_, SIGNAL(activated(QString, bool, bool)),
	  step_widget_, SLOT(fill_steps(QString, bool, bool)));

  connect(step_widget_, SIGNAL(change_base(bool)),
	  this, SLOT(change_base(bool)));
  connect(step_widget_, SIGNAL(load_image(QString, bool)),
	  this, SLOT(load(QString, bool)));
  connect(step_widget_, SIGNAL(load_xml(QString)),
	  this, SLOT(load_xml(QString)));

  connect(this, SIGNAL(mode_changed(bool)),
	  key_wgt_, SLOT(change_mode(bool)));
  connect(this, SIGNAL(updated()),
	  image_wgt_, SLOT(update()));
  connect(this, SIGNAL(fill_xml(QString)),
	  xml_wgt, SLOT(fill_widget(QString)));

  connect(key_wgt_, SIGNAL(updated(int, bool)),
	  this, SIGNAL(key_updated(int, bool)));

  connect(scene_, SIGNAL(selected(QString, QString)),
	  xml_wgt, SLOT(select(QString, QString)));
    connect(scene_, SIGNAL(deselected()),
  	  xml_wgt, SLOT(deselect()));

  connect(image_wgt_, SIGNAL(scaleUpdated(qreal)),
	  this, SLOT(maybeChangeCacheMode(qreal)));


  // Progress dialog and process runner.
  pdialog_.setModal(true);
  pdialog_.setAutoClose(false);
  pdialog_.setCancelButton(0);
  connect(&runner_, SIGNAL(finished()), &pdialog_, SLOT(close()));

  connect(&runner_, SIGNAL(new_step(const QString&)),
	  &pdialog_, SLOT(setWindowTitle(const QString&)));
  connect(&runner_, SIGNAL(new_progress_max_value(int)),
	  &pdialog_, SLOT(setMaximum(int)));
  connect(&runner_, SIGNAL(new_progress_label(const QString&)),
	  &pdialog_, SLOT(setLabelText(const QString&)));
  connect(&runner_, SIGNAL(progress()),
	  this, SLOT(run_progress()));
  connect(&runner_, SIGNAL(xml_saved(const QString&)),
	  this, SLOT(on_xml_saved(const QString&)));

  extended_action->setChecked(true);
}


Viewer::~Viewer()
{
  // Remove temporary xml files.
  foreach(QString file, tmp_files_to_remove_)
    QFile::remove(file);
}

void
Viewer::add_text(QDomNode line)
{

  int a_height = line.toElement().attribute("a_height", "0").toInt();
  int d_height = line.toElement().attribute("d_height", "0").toInt();
  int x_height = line.toElement().attribute("x_height", "0").toInt();

  if (d_height < 0)
    d_height = -d_height;

  if ( (a_height - x_height) < (d_height))
    a_height = x_height + d_height;

  if ( (a_height - x_height) > (d_height))
    d_height = a_height - x_height;

  QDomNode coords = line.firstChild();

  while (!coords.isNull() && !coords.toElement().tagName().contains("Coords"))
    coords = coords.nextSibling();

  if (coords.isNull())
  {
    qDebug() << "Warning : textline without coordinates...";
    return;
  }

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

  QString text = line.toElement().attribute("text", "");
  QFont font("Times");
  font.setPixelSize(a_height + d_height);
  QGraphicsTextItem* text_item  = scene_->addText(text, font);
  text_item->setPos(x_min, y_min);
  text_item->setTextInteractionFlags(Qt::TextSelectableByMouse);
  text_item->setZValue(5);
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

  while (!coords.isNull() && !coords.toElement().tagName().contains("Coords"))
    coords = coords.nextSibling();

  if (coords.isNull())
  {
    qDebug() << "Warning : add_region - region without coordinates";
    return;
  }

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
Viewer::add_typo_lines(QDomNode father, QString attr_id)
{
  // Retrieve typographical information
  int baseline = father.toElement().attribute("baseline").toInt();
  int meanline = father.toElement().attribute("meanline").toInt();

  QRect bbox;

  // Looking for bbox coordinates.
  {
    QDomNode coords = father.firstChild();
    while (!coords.isNull() && !coords.toElement().tagName().contains("Coords"))
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

    QPolygon polygon(points);
    bbox = polygon.boundingRect();
  }

  // Creating blocks

  // Baseline
  {
    QVector<QPoint> points;
    points.append(QPoint(bbox.topLeft().x(), baseline));
    points.append(QPoint(bbox.topRight().x(), baseline));

    ImageRegion* r = new ImageRegion(region::Baseline,
				     key_map_[region::Baseline].first,
				     key_map_[region::Baseline].second,
				     attr_id, points,
				     outline_action_->isChecked(),
				     fill_action_->isChecked(),
				     precise_action_->isChecked(),
				     key_wgt_->isChecked(region::Baseline));

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

  // Meanline
  {
    QVector<QPoint> points;
    points.append(QPoint(bbox.topLeft().x(), meanline));
    points.append(QPoint(bbox.topRight().x(), meanline));

    ImageRegion* r = new ImageRegion(region::Meanline,
				     key_map_[region::Meanline].first,
				     key_map_[region::Meanline].second,
				     attr_id, points,
				     outline_action_->isChecked(),
				     fill_action_->isChecked(),
				     precise_action_->isChecked(),
				     key_wgt_->isChecked(region::Meanline));

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
}


void
Viewer::load_xml(QString filename)
{
  app_->setOverrideCursor(QCursor(Qt::WaitCursor));
  emit fill_xml(filename);

  if (image_ && image_->scene() && image_->scene() == scene_)
    scene_->removeItem(image_);

  text_vector_.clear();
  image_vector_.clear();
  scene_->clear();

  if (!base64_ && use_image_ && image_)
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

  while (!page.isNull() && !page.toElement().tagName().contains("Page"))
    page = page.nextSibling();

  if (page.isNull())
    return;

  int width = page.toElement().attribute("image_width", "none").toInt();
  int height = page.toElement().attribute("image_height", "none").toInt();

  scene_->setSceneRect(0, 0, width, height);

  QDomNode region = page.firstChild();

  while (!region.isNull())
    {
      if (region.toElement().tagName().contains(QRegExp("(whitespace_separator|image|graphic|chart|separator|table|text)_region")))
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

	      while (!coords.isNull() && !coords.toElement().tagName().contains("Coords"))
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

	  if (region.toElement().tagName().contains("text_region"))
	  {
	    QDomNode line = region.firstChild();

	    do
	    {
	      while (!line.isNull() && !line.toElement().tagName().contains("line"))
		line = line.nextSibling();

	      if (!line.isNull())
	      {
		if (extended_mode_)
		{
		  QString line_id = line.toElement().attribute("id", "none");
		  add_region(line, line_id);
		  add_typo_lines(line, line_id);
		}
		add_text(line);
	      }
	    }
	    while (!(line = line.nextSiblingElement("line")).isNull());
	  }
	}

      region = region.nextSibling();
    }

  //emit updated();
  scene_->update();
  key_wgt_->update_all();

  app_->restoreOverrideCursor();
}

void
Viewer::load(QString filename, bool b)
{
  app_->setOverrideCursor(QCursor(Qt::WaitCursor));

  scene_->clear();
  text_vector_.clear();
  image_vector_.clear();
  scene_->update();
  image_ = 0;
  xml_file_ = QString::Null();

  // Load the image in a pixmap that is directly shown on screen.
  // This is very slow when used with the normal rendering system.
  // OpenGL might speed up things a bit.
  if (b)
    image_ = new QGraphicsPixmapItem(load_base64(filename));
  else
    image_ = new QGraphicsPixmapItem(QPixmap(filename));

  current_image_ = filename;

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


QAction
*Viewer::create_action(QString name, QMenu *menu, QString status, QString shortcut)
{
  QAction* action = new QAction(name, menu);
  action->setStatusTip(status);
  action->setShortcut(QKeySequence(shortcut));

  return (action);
}

void
Viewer::on_preferences()
{
  preferences_dialog *win = new preferences_dialog(win_);
  win->show();
}


void
Viewer::run_process()
{
  if (!current_image_.isEmpty())
  {
    reset_progress_dialog();
    runner_.start_demat(current_image_);
  }
}


void
Viewer::run_progress()
{
  pdialog_.setValue(pdialog_.value() + 1);
}


void
Viewer::on_xml_saved(const QString& filename)
{
  config * const conf = config::get_instance();

  if (!conf->general_save_xml_enabled())
    tmp_files_to_remove_.insert(filename);

  QListWidgetItem *item = step_widget_->insert_new_entry(filename);
  step_widget_->activate(item);
}


void
Viewer::export_as()
{
  QFileInfo f(browser_wgt_->current());
  QString output_suggestion = f.baseName() + ".pdf";
  QString output = QFileDialog::getSaveFileName(0, "Export document as...", output_suggestion, tr("PDF (*.pdf);; HTML (*.html *.htm)"));

  if (!output.isEmpty())
  {
    reset_progress_dialog();
    runner_.start_export(browser_wgt_->current(), step_widget_->current(), output);
    qDebug() << "Saving to " << output << " - " << browser_wgt_->current() << " - " <<  step_widget_->current();
  }
}


void
Viewer::reset_progress_dialog()
{
  pdialog_.setValue(0);
  pdialog_.setLabelText("");
  pdialog_.show();
}


void
Viewer::print()
{
  QPrinter printer(QPrinter::HighResolution);
  configure_printer(printer);

  QPrintDialog dialog(&printer);
  dialog.setWindowTitle(tr("Print Document"));
  if (dialog.exec() != QDialog::Accepted)
    return;

  do_print(&printer);
}

void
Viewer::preview_print()
{
  QPrinter printer(QPrinter::HighResolution);
  configure_printer(printer);

  QPrintPreviewDialog preview(&printer);
  connect(&preview, SIGNAL(paintRequested(QPrinter *)),
	  this, SLOT(do_print(QPrinter *)));

  preview.exec();
}

void Viewer::do_print(QPrinter * printer)
{
  QList<QGraphicsItem *> items = image_wgt_->view()->items();

  QPainter painter(printer);

  QGraphicsItem *item;
  QStyleOptionGraphicsItem options;

  // Painting backward objects first.
  for (int i = items.size() - 1; i >= 0; --i)
  {
    item = items.at(i);

    if (item != image_)
    {
      QRect vport = image_->mapRectFromItem(item,
					    item->boundingRect()).toRect();

      painter.translate(std::abs(item->boundingRect().x() - vport.x()),
			std::abs(item->boundingRect().y() - vport.y()));
    }


    item->paint(&painter, &options);
    painter.resetTransform();
  }
}

void
Viewer::configure_printer(QPrinter& printer)
{
  printer.setPageSize(QPrinter::A4);
  printer.setResolution(300);
}
