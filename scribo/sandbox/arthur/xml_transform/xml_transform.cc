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
# include "xml_transform.hh"

XmlTransform::XmlTransform(QString output, QString xml_file) :
  output_dir_(output),
  xml_file_(xml_file)
{
  Loader loader;
  if (!loader.set_output(output_dir_))
    abort();
}

XmlTransform::~XmlTransform()
{
}

void XmlTransform::createPDF (QString img, bool crop)
{
  Loader loader;

  QString output = output_dir_;
  output.append("img");

  if (loader.set_output(output))
    {
      if (loader.load_xml(xml_file_, false, output_dir_))
	{

	  ImageCrop Crop;

	  if (crop)
	    Crop.crop_regions(xml_file_, img, output_dir_);
	  else
	    Crop.save_image(img, output_dir_);

	  loader.add_pdf_templates(crop, output_dir_);
	}
      else
	abort();
    }
  else
    abort();
}

void XmlTransform::createHTML(QString img)
{
  Loader loader;

  QString output = output_dir_;
  output.append("img");

  if (loader.set_output(output))
    {
      if (loader.load_xml(xml_file_, true, output_dir_))
	{
	  ImageCrop Crop;

	  Crop.crop_regions(xml_file_, img, output_dir_);
	  loader.add_html_templates(output_dir_);
	}
      else
	abort();
    }
  else
    abort();
}

