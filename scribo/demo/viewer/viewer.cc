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

#include "viewer.hh"
#include "property_widget.hh"
#include "key_widget.hh"
#include "browser_widget.hh"
#include "image_widget.hh"
#include "xml_widget.hh"
#include "step_widget.hh"
#include "image_scene.hh"
#include "image_region.hh"
#include "help_dialog.hh"

#include "dommodel.hh"

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
    xml_file_(QString(""))
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
  PropertyWidget* property_wgt = new PropertyWidget();
  XmlWidget* xml_wgt = new XmlWidget();
  BrowserWidget* browser_wgt =
    new BrowserWidget(files_, argc != 2 ? QString() : argv[1]);
  key_wgt_ = new KeyWidget(key_map_);
  ImageWidget* image_wgt = new ImageWidget(scene_);

  scene_->setBackgroundBrush(scene_->palette().window());

  v_splitter->addWidget(property_wgt);
  v_splitter->addWidget(step_widget);
  v_splitter->addWidget(key_wgt_);
  v_splitter->addWidget(browser_wgt);

  v_splitter2->addWidget(image_wgt);
  v_splitter2->addWidget(xml_wgt);

  h_splitter->addWidget(v_splitter);
  h_splitter->addWidget(v_splitter2);

  win_->setCentralWidget(h_splitter);

  QList<int> v_sizes;
  v_sizes << 200 << 200 << 300 << 400;
  v_splitter->setSizes(v_sizes);

  QList<int> v_sizes2;
  v_sizes2 << 500 << 100;
  v_splitter2->setSizes(v_sizes2);

  QList<int> h_sizes;
  h_sizes << 200 << 700;
  h_splitter->setSizes(h_sizes);

  connect(browser_wgt, SIGNAL(activated(QString, bool, bool)),
	  step_widget, SLOT(fill_steps(QString, bool, bool)));

  connect(step_widget, SIGNAL(load_image(QString, bool)),
	  this, SLOT(load(QString, bool)));
  connect(step_widget, SIGNAL(load_xml(QString)),
	  this, SLOT(load_xml(QString)));

  connect(this, SIGNAL(updated(DomModel*)),
	  property_wgt, SLOT(update(DomModel*)));
  connect(this, SIGNAL(mode_changed(bool)),
	  key_wgt_, SLOT(change_mode(bool)));
  connect(this, SIGNAL(updated(DomModel*)),
	  xml_wgt, SLOT(update(DomModel*)));
  connect(this, SIGNAL(updated(DomModel*)),
	  image_wgt, SLOT(update()));

  connect(key_wgt_, SIGNAL(updated(int, bool)),
	  this, SIGNAL(key_updated(int, bool)));

  connect(scene_, SIGNAL(selected(QModelIndex)),
	  property_wgt, SLOT(select(QModelIndex)));
  connect(scene_, SIGNAL(deselected(QModelIndex)),
	  property_wgt, SLOT(deselect(QModelIndex)));
  connect(scene_, SIGNAL(selected(QModelIndex)),
	  xml_wgt, SLOT(select(QModelIndex)));
  connect(scene_, SIGNAL(deselected(QModelIndex)),
	  xml_wgt, SLOT(deselect(QModelIndex)));

  connect(image_wgt, SIGNAL(scaleUpdated(qreal)),
	  this, SLOT(maybeChangeCacheMode(qreal)));
}

void
Viewer::load_xml(QString filename)
{
  QString xml_file = filename;
  xml_file_ = filename;

  app_->setOverrideCursor(QCursor(Qt::WaitCursor));

  scene_->removeItem(image_);
  scene_->clear();
  scene_->addItem(image_);

  scene_->update();

  if (doc_layout_)
    {
      doc_layout_->deleteLater();
      doc_layout_ = 0;
    }

  emit updated(doc_layout_);

  if (QFile::exists(xml_file))
  {
    QFile file(xml_file);
    if (file.open(QIODevice::ReadOnly))
    {
      QDomDocument document;
      if (document.setContent(&file))
      {
	doc_layout_ = new DomModel(document, this);
      }
      else
	{
	  app_->restoreOverrideCursor();
	  QMessageBox msgBox;
	  msgBox.setText("Error while loading the XML file, please choose another.");
	  msgBox.exec();
	}
      file.close();
    }
  }

  xml_to_layout();

  app_->restoreOverrideCursor();
}

void
Viewer::xml_to_layout()
{

  /* /!\ XML parsing is VERY UGLY /!\
     TO DO: use same parsing as xml_transfrom. */


  // Add layout info to the scene.
  if (doc_layout_)
    {
      QModelIndex pgGts = doc_layout_->index(1, 0);
      QModelIndex page = doc_layout_->index(1, 0, pgGts);
      QModelIndex region;
      QModelIndex attributes;
      QModelIndex coords;
      QModelIndex point;
      for (int i = 0; true; ++i)
	{
	  region = doc_layout_->index(i, 0, page);
	  attributes = doc_layout_->index(i, 1, page);
	  QString name = doc_layout_->data(region, Qt::DisplayRole).toString();
	  region::RegionId id = static_cast<region::RegionId>(region_ids_[name]);

	  coords = doc_layout_->index(0, 0, region);
	  if (!region.isValid() || !coords.isValid())
	    break;

	  QVector<QPoint> points;
	  for (int j = 0; true; ++j)
	    {
	      // Navigate to the coordinate list
	      point = doc_layout_->index(j, 1, coords);
	      if (!point.isValid())
		break;

	      QMap<QString, QVariant> data =
		doc_layout_->data(point, Qt::UserRole).toMap();
	      int x = data["x"].toInt();
	      int y = data["y"].toInt();

	      points << QPoint(x, y);
	    }

	  // Create region
	  ImageRegion* r = new ImageRegion(id,
					   key_map_[id].first,
					   key_map_[id].second,
					   attributes, points,
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

	  // EXTENDED MODE
	  if (extended_mode_)
	    {
              for (int k = 1; true; ++k)
		{
		  QModelIndex paragraph = doc_layout_->index(k, 0, region);
		  QModelIndex attributes_par = doc_layout_->index(k, 1, region);
		  if (!paragraph.isValid())
		    break;

		  QString name_par = doc_layout_->data(paragraph, Qt::DisplayRole).toString();
		  region::RegionId id_par = static_cast<region::RegionId>(region_ids_[name_par]);

		  QDebug(&name_par) << name_par;

		  QModelIndex par_coords = doc_layout_->index(0, 0, paragraph);
		  QModelIndex point_par;

		  QVector<QPoint> points_par;
		  for (int m = 0; true; ++m)
		    {
		      // Navigate to the coordinate list
		      point_par = doc_layout_->index(m, 1, par_coords);
		      if (!point_par.isValid())
			break;

		      QMap<QString, QVariant> data_par =
			doc_layout_->data(point_par, Qt::UserRole).toMap();
		      int x = data_par["x"].toInt();
		      int y = data_par["y"].toInt();
		      points_par << QPoint(x, y);
		    }

		  // Create region
		  ImageRegion* r_par = new ImageRegion(id_par,
						       key_map_[id_par].first,
						       key_map_[id_par].second,
						       attributes_par, points_par,
						       outline_action_->isChecked(),
						       fill_action_->isChecked(),
						       precise_action_->isChecked(),
						       key_wgt_->isChecked(id_par));

		  connect(this, SIGNAL(key_updated(int, bool)),
			  r_par, SLOT(setDrawIfSameId(int, bool)));
		  connect(this, SIGNAL(setOutline(bool)),
			  r_par, SLOT(setOutline(bool)));
		  connect(this, SIGNAL(setPrecise(bool)),
			  r_par, SLOT(setPrecise(bool)));
		  connect(this, SIGNAL(setFill(bool)),
			  r_par, SLOT(setFill(bool)));

		  scene_->addItem(r_par);

		  for (int l = 1; true; ++l)
		    {
		      QModelIndex line = doc_layout_->index(l, 0, paragraph);
		      QModelIndex attributes_line = doc_layout_->index(l, 1, paragraph);
		      if (!line.isValid())
			break;

		      QString name_line = doc_layout_->data(line, Qt::DisplayRole).toString();
		      region::RegionId id_line = static_cast<region::RegionId>(region_ids_[name_line]);

		      QModelIndex line_coords = doc_layout_->index(0, 0, line);
		      QModelIndex point_line;
		      QVector<QPoint> points_line;
		      for (int n = 0; true; ++n)
			{
			  // Navigate to the coordinate list
			  point_line = doc_layout_->index(n, 1, line_coords);
			  if (!point_line.isValid())
			    break;

			  QMap<QString, QVariant> data_line =
			    doc_layout_->data(point_line, Qt::UserRole).toMap();
			  int x = data_line["x"].toInt();
			  int y = data_line["y"].toInt();
			  points_line << QPoint(x, y);
			}

		      // Create region
		      ImageRegion* r_line = new ImageRegion(id_line,
							   key_map_[id_line].first,
							   key_map_[id_line].second,
							   attributes_line, points_line,
							   outline_action_->isChecked(),
							   fill_action_->isChecked(),
							    precise_action_->isChecked(),
							    key_wgt_->isChecked(id_line));

		      connect(this, SIGNAL(key_updated(int, bool)),
			      r_line, SLOT(setDrawIfSameId(int, bool)));
		      connect(this, SIGNAL(setOutline(bool)),
			      r_line, SLOT(setOutline(bool)));
		      connect(this, SIGNAL(setPrecise(bool)),
			      r_line, SLOT(setPrecise(bool)));
		      connect(this, SIGNAL(setFill(bool)),
			      r_line, SLOT(setFill(bool)));

		      scene_->addItem(r_line);
		    }
		}
	    }
	  // END OF EXTENDED MODE

	}

      emit updated(doc_layout_);
      key_wgt_->update_all();
    }
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
  scene_->addItem(image_);

  if (no_cache_)
    image_->setCacheMode(QGraphicsItem::NoCache);
  else
    image_->setCacheMode(QGraphicsItem::DeviceCoordinateCache);

  if (doc_layout_)
  {
    doc_layout_->deleteLater();
    doc_layout_ = 0;
  }

  app_->restoreOverrideCursor();

  emit updated(doc_layout_);
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
  if (xml_file_ != QString(""))
    load_xml(xml_file_);

  emit mode_changed (b);
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

//  LocalWords:  hh
