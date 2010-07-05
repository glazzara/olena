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

# include "xml_to_html.hh"
# include "dommodel.hh"
# include <limits.h>

#include <scribo/preprocessing/crop.hh>
#include <mln/value/rgb8.hh>
#include <mln/core/alias/box2d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/io/magick/save.hh>
#include <mln/io/ppm/all.hh>

ImageCrop::ImageCrop():
  regions_(false)
{
}

ImageCrop::~ImageCrop()
{
}

void ImageCrop::merge(QString in, QString other, QString output)
{
  if (in.endsWith(".xml") && other.endsWith(".xml") && output.endsWith(".xml"))
    {
      QFile f_in(in);
      QFile f_other(other);
      QFile f_output(output);

      qDebug() << "Merging to " << output << "...";

      f_in.open(QIODevice::ReadOnly);
      f_other.open(QIODevice::ReadOnly);
      f_output.open(QIODevice::ReadWrite);

      QTextStream stream_in(&f_in);
      QTextStream stream_other(&f_other);
      QTextStream output_stream(&f_output);

      QString line = stream_in.readLine();
      output_stream << line;
      line = stream_in.readLine();

      while (!line.contains("</page>"))
	{
	  output_stream << "\n" << line;
	  line = stream_in.readLine();
	}

      line = stream_other.readLine();

      while (!line.contains("<page"))
	line = stream_other.readLine();

      line = stream_other.readLine();

      while (!line.contains("</pcGts>"))
	{
	  output_stream << "\n" << line;
	  line = stream_other.readLine();
	}

      output_stream << "\n" << line;

      f_in.close();
      f_other.close();
      f_output.close();
    }
  else
    std::cout << "merge : error, only xml files are needed." << std::endl;
}

bool ImageCrop::load_xml(QString xml_file, QString image_file, QString path)
{
  if (xml_file.endsWith(".xml") && (image_file.endsWith(".ppm") || image_file == QString::Null()) )
    {
      image_file_ = image_file;
      regions_ = (image_file != QString::Null());

      path_ = path;
      if (!path_.endsWith("/"))
	path_.append("/");

      QDir dir(path_);

      if (!dir.mkpath("img"))
	{
	  path_ = "output/";
	  dir.mkpath(path_);
	  dir.setPath(path_);
	  dir.mkpath("img");
	}

      if (QFile::exists(xml_file))
	{
	  QFile file(xml_file);
	  QFile output(path_ + "output.xml");
	  QFile xsl("patterns/xsl.xsl");
	  QFile css("patterns/css.css");
	  xsl.copy(path_ + "xsl.xsl");
	  css.copy(path_ + "css.css");

	  if (QFile::exists(path_ + "output.xml"))
	    output.remove();

	  output.open(QIODevice::ReadWrite);
	  file.open(QIODevice::ReadOnly);

	  QTextStream stream_in(&file);
	  QTextStream stream_out(&output);
	  stream_in.setCodec("UTF-8");
	  stream_out.setCodec("UTF-8");

	  QString line = stream_in.readLine();
	  stream_out << line;
	  stream_out << "\n<?xml-stylesheet type=\"text/xsl\" href=\"xsl.xsl\" ?>";

	  // /!\ attributes of ICDAR PcGts removed.
	  line = stream_in.readLine();
	  stream_out << "\n<pcGts>";

	  line = stream_in.readLine();

	  while (!line.contains("</pcGts>"))
	    {
	      stream_out << "\n" << line;
	      line = stream_in.readLine();
	    }

	  stream_out << "\n" << line;

	  output.close();
	  file.close();

	  qDebug() << "Output saved to " + path_;

	  if (file.open(QIODevice::ReadOnly))
	    {
	      QDomDocument document;
	      if (document.setContent(&file))
		{
		  layout_ = new DomModel(document, this);
		}
	      else
		qDebug() << "Error while loading the XML file, please choose another.";

	      file.close();
	    }
	}
      return true;
    }
    else
      {
	std::cout << "Wrong files format !" << std::endl;
	return false;
      }
}

void ImageCrop::find_image_regions()
{
  if (layout_ && regions_)
    {
      QModelIndex pgGts = layout_->index(1, 0);
      QModelIndex page = layout_->index(1, 0, pgGts);
      QModelIndex region;
      QModelIndex attributes;
      QModelIndex coords;
      QModelIndex point;
      bool regions_found = false;
      for (int i = 0; true; ++i)
	{
	  region = layout_->index(i, 0, page);
	  attributes = layout_->index(i, 1, page);
	  QString name = layout_->data(region, Qt::DisplayRole).toString();
	  coords = layout_->index(0, 0, region);

	  bool is_image_region = name == QString("image_region")
	    || name == QString("graphic_region")
	    || name == QString("separator_region")
	    || name == QString("chart_region")
	    || name == QString("table_region") ;

	  if (!region.isValid() || !coords.isValid())
	    break;

	  if (is_image_region)
	    {

	      regions_found = true;
	      QMap<QString, QVariant> data =
		layout_->data(attributes, Qt::UserRole).toMap();

	      QString id;
	      QMap<QString, QVariant>::iterator it = data.find("id");

	      if (it == data.end() || it.key() != "id")
		qDebug() << "No image region.";

	      while (it != data.end() && it.key() == "id")
		{
		  qDebug() << name;
		  id = it.value().toString();
		  qDebug() << it.key() + " = " + it.value().toString();
		  ++it;
		}

	      int x_max = 0;
	      int y_max = 0;
	      int x_min = INT_MAX;
	      int y_min = INT_MAX;

	      //	      QVector<QMap<QString, QVariant> > vect;
	      for (int j = 0; true; ++j)
		{
		  // Navigate to the coordinate list
		  point = layout_->index(j, 1, coords);
		  if (!point.isValid())
		    break;

		  QMap<QString, QVariant> data =
		    layout_->data(point, Qt::UserRole).toMap();
		  int x = data["x"].toInt();
		  int y = data["y"].toInt();

		  //  vect << data;

		  if (x < x_min)
		    x_min = x;
		  if (x > x_max)
		    x_max = x;

		  if (y < y_min)
		    y_min = y;
		  if (y > y_max)
		    y_max = y;

		}

	      using namespace mln;

	      box2d box = make::box2d(y_min, x_min, y_max, x_max);

	      image2d<value::rgb8> ima;
	      io::ppm::load(ima, image_file_.toStdString());
	      ima = scribo::preprocessing::crop(ima, box);

	      //  image2d<bool> mask = make::box2d(y_min, x_min, y_max, x_max);
	      // data::fill(mask, true);

	      /*for (int a = 1; a < vect.size(); ++a)
		{
		  int x = vect[a]["x"].toInt();
		  int y = vect[a]["y"].toInt();

		  data::fill((mask | make::box2d(y, x, y, x)).rw(), false);
		  }*/


	      // io::pbm::save(mask, "output/img/mask_" + id.toStdString());
	      io::magick::save(ima, path_.toStdString() + "img/" + id.toStdString() + ".png");

	    }
	}

      if (!regions_found)
	qDebug() << "No regions found.";
    }
}
