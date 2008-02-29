// Copyright (C) 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_SUBSAMPLING_SUBSAMPLING_2D_HH
# define MLN_SUBSAMPLING_SUBSAMPLING_2D_HH

/*! \file mln/binarization/threshold.hh
 *
 * \brief Threshold the contents of an image into another binary one.
 */

# include <mln/geom/ncols.hh>
# include <mln/geom/nrows.hh>


namespace mln
{

  namespace subsampling
  {    

    /*! Subsampling FIXME : doxy
     *
     *
     */
    template <typename I>
    inline
    mln_concrete(I)
    subsampling_2d(const Image<I>& input,
                   const mln_dpoint(I)& first_p,
                   const mln_coord(I)& gap);

# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I>
      inline
      mln_concrete(I)
      subsampling_2d_(const I& input,
                      const mln_dpoint(I)& first_p,
                      const mln_coord(I)& gap)
      {
	trace::entering("subsampling_2d::impl::subsampling_2d_");
        mln_concrete(I) output(geom::nrows(input) / gap,
                               geom::ncols(input) / gap);

        for (unsigned j = 0; j < geom::ncols(output); ++j)
          for (unsigned i = 0; i < geom::nrows(output); ++i)
            {
              point2d p1(i, j);
              point2d p2(first_p[0] + i * gap, first_p[1] + j * gap);
              std::cout << p1 << ' ' << p2 << std::endl;
              output(p1) = input(p2);
            }

	trace::exiting("subsampling_2d::impl::subsampling_2d_");
	return output;
      }

    } // end of namespace mln::subsampling_2d::impl


    template <typename I>
    inline
    mln_concrete(I)
    subsampling_2d(const Image<I>& input,
                   const mln_dpoint(I)& first_p,
                   const mln_coord(I)& gap)
    {
      trace::entering("subsampling::subsampling_2d");
      mln_precondition(exact(input).has_data());

      mln_concrete(I) output(geom::nrows(input) / gap,
                             geom::ncols(input) / gap);

      std::cout << geom::nrows(output) << ' ' << geom::ncols(output) << std::endl;
      
      output = impl::subsampling_2d_(exact(input), first_p, gap);

      trace::exiting("subsampling::subsampling_2d");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::subsampling_2d

} // end of namespace mln


#endif // ! MLN_SUBSAMPLING_SUBSAMPLING_2D_HH
