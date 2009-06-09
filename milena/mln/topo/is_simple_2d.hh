// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/core/concept/image.hh>
#include <mln/core/concept/neighborhood.hh>

#include <mln/core/alias/point2d.hh>
#include <mln/core/alias/neighb2d.hh>


namespace mln
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

  template<typename I, typename N>
  bool
  is_simple_2d(const Image<I>& ima, const Neighborhood<N>& nbh, const mln_psite(I)& p);



  struct is_simple_2d_t
  {
    template<typename I, typename N>
    bool operator()(const Image<I>& ima,
		    const Neighborhood<N>& nbh,
		    const mln_psite(I)& p) const
    {
      return is_simple_2d(ima, nbh, p);
    }
  };



# ifndef MLN_INCLUDE_ONLY

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


  template<typename I, typename N>
  inline
  unsigned
  connectivity_number_2d(const Image<I>& ima_, const Neighborhood<N>& nbh_,
			 const mln_psite(I)& p, bool b)
  {
    const I& ima = exact(ima_);
    const N& nbh = exact(nbh_);

    unsigned res = 0;

    mln_fwd_niter(neighb2d) n(c8(), p);
    for_all(n)
      {
	res = (res << 1);
	if (ima.has(n) && ima(n) == b)
	  res = res | 1;
      }

    unsigned number;

    switch (nbh.size())
      {
      case 4:
	number = connectivity_number_c4[res];
	break;
      case 8:
	number = connectivity_number_c8[res];
	break;
      default:
	mln_assertion(0);
      }

    return number;
  }


  template<typename I, typename N>
  inline
  bool
  is_simple_2d(const Image<I>& ima, const Neighborhood<N>& nbh_, const mln_psite(I)& p)
  {
    const N& nbh = exact(nbh_);
    return
      connectivity_number_2d(ima, nbh.foreground(), p, true ) == 1 &&
      connectivity_number_2d(ima, nbh.background(), p, false) == 1;
  }

# endif // MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // ! MLN_TOPO_IS_SIMPLE_2D_HH
