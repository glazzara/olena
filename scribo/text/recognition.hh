// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_TEXT_RECOGNITION_HH
# define SCRIBO_TEXT_RECOGNITION_HH

/// \file
///
/// Passes the text bounding boxes to an OCR (Tesseract).
///
/// \todo For each text bbox, we create a new image. We may like to avoid that.
/// \todo Do not store the result in an image?

# include <ostream>

# include <mln/core/image/dmorph/image_if.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/site_set/box.hh>
# include <mln/util/array.hh>
# include <mln/labeling/blobs.hh>
# include <mln/data/fill.hh>
# include <mln/pw/all.hh>

# include <mln/transform/distance_front.hh>

# include <mln/morpho/thickening.hh>
# include <mln/morpho/dilation.hh>

# include <mln/core/alias/w_window2d_int.hh>
# include <mln/make/w_window2d_int.hh>

# include <mln/border/resize.hh>

# include <mln/win/rectangle2d.hh>
# include <mln/win/disk2d.hh>
# include <mln/win/octagon2d.hh>

# include <mln/debug/put_word.hh>

# include <scribo/core/macros.hh>

# include <scribo/util/text.hh>

# include <scribo/text/clean.hh>

# include <tesseract/baseapi.h>


#include <mln/labeling/colorize.hh>

namespace scribo
{

  namespace text
  {

    using namespace mln;

    /// Passes the text bboxes to Tesseract (OCR).
    ///
    /// \param[in] text        The lines of text.
    /// \param[in] language    The language which should be recognized by
    ///		               Tesseract. (fra, en, ...)
    /// \param[in] output_file If set, store the recognized text in
    ///                        this file.
    //
    template <typename L>
    void
    recognition(const object_image(L)& objects,
		const char *language,
		const char *output_file);



# ifndef MLN_INCLUDE_ONLY


    template <typename L>
    void
    recognition(const object_image(L)& objects,
		const char *language,
		const char *output_file)
    {
      trace::entering("scribo::text::recognition");

      mln_precondition(objects.is_valid());

      // Initialize Tesseract.
      TessBaseAPI::InitWithLanguage(NULL, NULL, language, NULL, false, 0, NULL);

      typedef mln_ch_value(L,bool) I;
      int vals[] = { 0, 9, 0, 9, 0,
		     9, 6, 4, 6, 9,
		     0, 4, 0, 4, 0,
		     9, 6, 4, 6, 9,
		     0, 9, 0, 9, 0 };
      w_window2d_int dmap_win = mln::make::w_window2d_int(vals);

      std::ofstream file;
      if (output_file != 0)
	file.open(output_file);

      /// Use text bboxes with Tesseract
      for_all_ncomponents(i, objects.nlabels())
      {
	std::cout << "Text recognition... ("
		  << i << "/" << objects.nlabels() << ")" << std::endl;

	mln_domain(I) box = objects.bbox(i);
	// Make sure characters are isolated from the borders.
	// Help Tesseract.
	box.enlarge(2);

	I text_ima(box);
	data::fill(text_ima, true);

	// Careful : background is set to 'False'
	data::fill((text_ima | (pw::value(objects) == pw::cst(i))).rw(),
		   false);

	/// Improve text quality.

	/// text_ima_cleand domain is larger than text_ima's.
	I text_ima_cleaned = text::clean(text_ima, dmap_win);

        // Setting objects to 'True'
	logical::not_inplace(text_ima_cleaned);

	// Make sure there is no border.
	border::resize(text_ima_cleaned, 0);

	// Recognize characters.
	char* s = TessBaseAPI::TesseractRect(
	    (unsigned char*) text_ima_cleaned.buffer(),
	    sizeof (bool),			      // Pixel size.
	    text_ima_cleaned.ncols() * sizeof (bool), // Row_offset
	    0,					      // Left
	    0,					      // Top
	    text_ima_cleaned.ncols(),		      // n cols
	    text_ima_cleaned.nrows());		      // n rows



	mln_site(L) p = objects.bbox(i).center();
	p.col() -= (objects.bbox(i).pmax().col()
		      - objects.bbox(i).pmin().col()) / 2;
	if (s != 0)
	{
	  std::cerr << s << std::endl;
	  if (output_file != 0)
	    file << s << std::endl;
	}

	// The string has been allocated by Tesseract. We must free it.
	free(s);
      }

      if (output_file != 0)
	file.close();

      trace::exiting("scribo::text::recognition");
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::text

} // end of namespace scribo

#endif // ! SCRIBO_TEXT_RECOGNITION_HH
