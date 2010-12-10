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

#ifndef SCRIBO_DEMO_XML2DOC_LOADER_HH
# define SCRIBO_DEMO_XML2DOC_LOADER_HH

# include <QtCore>
# include <QDomDocument>

class Loader : public QObject
{
  Q_OBJECT
  public:

  Loader();
  ~Loader();

  //  void merge(const QString& in, const QString& other, const
  //  QString& output = "output.xml");
  bool xml_output(const QString& xml_file, bool html, const QString& output);
  QDomDocument* xml_to_dom(const QString& xml_file);
  bool set_output(QString& output);
  void add_pdf_templates(bool crop, bool base64, const QString& output);
  void add_svg_templates(const QString& output);
  void add_open_templates(const QString& output);
  void add_html_templates(bool base64, const QString& output);
};

#endif // ! SCRIBO_DEMO_XML2DOC_LOADER_HH
