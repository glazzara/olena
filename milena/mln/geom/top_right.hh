// Copyright (C) 2012 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_GEOM_TOP_RIGHT_HH
# define MLN_GEOM_TOP_RIGHT_HH

/// \file
///
/// Give the top right point of an image.

# include <mln/core/concept/image.hh>
# include <mln/geom/bbox.hh>


namespace mln
{

  namespace geom
  {

    /*! \brief Give the top right point of a 2d or 3d image.

      In 3d, the top-right point corresponds to the top right corner
      of the back face.
     */
    template <typename I>
    mln_site(I) top_right(const Image<I>& ima);


    /*! \brief Give the top right point of a box 2d or 3d.
      \overload
     */
    template <typename B>
    mln_site(B) top_right(const Box<B>& b);

# ifndef MLN_INCLUDE_ONLY


    namespace impl
    {

      template <typename B>
      inline
      mln_site(B) top_right_2d(const Box<B>& b_)
      {
	const B& b = exact(b_);
	mln_site(B) top_right(b.pmin().row(),
			      b.pmax().col());
	return top_right;
      }

      template <typename B>
      inline
      mln_site(B) top_right_3d(const Box<B>& b_)
      {
	const B& b = exact(b_);
	mln_site(B) top_right(b.pmin().sli(),
			      b.pmin().row(),
			      b.pmax().col());
	return top_right;
      }

    } // end of namespace mln::geom::impl


    namespace internal
    {

      // Not implemented.
      template <unsigned n>
      struct top_right_dispatch_impl;

      // 2d
      template <>
      struct top_right_dispatch_impl<2>
      {
	template <typename B>
	inline
	mln_site(B) run(const Box<B>& box) const
	{
	  return impl::top_right_2d(box);
	}
      };

      // 3d
      template <>
      struct top_right_dispatch_impl<3>
      {
	template <typename B>
	inline
	mln_site(B) run(const Box<B>& box) const
	{
	  return impl::top_right_3d(box);
	}
      };


      template <typename B>
      inline
      mln_site(B) top_right_dispatch(const Box<B>& box)
      {
	typedef mln_site(B) P;
	return top_right_dispatch_impl<P::dim>().run(box);
      }

    } // end of namespace mln::geom::internal


    // Facades

    template <typename I>
    inline
    mln_site(I) top_right(const Image<I>& ima)
    {
      mln_precondition(exact(ima).is_valid());
      mln_site(I) output = internal::top_right_dispatch(exact(ima).domain());

      return output;
    }

    template <typename B>
    inline
    mln_site(B) top_right(const Box<B>& b)
    {
      mln_precondition(exact(b).is_valid());
      mln_site(B) output = internal::top_right_dispatch(b);

      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::geom

} // end of namespace mln


#endif // ! MLN_GEOM_TOP_RIGHT_HH
