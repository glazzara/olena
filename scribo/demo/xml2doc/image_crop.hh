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

#ifndef SCRIBO_DEMO_XML2DOC_IMAGE_CROP_HH
# define SCRIBO_DEMO_XML2DOC_IMAGE_CROP_HH

# include <QDomDocument>
# include <QtCore>

# include <mln/value/rgb8.hh>
# include <mln/core/image/image2d.hh>


class DomModel;

class ImageCrop : public QObject
{
  Q_OBJECT
  public:

  ImageCrop(const QString&, const QString&, const QString&);
  ~ImageCrop();

  void save_image(const QString&);
  bool crop_regions(bool temp = false);

  QString img_to_base64();
  bool img_from_base64(const QString&, const QString&);
  void to_base64(const QString&, bool);

  void from_base64();

private:
  QString xml_;
  QString image_;
  QString output_dir_;
  QMap<QString, QString> region_map_;
  mln::image2d<mln::value::rgb8> ima_;
};

#endif // ! SCRIBO_DEMO_XML2DOC_IMAGE_CROP_HH
