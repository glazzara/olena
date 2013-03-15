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

#ifndef SCRIBO_UPSAMPLING_BS2X_HH
# define SCRIBO_UPSAMPLING_BS2X_HH

/// \file
///
/// Scale 2x algorithm for binary images.

# include <mln/core/concept/image.hh>
# include <mln/core/alias/box2d.hh>
# include <mln/opt/at.hh>
# include <mln/geom/all.hh>



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
    bs2x(const Image<I>& input);



# ifndef MLN_INCLUDE_ONLY


    template <typename I>
    mln_concrete(I)
    bs2x(const mln::Image<I>& input_)
    {
      mln_trace("scribo::upsampling::bs2x");

      const I& input = exact(input_);

      mlc_is(mln_domain(I), mln::box2d)::check();
      mlc_is(mln_value(I), bool)::check();

      mln_precondition(input.is_valid());

      typedef mln_value(I) V;

      mln::def::coord
	mrow = geom::min_row(input),
	mcol = geom::min_col(input);


      mln_piter(I) p(input.domain());
      mln_concrete(I) output(mln::make::box2d(mrow, mcol,
					      mrow + 2 * input.nrows() - 1,
					      mcol + 2 * input.ncols() - 1));
      for_all(p)
      {
	mln::def::coord
	  row = mrow + 2 * (p.row() - mrow),
	  col = mcol + 2 * (p.col() - mcol);

	if(p.row() == geom::min_row(input)
	   || p.col() == geom::min_col(input)
	   || p.row() == geom::max_row(input)
	   || p.col() == geom::max_col(input))
	{
	  mln::box2d b = mln::make::box2d(row, col,
					  row + 1, col + 1);
	  V value = opt::at(input, p.row(), p.col());

	  data::fill((output | b).rw(), value);
	}
	else
	{
	  // nw  n  ne
	  //
	  // w value e
	  //
	  // sw  s  se

	  V n = input(p + mln::up),
	    s = input(p + mln::down),
	    e = input(p + mln::right),
	    w = input(p + mln::left),
	    nw = input(p + mln::up_left),
	    ne = input(p + mln::up_right),
	    sw = input(p + mln::down_left),
	    se = input(p + mln::down_right),
	    value = input(p);

	  if(e != w && n != s)
	  {
	    opt::at(output, row, col) = (w == n &&((se != nw) || !value)) ? w : value;
	    opt::at(output, row, col + 1) = (e == n &&((sw != ne) || !value)) ? e : value;
	    opt::at(output, row + 1, col) = (w == s &&((ne != sw) || !value)) ? w : value;
	    opt::at(output, row + 1, col + 1) = (e == s &&((nw != se) || !value)) ? e : value;
	  }
	  else
	  {
	    mln::box2d b = mln::make::box2d(row, col,
					    row + 1, col + 1);
	    data::fill((output | b).rw(), value);
	  }
	}
      }

      return output;
    }

# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace upsampling

} // end of namespace scribo


#endif // ! SCRIBO_UPSAMPLING_BS2X_HH
