// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory
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

#ifndef MLN_SUBSAMPLING_SUBSAMPLING_HH
# define MLN_SUBSAMPLING_SUBSAMPLING_HH

/// \file
///
/// Produce a subsampled image

# include <mln/geom/ncols.hh>
# include <mln/geom/nrows.hh>


namespace mln
{

  namespace subsampling
  {

    /// Subsampling FIXME : doxy
    template <typename I>
    inline
    mln_concrete(I)
    subsampling(const Image<I>& input,
                const mln_deduce(I, site, delta)& first_p,
                const mln_deduce(I, site, coord)& gap);

# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      //FIXME : add version for every image types.

      template <typename T>
      inline
      mln_concrete(image2d<T>)
      subsampling_(const image2d<T>& input,
                   const mln_deduce(image2d<T>, site, delta)& first_p,
                   const mln_deduce(image2d<T>, site, coord)& gap)
      {
	trace::entering("subsampling::impl::subsampling_");
        mln_concrete(image2d<T>) output(geom::nrows(input) / gap,
                               geom::ncols(input) / gap);

        for (def::coord j = geom::min_col(output);
	     j <= geom::max_col(output); ++j)
          for (def::coord i = geom::min_row(output);
	       i <= geom::max_row(output); ++i)
            {
              point2d p1(i, j);
              point2d p2(first_p[0] + i * gap, first_p[1] + j * gap);

              output(p1) = input(p2);
            }

	trace::exiting("subsampling::impl::subsampling_");
	return output;
      }

    } // end of namespace mln::subsampling::impl


    template <typename I>
    inline
    mln_concrete(I)
    subsampling(const Image<I>& input,
                const mln_deduce(I, site, delta)& first_p,
                const mln_deduce(I, site, coord)& gap)
    {
      trace::entering("subsampling::subsampling");
      mln_precondition(exact(input).is_valid());

      mln_concrete(I) output(geom::nrows(input) / gap,
                             geom::ncols(input) / gap); // FIXME: only 2d here

      output = impl::subsampling_(exact(input), first_p, gap);

      trace::exiting("subsampling::subsampling");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::subsampling

} // end of namespace mln


#endif // ! MLN_SUBSAMPLING_SUBSAMPLING_HH
