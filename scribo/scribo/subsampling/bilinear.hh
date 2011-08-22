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

#ifndef SCRIBO_SUBSAMPLING_BILINEAR_HH
# define SCRIBO_SUBSAMPLING_BILINEAR_HH

/// \file
///
/// Bilinear subsampling.

# include <mln/core/concept/image.hh>
# include <mln/core/alias/box2d.hh>
# include <mln/geom/max_col.hh>
# include <mln/geom/max_row.hh>

# include <mln/opt/at.hh>


namespace scribo
{

  namespace subsampling
  {

    using namespace mln;

    template <typename I>
    mln_concrete(I)
    bilinear(const Image<I>& input_, int sub_ratio);


# ifndef MLN_INCLUDE_ONLY


    template <typename I>
    mln_concrete(I)
    bilinear(const Image<I>& input_, int sub_ratio)
    {
      trace::entering("scribo::subsampling::bilinear");

      const I& input = exact(input_);
      mln_precondition(input.is_valid());
      mlc_is(mln_domain(I), box2d)::check();

      mln_concrete(I) output(input.domain().nrows() / sub_ratio,
			     input.domain().ncols() / sub_ratio);

      mln_piter(I) p(output.domain());
      mln_value(I) pixels[4];

      mln::def::coord
	x_offset = input.domain().pmin().col(),
	y_offset = input.domain().pmin().row();

      for_all(p)
      {
	int x = int(round(-0.5 + p.col() * sub_ratio));
	int y = int(round(-0.5 + p.row() * sub_ratio));

	if (x < 0)
	  x = 0;
	else if (x >= geom::max_col(input))
	  x = input.ncols() - 1;

	if (y < 0)
	  y = 0;
	else if (y >= geom::max_row(input))
	  y = input.nrows() - 1;

	double dx = (p.col() * sub_ratio) - x;
	double dy = (p.row() * sub_ratio) - y;

	pixels[0] = opt::at(input, y + y_offset, x + x_offset);
	pixels[1] = opt::at(input, y + y_offset, x + x_offset + 1);
	pixels[2] = opt::at(input, y + y_offset + 1, x + x_offset);
	pixels[3] = opt::at(input, y + y_offset + 1, x + x_offset + 1);

	output(p) = pixels[0] * (1 - dx) * (1 - dy) + pixels[1] * dx * (1 - dy) +
	  pixels[2] * (1 - dx) * dy + pixels[3] * dx * dy;

      }

      trace::exiting("scribo::subsampling::bilinear");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace subsampling

} // end of namespace scribo


#endif // SCRIBO_SUBSAMPLING_BILINEAR_HH
