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

#include <limits.h>

#include <scribo/core/macros.hh>
#include <scribo/preprocessing/crop.hh>
#include <mln/value/rgb8.hh>
#include <mln/core/alias/box2d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/util/couple.hh>
#include <mln/draw/line.hh>
#include <mln/io/magick/save.hh>
#include <mln/io/magick/load.hh>
#include <mln/io/ppm/all.hh>

#include "image_crop.hh"
#include "loader.hh"
#include "common.hh"

#include <mln/io/pbm/save.hh>

ImageCrop::ImageCrop(const QString& xml, const QString& img,
		     const QString& output)
  : xml_(xml),
    image_(img),
    output_dir_(output)
{
  if (!image_.isEmpty())
    mln::io::magick::load(ima_, image_.toStdString());
}

ImageCrop::~ImageCrop()
{
  // Cleanup temporary files.
  foreach (QString value, region_map_)
    QFile::remove(value);
}


/* Save image to PNG format in output_dir/img. */
void ImageCrop::save_image(const QString& out)
{
  mln::io::magick::save(ima_, out.toStdString());
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


/* Decode the base 64 string str and save into
 * output_dir_/img/img_name. */
bool ImageCrop::img_from_base64(const QString& str, const QString& img)
{
  QByteArray ba;

  ba = ba.append(str);
  QByteArray out_ba = QByteArray::fromBase64(ba);
  QImage ima = QImage::fromData(out_ba);

  return ima.save(output_dir_ + img);
}

/* Read all regions of the XML file and save all base 64 data into
 * output_dir/img */
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
      if (child.toElement().tagName().contains(QRegExp("(image|graphic|chart|horizontal_separator|vertical_separator|table)_region")))
	{
	  QDomNode node = child.firstChild();
	  QString id = child.toElement().attribute("id", "none");

	  while (!node.isNull() && !node.toElement().tagName().contains("container"))
	    node = node.nextSibling();

	  if (!node.isNull())
	    {
	      QString data = node.firstChildElement("data").text();
	      QString mime = node.firstChildElement("mime").text();
	      QString img = id + "." + mime;
	      img_from_base64(data, img);
	    }
	}
      child = child.nextSibling();
    }
}

/* Tranfsorm the input XML file associated with images into a single
   XML output containing datas if images in base 64. */
void ImageCrop::to_base64(const QString& out_file, bool no_crop)
{
  QFile file(xml_);
  file.open(QIODevice::ReadOnly);

  QDomDocument doc;
  doc.setContent(&file);

  QDomElement root = doc.documentElement();
  QDomNode child = root.firstChild();

  while (!child.isNull() && !child.toElement().tagName().contains("page"))
    child = child.nextSibling();

  QString width = child.toElement().attribute("image_width", "0");
  QString height = child.toElement().attribute("image_height", "0");

  file.close();
  file.open(QIODevice::ReadOnly);
  QTextStream stream(&file);
  stream.setCodec("UTF-8");

  QFile file2(out_file);
  file2.open(QIODevice::ReadWrite);
  QTextStream stream2(&file2);
  stream2.setCodec("UTF-8");

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

      QTemporaryFile tmp(output_dir_ + Common::get_file_name(image_) + "_XXXXXX.png");
      tmp.open();
      save_image(tmp.fileName());
      QFile img(tmp.fileName());

      img.open(QIODevice::ReadOnly);
      stream2 << "        <data>\n";

      QByteArray byte = img.readAll();
      stream2 << byte.toBase64();

      stream2 << "        </data>";
      stream2 << "\n      </container>";
      stream2 << "\n      <coords>\n";

      stream2 << "        <point x=\"0\" y=\"0\" />\n";
      stream2 << "        <point x=\"" << width << "\" y=\"0\" />\n";
      stream2 << "        <point x=\"" << width << "\" y=\"" << height << "\" />\n";
      stream2 << "        <point x=\"0\" y=\"" << height << "\" />";

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
	  if (line.contains(QRegExp("<(image|graphic|chart|horizontal_separator|vertical_separator|table)_region")))
	    {
	      stream2 << "\n" << "      <container>\n";
	      stream2 << "        <mime>png</mime>\n";

	      QDomDocument doc;
	      doc.setContent(line);

	      QDomElement root = doc.documentElement();
	      QString id = root.attribute("id", "none");

	      QFile img(region_map_[id]);

	      img.open(QIODevice::ReadOnly);
	      stream2 << "        <data>\n";

	      QByteArray byte = img.readAll();
	      stream2 << byte.toBase64();

	      stream2 << "        </data>";
	      stream2 << "\n      </container>";
	      img.remove();
	      img.close();
	    }

	  line = stream.readLine();
	}

      stream2 << "\n" << line;
    }

  file2.close();
  file.close();
}

bool ImageCrop::crop_regions(bool temp)
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
	  if (region.toElement().tagName().contains(QRegExp("(image|graphic|chart|vertical_separator|horizontal_separator|table)_region")))
	    {
	      found_regions = true;

	      QDomNode coords = region.firstChild();
	      QString id = region.toElement().attribute("id", "none");

	      // Retrieve region bbox.
	      using namespace mln;
	      def::coord
		x_min = region.toElement().attribute("x_min").toInt(),
		y_min = region.toElement().attribute("y_min").toInt(),
		x_max = region.toElement().attribute("x_max").toInt(),
		y_max = region.toElement().attribute("y_max").toInt();
	      box2d box = make::box2d(y_min, x_min, y_max, x_max);

	      while (!coords.isNull() && !coords.toElement().tagName().contains("coords"))
		coords = coords.nextSibling();

	      if (coords.isNull())
		break;

	      QDomNode point = coords.firstChild();

	      // For each row, store first and last column of the line
	      // to be set as object. The rest will be considered as
	      // transparent.
	      util::array<util::couple<def::coord, def::coord> >
		p_mask(box.nrows(),
		       util::couple<def::coord, def::coord>(x_max + 1, x_min - 1));

	      // Compute opacity mask for image region.
	      while (!point.isNull())
		{
		  int x = point.toElement().attribute("x", "none").toInt();
		  int y = point.toElement().attribute("y", "none").toInt();

		  if (p_mask(y - y_min).first() > x)
		    p_mask(y - y_min).first() = x;
		  if  (p_mask(y - y_min).second() < x)
		    p_mask(y - y_min).second() = x;

		  point = point.nextSibling();
		}

	      // Crop image in input.
	      image2d<value::rgb8> crop = scribo::preprocessing::crop(ima_, box);

	      // Build image mask.
	      image2d<bool> opacity_mask(box);
	      {
		data::fill(opacity_mask, true);
		for_all_elements(e, p_mask)
		  draw::line(opacity_mask,
			     point2d(e + y_min, p_mask(e).first()),
			     point2d(e + y_min, p_mask(e).second()),
			     false);
	      }

	      if (temp)
		{
		  QTemporaryFile tmp(output_dir_ + id + ".XXXXXX.png");
		  tmp.open();
		  region_map_[id] = tmp.fileName();
		  tmp.setAutoRemove(false);
		  io::magick::save(crop, opacity_mask, tmp.fileName().toStdString());
		}
	      else
		io::magick::save(crop, opacity_mask, QString(output_dir_ + id + ".png").toStdString());
	    }

	  region = region.nextSibling();
	}

      return found_regions;
    }
  else
    {
      f.close();
      qDebug() << "Error while reading XML file!";
      abort();
      return false;
    }
}
