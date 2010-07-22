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

# include "xml_transform.hh"
# include "image_crop.hh"
# include "loader.hh"
# include "common.hh"
# include <iostream>

XmlTransform::XmlTransform(QString xml_file, QString image_file, QString output, QString file) :
  xml_file_(xml_file),
  image_(image_file),
  loader_(new Loader()),
  file_(file)
{
  QFile fi(image_);
  QFile fx(xml_file_);

  if (file_ != QString::Null())
    output_dir_ = "/tmp/xml_transform";
  else
    output_dir_ = output;

  if (output_dir_ != QString::Null() && !loader_->set_output(output_dir_))
    {
      std::cout << "Cant't create " << output_dir_.toStdString() << "dir! Aborting..." << std::endl;
      abort();
    }
  else if (!fi.exists() && image_ != QString::Null())
    {
      std::cout << "Image file does not exist ! Aborting..." << std::endl;
      abort();
    }
  else if (!fx.exists())
    {
      std::cout << "XML file does not exist ! Aborting..." << std::endl;
      abort();
    }

  crop_ = new ImageCrop(xml_file_, image_, output_dir_);
}

XmlTransform::~XmlTransform()
{
}

void XmlTransform::fromBase64()
{
  QString output = output_dir_;
  output.append("img");

  if (loader_->set_output(output))
    crop_->from_base64();
  else
    abort();
}

void XmlTransform::toBase64(bool crop)
{
  QString output = output_dir_;
  output.append("img");

  if (loader_->set_output(output))
    {
      if (!crop)
	crop_->crop_regions();

      crop_->to_base64(file_, crop);
    }
  else
    abort();
}

void XmlTransform::createPDF (bool crop, bool base64)
{
  if (loader_->xml_output(xml_file_, false, output_dir_))
    {
      if (!base64)
	{
	  QString output = output_dir_;
	  output.append("img");

	  if (loader_->set_output(output))
	    {
	      if (crop)
		crop_->crop_regions();
	      else
		crop_->save_image("image");
	    }
	}

      loader_->add_pdf_templates(crop, base64, output_dir_);
    }
  else
    abort();
}

void XmlTransform::createSVG ()
{
  if (loader_->xml_output(xml_file_, false, output_dir_))
    {
      loader_->add_svg_templates(output_dir_);
    }
  else
    abort();
}

void XmlTransform::createHTML(bool base64)
{
  if (loader_->xml_output(xml_file_, true, output_dir_))
    {
      if (!base64)
	{
	  QString output = output_dir_;
	  output.append("img");
	  if (loader_->set_output(output))
	    crop_->crop_regions();
	  else
	    abort();
	}
      loader_->add_html_templates(base64, output_dir_);
    }
  else
    abort();
}

