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

#ifndef SCRIBO_DEMO_XML2DOC_XML_TRANFORM_HH
# define SCRIBO_DEMO_XML2DOC_XML_TRANFORM_HH

# include <QtCore>


class Loader;
class ImageCrop;

class XmlTransform : public QObject
{
  Q_OBJECT
  public:

  XmlTransform(const QString& xml_file, const QString& image_file,
	       const QString& output, const QString& file = QString::Null());
  ~XmlTransform();

  QString out() { return output_dir_; }
  void createHTML(bool base64);
  void createPDF(bool crop, bool base64);
  void createSVG();
  void createOpen();
  void toBase64(bool crop);
  void fromBase64();

private:
  QString output_dir_;
  QString xml_file_;
  QString image_;
  Loader* loader_;
  ImageCrop* crop_;
  QString file_;
  bool tmp_;
};

#endif // ! SCRIBO_DEMO_XML2DOC_XML_TRANFORM_HH

