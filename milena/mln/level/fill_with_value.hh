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

#ifndef MLN_LEVEL_FILL_WITH_VALUE_HH
# define MLN_LEVEL_FILL_WITH_VALUE_HH

/*! \file mln/level/fill_with_value.hh
 *
 * \brief Fill an image with a value, that is, set all pixel values to
 * the given value.
 *
 * \todo Overload for fastest images.
 * \todo Handle all value_storage properties (piecewise, etc.)
 * \todo Make memset_ work and use it when appropriate.
 * \todo Move specializations in a extra file.
 * \toto Activate fill_with_value_cell_wise.
 * \todo Optimize when \p ima is large and sizeof(mln_value(I)) > 1.
 */

# include <cstdlib>

# include <mln/core/concept/image.hh>



// Specializations are in:
# include <mln/level/fill_with_value.spe.hh>


namespace mln
{

  namespace level
  {

    /// \brief Fill the whole image \p ima with the single value \p v.
    ///
    /// \param[in,out] ima The image to be filled.
    /// \param[in] val The value to assign to all sites.
    ///
    /// \pre \p ima has to be initialized.
    ///
    /// \{
    template <typename I, typename V>
    void fill_with_value(Image<I>& ima, const V& val);
    /// \}



# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {

      template <typename I, typename V>
      inline
      void fill_with_value_tests(Image<I>& ima, const V&)
      {
	// Avoid a warning about an undefined variable when NDEBUG
	// is not defined.
	(void) ima;

	mlc_converts_to(mln_exact(V), mln_value(I))::check();
	mln_precondition(exact(ima).has_data());
      }

    } // end of namespace mln::level::internal


    namespace impl
    {

      namespace generic
      {

	template <typename I, typename V>
	void fill_with_value(Image<I>& ima_, const V& val)
	{
	  trace::entering("level::impl::generic::fill_with_value");

	  I& ima = exact(ima_);

	  internal::fill_with_value_tests(ima, val);
          // FIXME: activate this test
          //mlc_is(mln_trait_image_pw_io(I),
          //trait::image::pw_io::read_write)::check();

	  mln_value(I) v = exact(val);
	  mln_piter(I) p(ima.domain());
	  for_all(p)
	    ima(p) = v;

	  trace::exiting("level::impl::generic::fill_with_value");
	}

      } // end if namespace mln::level::impl::generic

    } // end of namespace mln::level::impl


    // Facade.

    template <typename I, typename V>
    inline
    void fill_with_value(Image<I>& ima, const V& val)
    {
      trace::entering("level::fill_with_value");

      internal::fill_with_value_tests(ima, val);
      internal::fill_with_value_dispatch(ima, val);

      trace::exiting("level::fill_with_value");
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::level

} // end of namespace mln


#endif // ! MLN_LEVEL_FILL_WITH_VALUE_HH
