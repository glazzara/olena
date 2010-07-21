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
# include "common.hh"

# include <limits.h>

#include <scribo/preprocessing/crop.hh>
#include <mln/value/rgb8.hh>
#include <mln/core/alias/box2d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/io/magick/save.hh>
#include <mln/io/ppm/all.hh>

ImageCrop::ImageCrop(QString xml, QString img, QString output) :
  xml_(xml),
  image_(img),
  output_dir_(output)
{
}

ImageCrop::~ImageCrop()
{
}

/* Save PPM image to PNG format in output_dir/img. */
void ImageCrop::save_image(QString file)
{
  using namespace mln;

  image2d<value::rgb8> ima;
  io::ppm::load(ima, image_.toStdString());

  QString filename;
  if (file == QString::Null())
    filename =  Common::get_file_name(image_);
  else
    filename = file;

  //  std::cout << "Saving " << image_.toStdString() << " to "
  //	    << output_dir_.toStdString() << "img/"
  //	    << filename.toStdString()  << ".png"
  //	    << std::endl;

  io::magick::save(ima, output_dir_.toStdString() + "img/" + filename.toStdString() + ".png");
}

/* Return the image in base 64. */
QString ImageCrop::img_to_base64()
{
  QFile f(image_);
  f.open(QIODevice::ReadOnly);

  QByteArray file_content = f.readAll();

  f.close();

  return file_content.toBase64();
}

/* Decode the base 64 string str and save into output_dir_/img/img_name. */
bool ImageCrop::img_from_base64(QString str, QString img_name, QString mime)
{
  QByteArray ba;

  ba = ba.append(str);
  QByteArray out_ba = QByteArray::fromBase64(ba);
  QImage ima = QImage::fromData(out_ba);

  return ima.save(output_dir_ + "img/" + img_name + "." + mime);
}

/* Read all regions of the XML file and save all base 64 data into output_dir/img */
void ImageCrop::from_base64()
{
  QFile f_in(xml_);
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
      if (child.toElement().tagName().contains(QRegExp("(image|graphic|chart|separator|table)_region")))
	{
	  QDomNode node = child.firstChild();
	  QString id = child.toElement().attribute("id", "none");

	  while (!node.isNull() && !node.toElement().tagName().contains("container"))
	    node = node.nextSibling();

	  if (!node.isNull())
	    {
	      QString data = node.firstChildElement("data").text();
	      QString mime = node.firstChildElement("mime").text();
	      img_from_base64(data, id, mime);
	    }
	}
      child = child.nextSibling();
    }
}

/* Tranfsorm the input XML file associated with images into a single
   XML output containing datas if images in base 64. */
void ImageCrop::to_base64(QString out_file, bool no_crop)
{
  QFile file(xml_);
  file.open(QIODevice::ReadOnly);
  QTextStream stream(&file);

  QFile file2(out_file);
  file2.open(QIODevice::ReadWrite);
  QTextStream stream2(&file2);

  QString line = stream.readLine();
  stream2 << line;
  line = stream.readLine();

  // HEAD
  while (!line.contains("<page"))
    {
      stream2 << "\n" << line;
      line = stream.readLine();
    }

  if (no_crop)
    {
      stream2 << "\n" << line;

      stream2 << "\n" << "    <image_region id=\"image\">";
      stream2 << "\n" << "      <container>\n";
      stream2 << "        <mime>png</mime>\n";

      QString file_name = Common::get_file_name(image_);
      save_image();
      QFile img(output_dir_ + "img/" + file_name + ".png");

      img.open(QIODevice::ReadOnly);
      stream2 << "        <data>\n";

      QByteArray byte = img.readAll();
      stream2 << byte.toBase64();

      stream2 << "        </data>";
      stream2 << "\n      </container>";
      stream2 << "\n      <coords>\n";

      stream2 << "        <point x=\"0\" y=\"0\" />\n";
      stream2 << "        <point x=\"0\" y=\"0\" />";

      stream2 << "\n      </coords>";
      stream2 << "\n" << "    </image_region>\n";
      img.close();

      line = stream.readLine();

      while(!line.contains("</pcGts>"))
	{
	  stream2 << "\n" << line;
	  line = stream.readLine();
	}

      stream2 << "\n" << line;
    }
  else
    {
      while(!line.contains("</pcGts>"))
	{
	  stream2 << "\n" << line;
	  if (line.contains(QRegExp("<(image|graphic|chart|separator|table)_region")))
	    {
	      stream2 << "\n" << "      <container>\n";
	      stream2 << "        <mime>png</mime>\n";

	      QDomDocument doc;
	      doc.setContent(line);

	      QDomElement root = doc.documentElement();
	      QString id = root.attribute("id", "none");

	      QFile img(output_dir_ + "img/" + id + ".png");

	      img.open(QIODevice::ReadOnly);
	      stream2 << "        <data>\n";

	      QByteArray byte = img.readAll();
	      stream2 << byte.toBase64();

	      stream2 << "        </data>";
	      stream2 << "\n      </container>";
	      img.close();
	    }

	  line = stream.readLine();
	}

      stream2 << "\n" << line;
    }

  file2.close();
  file.close();
}

bool ImageCrop::crop_regions()
{
  Loader loader;
  QFile f(xml_);
  f.open(QIODevice::ReadOnly);
  QDomDocument doc;

  if (doc.setContent(&f))
    {
      bool found_regions = false;
      f.close();

      QDomElement root = doc.documentElement();
      QDomNode page = root.firstChild();

      while (!page.isNull() && !page.toElement().tagName().contains("page"))
	page = page.nextSibling();

      if (page.isNull())
	return false;

      QDomNode region = page.firstChild();

      while (!region.isNull())
	{
	  if (region.toElement().tagName().contains(QRegExp("(image|graphic|chart|separator|table)_region")))
	    {
	      found_regions = true;

	      QDomNode coords = region.firstChild();
	      QString id = region.toElement().attribute("id", "none");

	      qDebug() << region.toElement().tagName();

	      while (!coords.isNull() && !coords.toElement().tagName().contains("coords"))
		coords = coords.nextSibling();

	      if (coords.isNull())
		break;

	      QDomNode point = coords.firstChild();
	      int x_max = 0;
	      int y_max = 0;
	      int x_min = INT_MAX;
	      int y_min = INT_MAX;

	      while (!point.isNull())
		{
		  int x = point.toElement().attribute("x", "none").toInt();
		  int y = point.toElement().attribute("y", "none").toInt();

		  if (x < x_min)
		    x_min = x;
		  if (x > x_max)
		    x_max = x;

		  if (y < y_min)
		    y_min = y;
		  if (y > y_max)
		    y_max = y;

		  point = point.nextSibling();
		}

	      using namespace mln;
	      box2d box = make::box2d(y_min, x_min, y_max, x_max);

	      image2d<value::rgb8> ima;
	      io::ppm::load(ima, image_.toStdString());
	      ima = scribo::preprocessing::crop(ima, box);

	      io::magick::save(ima, output_dir_.toStdString() + "img/" + id.toStdString() + ".png");
	    }
	  region = region.nextSibling();
	}

      return found_regions;
    }
  else
    {
      f.close();
      return false;
    }
}
