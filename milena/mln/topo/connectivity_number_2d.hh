// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_TOPO_CONNECTIVITY_NUMBER_2D_HH
# define MLN_TOPO_CONNECTIVITY_NUMBER_2D_HH

/// \file
/// \brief 2D connectivity numbers computation.

# include <mln/core/concept/image.hh>

# include <mln/core/alias/neighb2d.hh>

# include <mln/value/int_u8.hh>

// FIXME: This file includes the routine mln::connectivity_number_2d,
// which should be moved into its own file.
# include <mln/topo/is_simple_2d.hh>


// FIXME: This implementation used the LUT-based (mask-based)
// computation of 2D connectivity numbers, which may is fast but may
// be inexact on the border of an image.  We shoud rename these
// routines with a `_fast' suffix.

namespace mln
{

  namespace topo
  {

    /** Type of a connectivity number.

	Note: The maximun number of connected components in the
	neighborhood of a point is necessarily lower or equal to half
	the number of neighbors.  This number fits on an
	mln::value::int_u8 when the dimension is lower or equal to
	5.  */
    typedef mln::value::int_u8 conn_number_t;


    /** Compute the 2D connectivity number of \a p within \a ima
        according to the 4-connectivity.  If \a ref_val is set to \c
        true, compute the number w.r.t. the foreground, otherwise
        compute it w.r.t. the background.

        \param ima      The image on which the connectivity number is
                        computed.
        \param p        The site the connectivity number of which is
                        computed.
        \param ref_val  The reference value, telling whether the
                        foreground (true) or the background (false) is
                        considered.  */
    template<typename I>
    conn_number_t
    connectivity_number_2d_c4(const Image<I>& ima, const mln_psite(I)& p,
			      bool ref_val);

    /** Compute the 2D connectivity number of \a p within \a ima
        according to the 8-connectivity.  If \a ref_val is set to \c
        true, compute the number w.r.t. the foreground, otherwise
        compute it w.r.t. the background.

        \param ima      The image on which the connectivity number is
                        computed.
        \param p        The site the connectivity number of which is
                        computed.
        \param ref_val  The reference value, telling whether the
                        foreground (true) or the background (false) is
                        considered.  */
    template<typename I>
    conn_number_t
    connectivity_number_2d_c8(const Image<I>& ima, const mln_psite(I)& p,
			      bool ref_val);



# ifndef MLN_INCLUDE_ONLY

    template<typename I>
    conn_number_t
    connectivity_number_2d_c4(const Image<I>& ima, const mln_psite(I)& p,
			      bool ref_val)
    {
      // Create a copy of mln::c4()'s own neighborhood to avoid
      // thread-unsafe accesses to this neighborhood.
      neighb2d nbh = c4();
      return connectivity_number_2d(ima, nbh, p, ref_val);
    }

    template<typename I>
    conn_number_t
    connectivity_number_2d_c8(const Image<I>& ima, const mln_psite(I)& p,
			      bool ref_val)
    {
      // Create a copy of mln::c8()'s own neighborhood to avoid
      // thread-unsafe accesses to this neighborhood.
      neighb2d nbh = c8();
      return connectivity_number_2d(ima, nbh, p, ref_val);
    }

# endif // MLN_INCLUDE_ONLY

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_CONNECTIVITY_NUMBER_2D_HH
