// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_TOPO_SKELETON_IS_SIMPLE_POINT_HH
# define MLN_TOPO_SKELETON_IS_SIMPLE_POINT_HH

/// \file
///
/// is_simple_point tells if a point is simple or not.
/// For more information refer to bertrand.07.chap.

# include <mln/core/concept/image.hh>
# include <mln/core/alias/point2d.hh>
# include <mln/core/alias/neighb2d.hh>

namespace mln
{

  namespace topo
  {

    namespace skeleton
    {

      /*! Tell if a point is simple or not. A point of an object is simple
       *  if in its c8 neiborhood, there is exactly one connected component of the
       *  object, and only one connected component of the background
       *  Examples : ( | == object, - = background)
       * \verbatim

       - - |
       | P | Here p is simple in the c4 and c8 case.
       | | |

       - | -
       | P | Here p is never simple.
       | | |

       \endverbatim
       */
      template <typename I, typename N>
      bool
      is_simple_point(const Image<I>& ima,
		      const Neighborhood<N>& nbh,
		      const mln_site(I)& p);


# ifndef MLN_INCLUDE_ONLY


      namespace internal
      {


	static const unsigned char nb_connexity_c8[256] =
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

	static const unsigned char nb_connexity_c4[256] =
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



	template <typename I, typename N>
        inline
        unsigned
        nb_connexity2d(const I& ima,
		       const N& nbh,
		       const mln_site(I)& p,
		       bool object)
	{
	  unsigned res = 0;

	  mln_bkd_niter(N) n(c8(), p);
	  for_all(n)
	  {
	    res = (res << 1);
	    if (ima.domain().has(n) && ima(n) == object)
	      res = res | 1;
	  }

	  if (nbh == c8())
	    return nb_connexity_c8[res];
	  else
	  {
	    mln_assertion(nbh == c4());
	    return nb_connexity_c4[res];
	  }
	}


	template <typename N>
	neighb2d
	complement2d(const Neighborhood<N>& nbh_)
	{
	  const N& nbh = exact(nbh_);
	  mln_precondition(nbh.is_valid());
	  mln_precondition(nbh == c4() || nbh == c8());

	  if (nbh == c4())
	    return c8();
	  else
	    return c4();
	}


	// Tests.

	template <typename I, typename N>
	inline
	void
	is_simple_point_tests(const Image<I>& ima_,
			      const Neighborhood<N>& nbh_,
			      const mln_site(I)& p)
	{
	  const I& ima = exact(ima_);
	  const N& nbh = exact(nbh_);

	  mln_assertion(nbh == c4() || nbh == c8());
	  mln_precondition(ima.is_valid());
	  mln_precondition(nbh.is_valid());

	  (void) ima;
	  (void) nbh;
	  (void) p;
	}

      } // end of namespace mln::topo::skeleton::internal



      // Implementations

      namespace impl
      {

	template <typename I, typename N>
	inline
	bool
	is_simple_point2d(const Image<I>& ima_,
			  const Neighborhood<N>& nbh_,
			  const point2d& p)
	{
	  const I& ima = exact(ima_);
	  const N& nbh = exact(nbh_);

	  internal::is_simple_point_tests(ima, nbh, p);

	  bool b = (internal::nb_connexity2d(ima, nbh, p, true) == 1)
	    && (internal::nb_connexity2d(ima, internal::complement2d(nbh),
					 p, false) == 1);

	  trace::exiting("topo::skeleton::is_simple_point2d");
	  return b;
	}

      } // end of namespace mln::topo::skeleton::impl




      // Dispatch

      namespace internal
      {

	template <typename I, typename N>
        inline
        bool
	is_simple_point_dispatch(const Image<I>& ima,
				 const Neighborhood<N>& nbh,
				 const point2d& p)
	{
	  return impl::is_simple_point2d(ima, nbh, p);
	}


	template <typename I, typename N>
        inline
        bool
	is_simple_point_dispatch(const Image<I>& ima,
				 const Neighborhood<N>& nbh,
				 const mln_site(I)& p)
	{
	  /// Not implemented for that site type yet.
	  mlc_abort(I)::check();
	  return false;
	}

      } // end of namespace mln::topo::skeleton::internal





      // Facade

      template <typename I, typename N>
      inline
      bool
      is_simple_point(const Image<I>& ima,
		      const Neighborhood<N>& nbh,
		      const mln_site(I)& p)
      {
	trace::entering("topo::skeleton::is_simple_point2d");

	internal::is_simple_point_tests(ima, nbh, p);

	bool b = internal::is_simple_point_dispatch(ima, nbh, p);

	trace::exiting("topo::skeleton::is_simple_point2d");
	return b;
      }


# endif // MLN_TOPO_SKELETON_INCLUDE_ONLY

    } // end of namespace mln::topo::skeleton

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_SKELETON_IS_SIMPLE_POINT_HH
