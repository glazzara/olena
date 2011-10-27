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
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.


#ifndef SCRIBO_TOOLCHAIN_NEPOMUK_TEXT_EXTRACTION_HH
# define SCRIBO_TOOLCHAIN_NEPOMUK_TEXT_EXTRACTION_HH

/// \file
///
/// Extract text from a document.


# include <QtCore/QSet>
# include <QtCore/QString>
# include <QtCore/QStringList>
# include <QtCore/QTextStream>
# include <QtCore/QTextCodec>
# include <QtGui/QImage>

# include <mln/core/image/image2d.hh>
# include <mln/data/transform.hh>
# include <mln/logical/not.hh>
# include <mln/value/qt/rgb32.hh>
# include <mln/fun/v2v/qt_rgb_to_int_u.hh>

# include <scribo/convert/from_qimage.hh>
# include <scribo/binarization/sauvola_ms.hh>
# include <scribo/preprocessing/deskew.hh>
# include <scribo/toolchain/text_in_doc.hh>


namespace scribo
{

  namespace toolchain
  {

    namespace nepomuk
    {

      /*! \brief Extract text from a document.

	This is a convenient routine to be used in Nepomuk.

	\param[in] input A document image.
	\param[in] language The main language used in the input
	                    document image. Improve text recognition
	                    quality if accurate.

	\return A set of recognized words.


	Don't forget to define NDEBUG for compilation to disable debug
	checks.

	Depending on your version of Tesseract (OCR) you may define
	HAVE_TESSERACT_2 or HAVE_TESSERACT_3 .

	\ingroup grptoolchain
       */
      QSet<QString>
      text_extraction(const QImage& input, const QString& language);


# ifndef MLN_INCLUDE_ONLY

      QSet<QString>
      text_extraction(const QImage& input, const QString& language = QString("eng"))
      {
	trace::entering("scribo::toolchain::nepomuk::text_extraction");

	mln_precondition(!input.isNull());

	typedef image2d<scribo::def::lbl_type> L;

	// Convert image to Milena's format.
	mln::image2d<mln::value::qt::rgb32>
	  input_mln = scribo::convert::from_qimage(input);

	image2d<bool> input_bin;


	// Preprocess
	{
	  // Convert to Gray level image.
	  image2d<value::int_u8>
	    input_gl = data::transform(input_mln,
				       mln::fun::v2v::qt_rgb_to_int_u<8>());

	  // Deskew if needed.
	  input_gl = preprocessing::deskew(input_gl);

	  // Binarize foreground to use it in the processing chain.
	  input_bin = scribo::binarization::sauvola_ms(input_gl, 51, 3);
	}




	line_set<L> lines_bg, lines_fg;
	// Process
	{
	  // Run document toolchain.
	  lines_bg = scribo::toolchain::text_in_doc(input_bin,
						    true,
						    language.toUtf8().data());

	  // Negate document.
	  logical::not_inplace(input_bin);

	  // Run document toolchain.
	  lines_fg = scribo::toolchain::text_in_doc(input_bin,
						    true,
						    language.toUtf8().data());
	}


	QSet<QString> output;

	// Construct output
	{
  	  QTextCodec *codec = QTextCodec::codecForName("UTF-8");

	  QString tmp_out;
	  QTextStream stream(&tmp_out, QIODevice::WriteOnly);
	  stream.setCodec("UTF-8");

	  for_all_lines(l, lines_bg)
	    if (lines_bg(l).has_text())
	      stream << " " << codec->toUnicode(lines_bg(l).text().c_str());

	  for_all_lines(l, lines_fg)
	    if (lines_fg(l).has_text())
	      stream << " " << codec->toUnicode(lines_fg(l).text().c_str())
		.remove(QRegExp("[\\?!()\\[\\]\\{\\}\\.,;\\\"\\\'`_]")); // Remove useless punctuation.

	  QStringList list = tmp_out.split(' ', QString::SkipEmptyParts);

	  output = QSet<QString>::fromList(list);
	}

	trace::exiting("scribo::toolchain::nepomuk::text_extraction");
	return output;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::toolchain::nepomuk

  } // end of namespace scribo::toolchain

} // end of namespace scribo


#endif // ! SCRIBO_TOOLCHAIN_NEPOMUK_TEXT_EXTRACTION_HH
