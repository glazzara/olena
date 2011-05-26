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

namespace scribo
{

  namespace text
  {

    using namespace mln;

    template< typename L, typename V >
    void
    paragraphs_closing(image2d<V>& output,
		       const paragraph_set<L>& par_set,
		       const box2d& domain);

# ifndef MLN_INCLUDE_ONLY

    template< typename V >
    inline
    void horizontal_CRLA(const image2d<V>& input,
			 image2d<V>& output,
			 const mln::util::array<int>& deltas)
    {
      mln_piter(image2d<V>) p(input.domain());
      int count = 0;
      unsigned nrows = input.nrows();
      unsigned ncols = input.ncols();
      V last_pixel_value = 0;

      for (unsigned i = 0; i < nrows; ++i)
      {
	for (unsigned j = 0; j < ncols; ++j)
	{
	  const V& current_pixel = input.at_(i, j);

	  if (!current_pixel)
	  {
	    if (last_pixel_value)
	    {
	      unsigned k = j + 1;
	      for (; !input.at_(i, k) && (k < ncols); ++k);

	      count = k - j;
	      const int threshold = deltas(last_pixel_value - 1);

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

    template< typename V >
    inline
    void vertical_CRLA(const image2d<V>& input,
		       image2d<V>& output,
		       const mln::util::array<int>& deltas)
    {
      mln_piter(image2d<V>) p(input.domain());
      int count = 0;
      unsigned nrows = input.nrows();
      unsigned ncols = input.ncols();
      V last_pixel_value = 0;

      for (unsigned j = 0; j < ncols; ++j)
      {
	for (unsigned i = 0; i < nrows; ++i)
	{
	  const V& current_pixel = input.at_(i, j);

	  if (!current_pixel)
	  {
	    if (last_pixel_value)
	    {
	      unsigned k = i + 1;
	      for (; !input.at_(k, j) && (k < nrows); ++k);

	      count = k - i;
	      const int threshold = deltas(last_pixel_value - 1);

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

    template< typename V >
    inline
    void CRLA(const image2d<V>& input,
	      image2d<V>& output,
	      const mln::util::array<int>& deltas,
	      const mln::util::array<int>& deltas_factor)
    {
      horizontal_CRLA(input, output, deltas_factor);
      vertical_CRLA(output, output, deltas);
      horizontal_CRLA(output, output, deltas_factor);
    }

    template< typename L, typename V >
    void
    paragraphs_closing(image2d<V>& output,
		       const paragraph_set<L>& par_set,
		       const box2d& domain)
    {
      trace::entering("scribo::text::paragraphs_closing");

      image2d<V> debug(domain);

      mln::util::array<int> deltas;
      deltas.reserve(par_set.nelements());
      mln::util::array<int> deltas_factor;
      deltas_factor.reserve(par_set.nelements());

      data::fill(debug, 0);
      data::fill(output, 0);

      const line_set<L>& lines = par_set.lines();

      for_all_paragraphs(p, par_set)
      {
	const paragraph_info<L>& current_par = par_set(p);
	const mln::util::array<line_id_t>& line_ids = current_par.line_ids();
	const unsigned nelements = line_ids.nelements();

	for (unsigned i = 0; i < nelements; ++i)
	{
	  const line_id_t& line_id = line_ids(i);
	  const line_info<L>& current_line = lines(line_id);

	  draw::line_components(debug, current_line, p);
	}

	int delta_baseline = current_par.delta_baseline();

	if (delta_baseline % 2 == 0)
	    --delta_baseline;

	deltas.append(delta_baseline);
	deltas_factor.append(3 * delta_baseline);
      }
      CRLA(debug, output, deltas, deltas_factor);

      trace::exiting("scribo::draw::line_components");
    }

# endif

  } // end of namespace scribo::text

} // end of namespace scribo


#endif // ! SCRIBO_TEXT_PARAGRAPHS_CLOSING_HH

