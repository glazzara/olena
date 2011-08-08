// Copyright (C) 2008, 2009, 2011 EPITA Research and Development
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

#ifndef MLN_TOPO_IS_SIMPLE_2D_HH
# define MLN_TOPO_IS_SIMPLE_2D_HH

/// \file
///
/// Define the function is_simple_2d which tests if a point is simple
/// or not (Cf bertrand.07.chap).

/// \fixme Merge with mln/topo/skeleton/is_simple_point.hh
/// \fixme The fastest version give an approximation of the result. Do
/// we want it to be exact?


#include <mln/core/concept/image.hh>
#include <mln/core/concept/neighborhood.hh>

#include <mln/core/alias/point2d.hh>
#include <mln/core/alias/neighb2d.hh>


namespace mln
{

  namespace topo
  {

    /// Test if a point is simple or not. A point of an object is simple
    /// if in its c8 neiborhood, there is exactly one connected component of the
    /// object, and only one connected component of the background
    /// Examples : ( | == object, - = background)
    ///
    ///          - - |
    ///          | P | Here p is simple in the c4 and c8 case.
    ///          | | |
    ///
    ///          - | -
    ///          | P | Here p is never simple.
    ///          | | |
    template <typename N>
    struct is_simple_2d_t
    {

      is_simple_2d_t(const Neighborhood<N>& nbh);

      template <typename I>
      bool check(const I& ima, const mln_psite(I)& p) const;

      template <typename I>
      bool check__(const I& ima, unsigned p) const;

      template <typename I, typename N2>
      unsigned nb_connectivity2d(const I&, const N2& nbh,
				 const mln_psite(I)& p, bool object) const;
      template <typename I, typename N2>
      unsigned nb_connectivity2d__(const I&, const N2& nbh,
				   unsigned p, bool object) const;

    protected:
      const N& nbh_;
    };



# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {

      static const unsigned char connectivity_number_c8[256] =
      {
	0,  1,  1,  1,   1,  2,  1,  1,   1,  1,  1,  1,   2,  2,  1,  1,
	1,  2,  1,  1,   1,  2,  1,  1,   2,  2,  1,  1,   2,  2,  1,  1,
	1,  2,  2,  2,   2,  3,  2,  2,   1,  1,  1,  1,   2,  2,  1,  1,
	2,  3,  2,  2,   2,  3,  2,  2,   2,  2,  1,  1,   2,  2,  1,  1,

	1,  2,  2,  2,   2,  3,  2,  2,   1,  1,  1,  1,   2,  2,  1,  1,
	1,  2,  1,  1,   1,  2,  1,  1,   1,  1,  1,  1,   1,  1,  1,  1,
	1,  2,  2,  2,   2,  3,  2,  2,   1,  1,  1,  1,   2,  2,  1,  1,
	1,  2,  1,  1,   1,  2,  1,  1,   1,  1,  1,  1,   1,  1,  1,  1,

	1,  2,  2,  2,   2,  3,  2,  2,   2,  2,  2,  2,   3,  3,  2,  2,
	1,  2,  1,  1,   1,  2,  1,  1,   2,  2,  1,  1,   2,  2,  1,  1,
	2,  3,  3,  3,   3,  4,  3,  3,   2,  2,  2,  2,   3,  3,  2,  2,
	2,  3,  2,  2,   2,  3,  2,  2,   2,  2,  1,  1,   2,  2,  1,  1,

	1,  2,  2,  2,   2,  3,  2,  2,   1,  1,  1,  1,   2,  2,  1,  1,
	1,  2,  1,  1,   1,  2,  1,  1,   1,  1,  1,  1,   1,  1,  1,  1,
	1,  2,  2,  2,   2,  3,  2,  2,   1,  1,  1,  1,   2,  2,  1,  1,
	1,  2,  1,  1,   1,  2,  1,  1,   1,  1,  1,  1,   1,  1,  1,  1
      };


      static const unsigned char connectivity_number_c4[256] =
      {
	0,  0,  1,  1,   0,  0,  1,  1,   1,  1,  2,  1,   1,  1,  2,  1,
	1,  1,  2,  2,   1,  1,  1,  1,   2,  2,  3,  2,   2,  2,  2,  1,
	0,  0,  1,  1,   0,  0,  1,  1,   1,  1,  2,  1,   1,  1,  2,  1,
	1,  1,  2,  2,   1,  1,  1,  1,   2,  2,  3,  2,   2,  2,  2,  1,

	1,  1,  2,  2,   1,  1,  2,  2,   2,  2,  3,  2,   2,  2,  3,  2,
	2,  2,  3,  3,   2,  2,  2,  2,   3,  3,  4,  3,   3,  3,  3,  2,
	1,  1,  2,  2,   1,  1,  2,  2,   1,  1,  2,  1,   1,  1,  2,  1,
	2,  2,  3,  3,   2,  2,  2,  2,   2,  2,  3,  2,   2,  2,  2,  1,

	0,  0,  1,  1,   0,  0,  1,  1,   1,  1,  2,  1,   1,  1,  2,  1,
	1,  1,  2,  2,   1,  1,  1,  1,   2,  2,  3,  2,   2,  2,  2,  1,
	0,  0,  1,  1,   0,  0,  1,  1,   1,  1,  2,  1,   1,  1,  2,  1,
	1,  1,  2,  2,   1,  1,  1,  1,   2,  2,  3,  2,   2,  2,  2,  1,

	1,  1,  2,  2,   1,  1,  2,  2,   2,  2,  3,  2,   2,  2,  3,  2,
	1,  1,  2,  2,   1,  1,  1,  1,   2,  2,  3,  2,   2,  2,  2,  1,
	1,  1,  2,  2,   1,  1,  2,  2,   1,  1,  2,  1,   1,  1,  2,  1,
	1,  1,  2,  2,   1,  1,  1,  1,   1,  1,  2,  1,   1,  1,  1,  1
      };

    } // end of namespace mln::topo::internal

    template <typename N>
    is_simple_2d_t<N>::is_simple_2d_t(const Neighborhood<N>& nbh)
      : nbh_(exact(nbh))
    {
      mln_precondition(nbh_.is_valid());
    }



    template <typename N>
    template <typename I, typename N2>
    unsigned
    is_simple_2d_t<N>::nb_connectivity2d(const I& ima, const N2& nbh,
					 const mln_psite(I)& p, bool b) const
    {
      mln_precondition(ima.is_valid());
      mln_precondition(nbh.is_valid());

      unsigned res = 0;

      mln_fwd_niter(N2) n(c8(), p);
      for_all(n)
      {
	res = (res << 1);
	if (ima.has(n) && ima(n) == b)
	  res = res | 1;
      }

      switch (nbh.size())
      {
	case 4: // C4
	  return internal::connectivity_number_c4[res];
	case 8: // C8
	  return internal::connectivity_number_c8[res];
	default:
	  mln_assertion(0);

      }

      return 0;
    }


    template <typename N>
    template <typename I, typename N2>
    unsigned
    is_simple_2d_t<N>::nb_connectivity2d__(const I& ima, const N2& nbh,
					   unsigned p, bool b) const
    {
      mln_precondition(ima.is_valid());
      mln_precondition(nbh.is_valid());

      unsigned res = 0;

      static util::array<int>
	noffset = mln::offsets_wrt(ima, c8());

      for (unsigned i = 0; i < noffset.nelements(); ++i)
      {
	res = (res << 1);
	if (ima.element(p + noffset[i]) == b)
	  res = res | 1;
      }

      switch (nbh.size())
      {
	case 4: // C4
	  return internal::connectivity_number_c4[res];
	case 8: // C8
	  return internal::connectivity_number_c8[res];
	default:
	  mln_assertion(0);

      }

      return 0;
    }


    template <typename N>
    template <typename I>
    inline
    bool
    is_simple_2d_t<N>::check(const I& ima, const mln_psite(I)& p) const
    {
      mln_precondition(ima.is_valid());
      return (nb_connectivity2d(ima, nbh_.foreground(), p, true) == 1)  // Consider neighbor.
	&& (nb_connectivity2d(ima, nbh_.background(), p, false) == 1); // Consider complement neighbor.
    }




    template <typename N>
    template <typename I>
    inline
    bool
    is_simple_2d_t<N>::check__(const I& ima, unsigned p) const
    {
      mln_precondition(ima.is_valid());
      return (nb_connectivity2d__(ima, nbh_.foreground(), p, true) == 1)  // Consider neighbor
	&& (nb_connectivity2d__(ima, nbh_.background(), p, false) == 1); // Consider complement neighbor.
    }



# endif // MLN_INCLUDE_ONLY

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_IS_SIMPLE_2D_HH
