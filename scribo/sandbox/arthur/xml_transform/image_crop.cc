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

# include "image_crop.hh"
# include "loader.hh"
# include "dommodel.hh"

# include <limits.h>

#include <scribo/preprocessing/crop.hh>
#include <mln/value/rgb8.hh>
#include <mln/core/alias/box2d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/io/magick/save.hh>
#include <mln/io/ppm/all.hh>

ImageCrop::ImageCrop()
{
}

ImageCrop::~ImageCrop()
{
}

void ImageCrop::save_image(QString image, QString output)
{
  using namespace mln;

  image2d<value::rgb8> ima;
  io::ppm::load(ima, image.toStdString());

  io::magick::save(ima, output.toStdString() + "img/image.png");
}

void ImageCrop::crop_regions(QString xml_file, QString image_file, QString output)
{
  Loader loader;

  QFile f(image_file);

  if (!f.exists())
    {
      qDebug() << "Image doesn't exist !";
      abort();
    }
  DomModel* layout = loader.xml_to_dom(xml_file);
  if (layout)
    {
      QModelIndex pcGts = layout->index(1, 0);
      QModelIndex page = layout->index(1, 0, pcGts);
      QModelIndex region;
      QModelIndex attributes;
      QModelIndex coords;
      QModelIndex point;
      bool regions_found = false;

      for (int i = 0; true; ++i)
	{
	  region = layout->index(i, 0, page);
	  attributes = layout->index(i, 1, page);

	  QString name = layout->data(region, Qt::DisplayRole).toString();
	  coords = layout->index(0, 0, region);

	  bool is_region =
	    name == QString("image_region")
	    || name == QString("graphic_region")
	    || name == QString("separator_region")
	    || name == QString("chart_region")
	    || name == QString("table_region") ;

	  if (!region.isValid() || !coords.isValid())
	    break;

	  if (is_region)
	    {
	      regions_found = true;
	      QMap<QString, QVariant> data =
		layout->data(attributes, Qt::UserRole).toMap();

	      QString id;
	      QMap<QString, QVariant>::iterator it = data.find("id");

	      if (it == data.end() || it.key() != "id")
		qDebug() << "WTF_Error : No image region.";

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
		  point = layout->index(j, 1, coords);
		  if (!point.isValid())
		    break;

		  QMap<QString, QVariant> data =
		    layout->data(point, Qt::UserRole).toMap();
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
	      io::ppm::load(ima, image_file.toStdString());
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
	      io::magick::save(ima, output.toStdString() + "img/" + id.toStdString() + ".png");

	    }
	}

      if (!regions_found)
	qDebug() << "No regions found.";

    }
  else
    {
      qDebug() << "Error with XML file.";
    }


}
