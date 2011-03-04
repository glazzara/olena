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

#include <QDir>

#include "config.hh"
#include "defs.hh"

// Defines enum of binarization algorithms
# include <scribo/toolchain/internal/text_in_doc_preprocess_functor.hh>


config::config()
  : QSettings("Olena-Scribo", "viewer")
{

}

config* config::get_instance()
{
  static config * conf = new config();
  return conf;
}


// Preprocessing

bool config::preprocessing_subsample()
{
  return value("preprocessing/subsample", false).toBool();
}

void config::set_preprocessing_subsample(bool b)
{
  setValue("preprocessing/subsample", b);
}


bool config::preprocessing_remove_bg()
{
  return value("preprocessing/remove_bg", false).toBool();
}

void config::set_preprocessing_remove_bg(bool b)
{
  setValue("preprocessing/remove_bg", b);
}


bool config::preprocessing_deskew()
{
  return value("preprocessing/deskew", false).toBool();
}

void config::set_preprocessing_deskew(bool b)
{
  setValue("preprocessing/deskew", b);
}


bool config::preprocessing_remove_noise()
{
  return value("preprocessing/remove_noise", true).toBool();
}

void config::set_preprocessing_remove_noise(bool b)
{
  setValue("preprocessing/remove_noise", b);
}


int config::preprocessing_bin_algo()
{
  return value("preprocessing/bin_algo",
	       scribo::toolchain::internal::SauvolaMs).toInt();
}

void config::set_preprocessing_bin_algo(int algo)
{
  setValue("preprocessing/bin_algo", algo);
}



// Segmentation

int config::segmentation_find_seps()
{
  return value("segmentation/find_seps", defs::LinesAndWhitespaces).toInt();
}

void config::set_segmentation_find_seps(int seps)
{
  setValue("segmentation/find_seps", seps);
}


// OCR

bool config::ocr_enabled()
{
  return value("ocr/enabled", true).toBool();
}

void config::set_ocr_enabled(bool b)
{
  setValue("ocr/enabled", b);
}



QString config::ocr_language()
{
  return value("ocr/language", 0).toString();
}

void config::set_ocr_language(const QString& lang)
{
  setValue("ocr/language", lang);
}



// General options

bool config::general_save_xml_enabled()
{
  return value("general/save_xml/enabled", true).toBool();
}

void config::set_general_save_xml_enabled(bool b)
{
  setValue("general/save_xml/enabled", b);
}


bool config::general_save_xml_same_dir()
{
  return value("general/save_xml/same_dir", true).toBool();
}

void config::set_general_save_xml_same_dir(bool b)
{
  setValue("general/save_xml/same_dir", b);
}


bool config::general_save_xml_custom_dir()
{
  return value("general/save_xml/custom_dir", false).toBool();
}

void config::set_general_save_xml_custom_dir(bool b)
{
  setValue("general/save_xml/custom_dir", b);
}


QString config::general_save_xml_custom_dir_path()
{
  return value("general/save_xml/custom_dir_path", QDir::tempPath()).toString();
}

void config::set_general_save_xml_custom_dir_path(const QString& path)
{
  setValue("general/save_xml/custom_dir_path", path);
}

