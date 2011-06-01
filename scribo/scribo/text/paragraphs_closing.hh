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

#ifndef SCRIBO_TEXT_PARAGRAPHS_CLOSING_HH
# define SCRIBO_TEXT_PARAGRAPHS_CLOSING_HH

/// \file
///
/// Paragraphs closing using CRLA.

# include <mln/draw/line.hh>
# include <scribo/draw/line_components.hh>

namespace scribo
{

  namespace text
  {

    using namespace mln;

    template< typename L>
    mln_concrete(L)
    paragraphs_closing(const paragraph_set<L>& parset);


# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {

      template<typename L>
      inline
      void horizontal_CRLA(const Image<L>& input_,
			   Image<L>& output_,
			   const mln::util::array<int>& deltas)
      {
	const L& input = exact(input_);
	L& output = exact(output_);
	mln_precondition(input.is_valid());
	mln_precondition(output.is_valid());

	mln_piter(L) p(input.domain());
	int count = 0;
	unsigned nrows = input.nrows();
	unsigned ncols = input.ncols();
	mln_value(L) last_pixel_value = 0;

	for (unsigned i = 0; i < nrows; ++i)
	{
	  for (unsigned j = 0; j < ncols; ++j)
	  {
	    const mln_value(L)& current_pixel = input.at_(i, j);

	    if (!current_pixel)
	    {
	      if (last_pixel_value)
	      {
		unsigned k = j + 1;
		for (; !(input.at_(i, k)) && (k < ncols); ++k);

		count = k - j;
		const int threshold = deltas(last_pixel_value);

		if (last_pixel_value == input.at_(i, k) && count < threshold)
		  for (unsigned l = j; l <= k; ++l)
		    output.at_(i, l) = last_pixel_value;

		j = k;
		last_pixel_value = 0;
	      }
	    }
	    else
	    {
	      output.at_(i, j) = current_pixel;
	      last_pixel_value = current_pixel;
	    }
	  }
	}
      }

      template<typename L>
      inline
      void vertical_CRLA(const Image<L>& input_,
			 Image<L>& output_,
			 const mln::util::array<int>& deltas)
      {
	const L& input = exact(input_);
	L& output = exact(output_);
	mln_precondition(input.is_valid());
	mln_precondition(output.is_valid());

	mln_piter(L) p(input.domain());
	int count = 0;
	unsigned nrows = input.nrows();
	unsigned ncols = input.ncols();
	mln_value(L) last_pixel_value = 0;

	for (unsigned j = 0; j < ncols; ++j)
	{
	  for (unsigned i = 0; i < nrows; ++i)
	  {
	    const mln_value(L)& current_pixel = input.at_(i, j);

	    if (!current_pixel)
	    {
	      if (last_pixel_value)
	      {
		unsigned k = i + 1;
		for (; !(input.at_(k, j)) && (k < nrows); ++k);

		count = k - i;
		const int threshold = deltas(last_pixel_value);

		if (last_pixel_value == input.at_(k, j)
		    && count < threshold)
		  for (unsigned l = i; l <= k; ++l)
		    output.at_(l, j) = last_pixel_value;

		i = k;
		last_pixel_value = 0;
	      }
	    }
	    else
	    {
	      output.at_(i, j) = current_pixel;
	      last_pixel_value = current_pixel;
	    }
	  }
	}
      }

      template<typename L>
      inline
      void CRLA(const Image<L>& input,
		Image<L>& output,
		const mln::util::array<int>& deltas,
		const mln::util::array<int>& deltas_factor)
      {
	horizontal_CRLA(input, output, deltas_factor);

	debug::logger().log_image(debug::AuxiliaryResults,
				  output,
				  "paragraph_closing_horizontal_CRLA");


	vertical_CRLA(output, output, deltas);

	debug::logger().log_image(debug::AuxiliaryResults,
				  output,
				  "paragraph_closing_vertical_CRLA");

	horizontal_CRLA(output, output, deltas_factor);
      }

    } // end of namespace scribo::text::internal


    template<typename L>
    mln_concrete(L)
    paragraphs_closing(const paragraph_set<L>& parset)
    {
      trace::entering("scribo::text::paragraphs_closing");

      // FIXME: 'debug' may be useless.
      mln_concrete(L) output, debug;
      initialize(output, parset.lines().components().labeled_image());
      initialize(debug, output);

      mln::util::array<int> deltas(parset.nelements() + 1, 0);
      mln::util::array<int> deltas_factor(parset.nelements() + 1, 0);

      data::fill(debug, 0);
      data::fill(output, 0);

      const line_set<L>& lines = parset.lines();

      for_all_paragraphs(p, parset)
	if (parset(p).is_valid())
	{
	  const paragraph_info<L>& current_par = parset(p);
	  const mln::util::array<line_id_t>& line_ids = current_par.line_ids();

	  line_id_t last_id = line_ids[0];
	  for_all_elements(i, line_ids)
	  {
	    const line_id_t& line_id = line_ids(i);
	    const line_info<L>& current_line = lines(line_id);

	    scribo::draw::line_components(debug, lines, current_line, p);

	    // HACK DISCLAIMER : this line is drawn in order to be
	    // sure that every line will be reduced to a single
	    // component after closing.  It is necessary to reduce a
	    // paragraph to one component in order to extract its
	    // outline correctly for xml/debug output.
	    component_id_t last_comp = lines(line_id).component_ids()(0);
	    for_all_elements(i, lines(line_id).component_ids())
	    {
	      const unsigned c = lines(line_id).component_ids()(i);
	      mln::draw::line(debug,
	    		      lines.components()(c).mass_center(),
	    		      lines.components()(last_comp).mass_center(),
	    		      p);
	      last_comp = c;
	    }

	    // mln::draw::line(debug, current_line.bbox().pcenter(), lines(last_id).bbox().pcenter(), p);
	    // last_id = line_id;
	  }

	  int delta_baseline = current_par.delta_baseline();

	  if (delta_baseline % 2 == 0)
	    --delta_baseline;

	  deltas(p) = 2 * delta_baseline; // Vertical
	  deltas_factor(p) = 3 * delta_baseline; // Horizontal
	}

      debug::logger().log_image(debug::AuxiliaryResults,
				debug,
				"paragraph_closing_input_CRLA");

      internal::CRLA(debug, output, deltas, deltas_factor);

      debug::logger().log_image(debug::Results,
				output,
				"paragraph_closing");

      trace::exiting("scribo::text::paragraphs_closing");
      return output;
    }

# endif

  } // end of namespace scribo::text

} // end of namespace scribo


#endif // ! SCRIBO_TEXT_PARAGRAPHS_CLOSING_HH

