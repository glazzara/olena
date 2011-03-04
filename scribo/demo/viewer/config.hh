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

#ifndef SCRIBO_DEMO_VIEWER_CONFIG_HH
# define SCRIBO_DEMO_VIEWER_CONFIG_HH

# include <QSettings>

class config : public QSettings
{
public:

  config();

  static config* get_instance();

  // Preprocessing
  bool preprocessing_subsample();
  void set_preprocessing_subsample(bool b);

  bool preprocessing_remove_bg();
  void set_preprocessing_remove_bg(bool b);

  bool preprocessing_deskew();
  void set_preprocessing_deskew(bool b);

  bool preprocessing_remove_noise();
  void set_preprocessing_remove_noise(bool b);

  int preprocessing_bin_algo();
  void set_preprocessing_bin_algo(int algo);


  // Segmentation
  int segmentation_find_seps();
  void set_segmentation_find_seps(int seps);


  // OCR
  bool ocr_enabled();
  void set_ocr_enabled(bool b);

  QString ocr_language();
  void set_ocr_language(const QString& lang);


  // General options
  bool general_save_xml_enabled();
  void set_general_save_xml_enabled(bool b);

  bool general_save_xml_same_dir();
  void set_general_save_xml_same_dir(bool b);

  bool general_save_xml_custom_dir();
  void set_general_save_xml_custom_dir(bool b);

  QString general_save_xml_custom_dir_path();
  void set_general_save_xml_custom_dir_path(const QString& path);


};


#endif // ! SCRIBO_DEMO_VIEWER_CONFIG_HH
