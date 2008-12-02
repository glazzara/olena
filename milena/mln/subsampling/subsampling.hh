// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_SUBSAMPLING_SUBSAMPLING_HH
# define MLN_SUBSAMPLING_SUBSAMPLING_HH

/// \file mln/binarization/threshold.hh
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
                   const mln_coord(image2d<T>)& gap)
      {
	trace::entering("subsampling::impl::subsampling_");
        mln_concrete(image2d<T>) output(geom::nrows(input) / gap,
                               geom::ncols(input) / gap);

        for (unsigned j = 0; j < geom::ncols(output); ++j)
          for (unsigned i = 0; i < geom::nrows(output); ++i)
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
      mln_precondition(exact(input).has_data());

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
