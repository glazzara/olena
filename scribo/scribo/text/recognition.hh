// Copyright (C) 2009, 2010, 2011 EPITA Research and Development
// Laboratory (LRDE)
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
# include <mln/data/fill.hh>
# include <mln/data/paste.hh>
# include <mln/data/paste_without_localization.hh>
# include <mln/pw/all.hh>

# include <mln/core/alias/w_window2d_int.hh>
# include <mln/make/w_window2d_int.hh>

# include <mln/border/resize.hh>

# include <scribo/core/macros.hh>

# include <scribo/text/clean_inplace.hh>

# include <scribo/core/line_set.hh>


# include <tesseract/baseapi.h>


# if !defined HAVE_TESSERACT_2 && !defined HAVE_TESSERACT_3
#  define HAVE_TESSERACT_2
# endif



namespace scribo
{

  namespace text
  {

    using namespace mln;

    /// Passes the text bboxes to Tesseract (OCR).
    ///
    /// \param[in] lines       The lines of text.
    /// \param[in] language    The language which should be recognized by
    ///		               Tesseract. (fra, en, ...)
    /// \param[in] output_file If set, store the recognized text in
    ///                        this file.
    //
    template <typename L>
    void
    recognition(line_set<L>& lines, const char *language);


    /// Recognize text from an image.
    template <typename I>
    void
    recognition(const Image<I>& line,
		const char *language,
		const std::string& output_file = std::string());



# ifndef MLN_INCLUDE_ONLY


    template <typename L>
    void
    recognition(line_set<L>& lines, const char *language)
    {
      trace::entering("scribo::text::recognition");


      // Initialize Tesseract.
#  ifdef HAVE_TESSERACT_2
      TessBaseAPI::InitWithLanguage(NULL, NULL, language, NULL, false, 0, NULL);
#  else // HAVE_TESSERACT_3
      tesseract::TessBaseAPI tess;
      if (tess.Init(NULL, language, tesseract::OEM_DEFAULT) == -1)
      {
	std::cout << "Error: cannot initialize tesseract!" << std::endl;
	abort();
      }
      tess.SetPageSegMode(tesseract::PSM_SINGLE_LINE);

#  endif // HAVE_TESSERACT_2

      typedef mln_ch_value(L,bool) I;


      /// Use text bboxes with Tesseract
      for_all_lines(i, lines)
      {
	if (! lines(i).is_textline())
	  continue;

	mln_domain(I) box = lines(i).bbox();

	// Make sure characters are isolated from the borders.
	// Help Tesseract.
	box.enlarge(2);

	I text_ima(box);
	data::fill(text_ima, false);

	// Careful : background is set to 'False'
	const component_set<L>& comp_set = lines.components();
	const L& lbl = comp_set.labeled_image();

	// Extract each character components to create the line image.
	const mln::util::array<component_id_t>& comps = lines(i).component_ids();
	for_all_elements(e, lines(i).component_ids())
	{
	  unsigned comp_id = comps(e);
	  data::fill(((text_ima | comp_set(comp_id).bbox()).rw() | (pw::value(lbl) == pw::cst(comp_id))).rw(),
		     true);
	}

	/// Improve text quality.
	text::clean_inplace(lines(i), text_ima);

	// Make sure characters are isolated from the borders.
	// Help Tesseract.
	//
	// FIXME: can be improved! We need a morpher for a constant
	// extension set to false (avoid data::fill), a morpher for
	// translating the domain to (0,0) (avoid the creation of a
	// new image), change the default border::thickness to 0 and a
	// morpher to enlarge the domain to a part of the extension.
	mln_domain(I) lbox = text_ima.domain();
	lbox.enlarge(lines(i).char_space() + 2);
	I line_image(lbox, 0); // Make sure there is no border!
	data::fill(line_image, false);
	data::paste_without_localization(text_ima, line_image);

	// Recognize characters.
#  ifdef HAVE_TESSERACT_2
	char* s = TessBaseAPI::TesseractRect(
	    (unsigned char*) line_image.buffer(),
	    sizeof (bool),			 // Pixel size.
	    line_image.ncols() * sizeof (bool),  // Row_offset
	    0,					 // Left
	    0,					 // Top
	    line_image.ncols(),		         // n cols
	    line_image.nrows());		 // n rows
#  else // HAVE_TESSERACT_3
	tess.SetImage(
	  (unsigned char*) line_image.buffer(),
	  line_image.ncols(),		         // n cols
	  line_image.nrows(),		         // n rows
	  sizeof (bool),			 // Pixel size.
	  line_image.ncols() * sizeof (bool));    // Row_offset
	char* s = tess.GetUTF8Text();
#  endif // ! HAVE_TESSERACT_2

	if (s != 0)
	{
	  std::string str(s);
	  str = str.substr(0, str.length() - 2);
	  lines(i).update_text(str);
	}

	// The string has been allocated by Tesseract. It must be released.
	delete [] s;
      }

      trace::exiting("scribo::text::recognition");
    }


    template <typename I>
    void
    recognition(const Image<I>& line_,
		const char *language,
		const std::string& output_file = std::string())
    {
      trace::entering("scribo::text::recognition");

      const I& line = exact(line_);
      mln_precondition(line.is_valid());

      // Initialize Tesseract.
#  ifdef HAVE_TESSERACT_2
      TessBaseAPI::InitWithLanguage(NULL, NULL, language, NULL, false, 0, NULL);
#  else // HAVE_TESSERACT_3
      tesseract::TessBaseAPI tess;
      if (tess.Init(NULL, language, tesseract::OEM_DEFAULT) == -1)
      {
	std::cout << "Error: cannot initialize tesseract!" << std::endl;
	abort();
      }
#  endif // ! HAVE_TESSERACT_2

      std::ofstream file;
      if (!output_file.empty())
	file.open(output_file.c_str());

      mln_domain(I) box = line.domain();
      // Make sure characters are isolated from the borders.
      // Help Tesseract.
      box.enlarge(2);

      I text_ima(box);
      data::fill(text_ima, false);
      data::paste(line, text_ima);

      // Make sure there is no border.
      border::resize(text_ima, 0);

      // Recognize characters.
#  ifdef HAVE_TESSERACT_2
      char* s = TessBaseAPI::TesseractRect(
	(unsigned char*) text_ima.buffer(),
	sizeof (bool),			  // Pixel size.
	text_ima.ncols() * sizeof (bool), // Row_offset
	0,				  // Left
	0,				  // Top
	text_ima.ncols(),		  // n cols
	text_ima.nrows());		  // n rows
#  else // HAVE_TESSERACT_3
      char* s = tess.TesseractRect(
	(unsigned char*) text_ima.buffer(),
	sizeof (bool),			  // Pixel size.
	text_ima.ncols() * sizeof (bool), // Row_offset
	0,				  // Left
	0,				  // Top
	text_ima.ncols(),		  // n cols
	text_ima.nrows());		  // n rows
#  endif // ! HAVE_TESSERACT_2

	if (s != 0)
	{
	  if (!output_file.empty())
	  {
	    std::string str(s);
	    str = str.substr(0, str.length() - 1);
	    file << line.domain().bbox().pmin().row()
		 << " "
		 << line.domain().bbox().pmin().col()
		 << " "
		 << line.domain().bbox().pmax().row()
		 << " "
		 << line.domain().bbox().pmax().col()
		 << " "
		 << str;
	  }
	}

	// The string has been allocated by Tesseract. We must free it.
	delete [] s;

	if (!output_file.empty())
	  file.close();

	trace::exiting("scribo::text::recognition");
    }



# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::text

} // end of namespace scribo

#endif // ! SCRIBO_TEXT_RECOGNITION_HH
