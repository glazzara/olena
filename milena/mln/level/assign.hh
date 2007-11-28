// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_LEVEL_ASSIGN_HH
# define MLN_LEVEL_ASSIGN_HH

/*! \file mln/level/assign.hh
 *
 * \brief Assignment between a couple of images.
 *
 * \todo Assign should be a precondition then a call to level::fill.
 */

# include <mln/core/concept/image.hh>


// Specializations are in:
# include <mln/level/assign.spe.hh>



namespace mln
{

  namespace level
  {

    /*! Assignment of image \p target with image \p data.
     *
     * \param[out] target The image to be assigned.
     * \param[in] data The auxiliary image.
     *
     * \pre target.domain = data.domain
     *
     * \todo Overload in impl:: for mixed (fast, non-fast).
     *
     * \todo Overload in impl:: for (fast, fast) if same value using memcpy.
     */
    template <typename L, typename R>
    void assign(Image<L>& target, const Image<R>& data);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      namespace generic
      {
	template <typename L, typename R>
	inline
	void assign_(L& target, const R& data)
	{
	  trace::entering("level::impl::generic::assign_");

	  mln_piter(L) p(target.domain());
	  for_all(p)
	    target(p) = data(p);

	  trace::exiting("level::impl::generic::assign_");
	}

      }  // end of namespace mln::level::impl::generic

    } // end of namespace mln::level::impl


    // Facade.

    template <typename L, typename R>
    inline
    void assign(Image<L>& target, const Image<R>& data)
    {
      trace::entering("level::assign");

      mln_precondition(exact(data).domain() == exact(target).domain());
      impl::assign_(mln_trait_image_speed(L)(), exact(target),
		    mln_trait_image_speed(R)(), exact(data));

      trace::exiting("level::assign");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::level

} // end of namespace mln


#endif // ! MLN_LEVEL_ASSIGN_HH
