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
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef SCRIBO_UPSAMPLING_EAGLE_HH_
# define SCRIBO_UPSAMPLING_EAGLE_HH_

/// \file
///
/// Scale 2x algorithm for binary images.

# include <mln/core/concept/image.hh>
# include <mln/core/def/coord.hh>
# include <mln/geom/min_row.hh>
# include <mln/geom/min_col.hh>
# include <mln/geom/max_row.hh>
# include <mln/geom/max_col.hh>
# include <mln/opt/at.hh>
# include <mln/make/box2d.hh>

namespace scribo
{

  namespace upsampling
  {

    using namespace mln;

    /// \brief Scale 2x algorithm for binary images.
    ///
    //
    template <typename I>
    mln_concrete(I)
    eagle(const Image<I>& ima);



# ifndef MLN_INCLUDE_ONLY


    template <typename I>
    mln_concrete(I)
    eagle(const Image<I>& input_)
    {
      mln_trace("scribo::upsampling::eagle");

      typedef mln_value(I) V;
      const I& input = exact(input_);

      mln_precondition(input.is_valid());

      mln::def::coord
	mrow = geom::min_row(input),
	mcol = geom::min_col(input),
	mxrow = geom::max_row(input),
	mxcol = geom::max_col(input);


      mln_piter(I) p(input.domain());
      mln_concrete(I)
	output(mln::make::box2d(mrow, mcol,
				mrow + 2 * input.nrows() - 1,
				mcol + 2 * input.ncols() - 1));

      for_all(p)
      {
	mln::def::coord
	  prow = p.row() - mrow,
	  pcol = p.col() - mcol;

	if (p.row() == mrow
	    || p.col() == mcol
	    || p.row() == mxrow
	    || p.col() == mxcol)
	{
	  box2d	b = mln::make::box2d(2 * prow, 2 * pcol,
				     2 * prow + 1, 2 * pcol + 1);
	  mln_piter_(box2d) pb(b);
	  V value = opt::at(input, p.row(), p.col());

	  for_all(pb)
	    opt::at(output, mrow + pb.row(), mcol + pb.col()) = value;
	}
	else
	{
	  V
	    n = opt::at(input, p.row() - 1, p.col()),
	    s = opt::at(input, p.row() + 1, p.col()),
	    e = opt::at(input, p.row(), p.col() + 1),
	    w = opt::at(input, p.row(), p.col() - 1),
	    nw = opt::at(input, p.row() - 1, p.col() - 1),
	    ne = opt::at(input, p.row() - 1, p.col() + 1),
	    sw = opt::at(input, p.row() + 1, p.col() - 1),
	    se = opt::at(input, p.row() + 1, p.col() + 1),
	    value = opt::at(input, p.row(), p.col());

	  if (e != w || n != s || e != n)
	  {
	    opt::at(output, mrow + 2 * prow, mcol + 2 * pcol) = ((w == n && w == nw && nw == n) ? nw : value);
	    opt::at(output, mrow + 2 * prow, mcol + 2 * pcol + 1) = ((e == n && e == ne && ne == n) ? ne : value);
	    opt::at(output, mrow + 2 * prow + 1, mcol + 2 * pcol) = ((w == s && w == sw && sw == s) ? sw : value);
	    opt::at(output, mrow + 2 * prow + 1, mcol + 2 * pcol + 1) = ((e == s && e == se && se == s) ? se : value);
	  }
	  else
	  {
	    box2d b = mln::make::box2d(2 * prow, 2 * pcol,
				       2 * prow + 1, 2 * pcol + 1);
	    mln_piter_(box2d) pb(b);
	    V value = opt::at(input, p.row(), p.col());

	    for_all(pb)
	      opt::at(output, mrow + pb.row(), mcol + pb.col()) = value;
	  }
	}
      }

      return output;
    }

# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace upsampling

} // end of namespace scribo

#endif // ! SCRIBO_UPSAMPLING_EAGLE_HH_
