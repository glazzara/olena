// Copyright (C) 2009 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef SCRIBO_TEXT_RECOGNITION_HH
# define SCRIBO_TEXT_RECOGNITION_HH

/// \file scribo/text/recognition.hh
///
/// Passes the text bounding boxes to an OCR (Tesseract).
///
/// \todo For each text bbox, we create a new image. We may like to avoid that.
/// \todo Do not store the result in an image?

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/site_set/box.hh>
# include <mln/util/array.hh>
# include <mln/labeling/blobs.hh>
# include <mln/data/fill.hh>
# include <mln/pw/all.hh>
# include <mln/debug/put_words.hh>

# include <scribo/core/macros.hh>
# include <scribo/util/text.hh>

# include <tesseract/baseapi.h>



namespace scribo
{

  namespace text
  {

    using namespace mln;

    /// Passes the text bboxes to Tesseract (OCR).
    ///
    /// \param[in] input_ Image from where the text is extracted.
    /// \param[in] text The lines of text.
    /// \param[in] language the language which should be recognized by Tesseract.
    ///		   (fra, en, ...)
    ///
    /// \return An image of characters.
    template <typename I, typename L>
    mln_ch_value(I,char)
    text_recognition(const Image<I>& input_,
		     const scribo::util::text<L>& text,
		     const char *language);


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename L>
    mln_ch_value(I,char)
    text_recognition(const Image<I>& input_,
		     const scribo::util::text<L>& text,
		     const char *language)
    {
      trace::entering("scribo::text::recognition");

      mlc_equal(mln_value(I), bool)::check();
      const I& input = exact(input_);
      mln_precondition(input.is_valid());
      mln_precondition(text.is_valid());

      // Initialize Tesseract.
      TessBaseAPI::InitWithLanguage(NULL, NULL, language, NULL, false, 0, NULL);

      mln_ch_value(I,char) txt(input.domain());
      data::fill(txt, ' ');

      /// Use text bboxes with Tesseract
      for_all_components(i, text.bboxes())
      {
	mln_ch_value(I,bool) b(text.bbox(i), 0);
	data::fill(b, false);
	data::fill((b | (pw::value(text.label_image()) == pw::cst(i))).rw(),
		   true);

	// Recognize characters.
	char* s = TessBaseAPI::TesseractRect(
	    (unsigned char*) b.buffer(),
	    sizeof (bool),		// Pixel size.
	    b.ncols() * sizeof (bool),  // Row_offset
	    0,				// Left
	    0,				// Top
	    b.ncols(),			// n cols
	    b.nrows());			// n rows



	mln_site(I) p = text.bbox(i).center();
	p.col() -= (text.bbox(i).pmax().col()
		      - text.bbox(i).pmin().col()) / 2;
	if (s != 0)
	  debug::put_word(txt, p, s);

	// The string has been allocated by Tesseract. We must free it.
	free(s);
      }

      trace::exiting("scribo::text::recognition");
      return txt;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::text

} // end of namespace scribo

#endif // ! SCRIBO_TEXT_RECOGNITION_HH
