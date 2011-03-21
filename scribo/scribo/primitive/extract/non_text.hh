// Copyright (C) 2011 EPITA Research and Development Laboratory
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

/// \file
///
/// \brief Find in a document non text which are not text.
///
/// \fixme To be optimized!

#ifndef SCRIBO_PRIMITIVE_EXTRACT_NON_TEXT_HH
# define SCRIBO_PRIMITIVE_EXTRACT_NON_TEXT_HH

# include <mln/morpho/elementary/dilation.hh>


# include <scribo/make/text_components_image.hh>
# include <scribo/make/text_blocks_image.hh>

# include <scribo/primitive/extract/internal/union.hh>
# include <scribo/debug/logger.hh>

//DEBUG
#include <mln/util/timer.hh>
#include <mln/io/pbm/save.hh>


namespace scribo
{

  namespace primitive
  {

    namespace extract
    {

      using namespace mln;

      /*! \brief Extract non text components.

	This method takes text localization into account and tries to
	learn the background colors to deduce the relevant non text
	components.

	\param[in] doc A document structure. Its must have paragraph
	information.

	\param[in] nlines The number of lines needed in a paragraph to
	consider the latter during the background color learning.


	\return A component set of non text components.
       */
      template <typename L>
      component_set<L>
      non_text(const document<L>& doc, unsigned nlines);


# ifndef MLN_INCLUDE_ONLY


      namespace internal
      {

	template <typename L>
	image2d<bool>
	learn(const document<L>& doc,
	      const image2d<bool>& txt,
	      const image2d<bool>& txtblocks,
	      unsigned nbits,
	      float p_cover)
	{
	  const image2d<value::rgb8>& input = doc.image();
	  const image2d<bool>&
	    seps = doc.paragraphs().lines().components().separators();

	  if (txt.border() != input.border()
	      || txtblocks.border() != input.border()
	      || seps.border() != input.border())
	  {
	    std::cerr << " txt.border() = " << txt.border()
		      << " - txtblocks.border() = " << txtblocks.border()
		      << " - input.border() = " << input.border()
		      << " - seps.border() = " << seps.border()
		      << std::endl;
	    std::cerr << "different sizes for borders! Resizing..." << std::endl;


	    border::resize(txt, border::thickness);
	    border::resize(input, border::thickness);
	    border::resize(txtblocks, border::thickness);
	    border::resize(seps, border::thickness);
	    // std::abort();
	  }


	  const unsigned q_div = std::pow(2.f, (int)(8 - nbits));
	  const unsigned q = unsigned(std::pow(2.f, (int)nbits));
	  const unsigned nelements = input.nelements();


	  image3d<unsigned> h_bg(q, q, q);
	  data::fill(h_bg, 0);

	  border::fill(txtblocks, false); // so h_bg is not updated by border pixels!

	  unsigned n_bg = 0;
	  {
	    // compute h_bg
	    for (unsigned i = 0; i < nelements; ++i)
	      if (txtblocks.element(i) == true)
	      {
		++n_bg;
		const value::rgb8& c = input.element(i);
		++h_bg.at_(c.red() / q_div, c.green() / q_div, c.blue() / q_div);
	      }
	  }

	  typedef std::map<unsigned, unsigned> map_t;
	  map_t ncells_with_nitems;
	  {
	    mln_piter_(box3d) c(h_bg.domain());
	    for_all(c)
	    {
	      unsigned nitems_in_c = h_bg(c);
	      ++ncells_with_nitems[ nitems_in_c ];
	    }
	  }


	  unsigned n_items_min = 0;
	  {
	    map_t::const_reverse_iterator i;
	    unsigned N = 0;
	    for (i = ncells_with_nitems.rbegin(); i != ncells_with_nitems.rend(); ++i)
	    {
	      unsigned nitems = i->first, ncells = i->second;
	      N += nitems * ncells;
	      if (float(N) > p_cover * float(n_bg))
	      {
		n_items_min = nitems;
		break;
	      }
	    }
	  }
	  if (n_items_min == 0)
	    n_items_min = 1;  // safety


	  image3d<bool> bg(q, q, q);
	  {
	    mln_piter_(box3d) c(h_bg.domain());
	    for_all(c)
	      bg(c) = (h_bg(c) >= n_items_min);
	  }


	  // outputing

	  image2d<bool> output;
	  initialize(output, input);
	  {
	    for (unsigned i = 0; i < nelements; ++i)
	      if (txt.element(i) == true || seps.element(i) == true)
		output.element(i) = false;
	      else
	      {
		const value::rgb8& c = input.element(i);
		output.element(i) = ! bg.at_(c.red() / q_div, c.green() / q_div, c.blue() / q_div);
	      }
	  }

	  return output;
	}





	inline
	image2d<bool>
	cleaning(const image2d<bool>& input, unsigned lambda)
	{
	  const box2d& dom = input.domain();

	  image2d<unsigned> area(dom);
	  image2d<unsigned> parent(dom);
	  image2d<bool> output(dom);

	  unsigned max_area = 0;


	  // 1st pass = bg union-find

	  {
	    union_find(input, false, // in
		       parent, area, max_area // out
	      );
	  }


	  // echo
	  // std::cout << "max_area = " << max_area << std::endl;


	  // 2nd pass = bg biggest component selection

	  {
	    const unsigned nelements = input.nelements();
	    const bool* p_i = input.buffer();
	    bool* p_o = output.buffer();
	    const unsigned* p_a = area.buffer();
	    const unsigned* p_par = parent.buffer();

	    for (unsigned i = 0; i < nelements; ++i)
	    {
	      if (*p_i == true)
		*p_o = true;
	      else
	      {
		if (*p_par == i)
		  *p_o = (*p_a != max_area);
		else
		  *p_o = output.element(*p_par);
	      }
	      ++p_i;
	      ++p_o;
	      ++p_a;
	      ++p_par;
	    }
	  }



	  // 3rd pass = fg union-find

	  {
	    union_find(output, true, // in
		       parent, area, max_area // out
	      );
	  }



	  // 4th pass = cleaning fg

	  {
	    const unsigned nelements = input.nelements();
	    bool* p_o = output.buffer();
	    const unsigned* p_a = area.buffer();
	    const unsigned* p_par = parent.buffer();

	    for (unsigned i = 0; i < nelements; ++i)
	    {
	      if (*p_o == true)
	      {
		if (*p_par == i)
		  *p_o = (*p_a > lambda);
		else
		  *p_o = output.element(*p_par);
	      }
	      ++p_o;
	      ++p_a;
	      ++p_par;
	    }
	  }


	  return output;
	}

      } // end of namespace scribo::primitive::extract::internal



      // FACADE

      template <typename L>
      component_set<L>
      non_text(const document<L>& doc, unsigned nlines)
      {
	trace::entering("scribo::primitive::extract::non_text");


	util::timer t;
	t.start();

	mln_precondition(doc.is_valid());

	mln_precondition(doc.has_line_seps());
	mln_precondition(doc.has_text());

	// FIXME: Do these images exist elsewhere?
	image2d<bool>
	  txt = make::text_components_image(doc),
	  txtblocks = make::text_blocks_image(doc, nlines);

	unsigned nbits = 5;
	float p = 0.9998; // 0.80 <= x < 1.0
	unsigned lambda = 1000;

	// enlarge the text mask so that "not txt" does not include
	// any text pixel
	txt = morpho::elementary::dilation(txt, c8());
	txt = morpho::elementary::dilation(txt, c4());

	// FIXME: Make it faster?
	data::fill((txtblocks | pw::value(txt)).rw(), false);

	// Debug
	{
	  debug::logger().log_image(debug::Special,//debug::AuxiliaryResults,
				    txt, "txt_components");
	  debug::logger().log_image(debug::Special,//debug::AuxiliaryResults,
				    txtblocks, "txt_blocks");
	}

	image2d<bool>
	  element_image = internal::learn(doc, txt, txtblocks, nbits, p);
	element_image = internal::cleaning(element_image, lambda);

	mln_value(L) ncomps;
	component_set<L>
	  elements = primitive::extract::components(element_image,
						    c8(), ncomps);

	// Debug
	{
	  debug::logger().log_image(debug::Results,
				    elements.labeled_image(),
				    "non_text_components");
	}

	trace::exiting("scribo::primitive::extract::non_text");
	return elements;
      }

# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace scribo::primitive::extract

  } // end of namespace scribo::primitive

} // end of namespace scribo

#endif // ! SCRIBO_PRIMITIVE_EXTRACT_NON_TEXT_HH
