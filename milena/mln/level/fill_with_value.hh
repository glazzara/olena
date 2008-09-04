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
 */

# include <mln/core/concept/image.hh>
# include <mln/core/image/inplace.hh>
# include <mln/core/image/instant.hh>


namespace mln
{

  namespace level
  {

    /*! Fill the whole image \p ima with the single value \p v.
     *
     * \param[in,out] ima The image to be filled.
     * \param[in] val The value to assign to all sites.
     *
     * \pre \p ima has to be initialized.
     *
     * \todo Optimize when \p ima is large and sizeof(mln_value(I)) > 1.
     */
    template <typename I, typename V>
    void fill_with_value(Image<I>& ima, const V& val);


    // Case of instant images.
    template <typename I, typename V>
    void fill_with_value(const Image< instant_<I> >& ima, const V& val);



# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I>
      inline
      void fill_with_value_site_wise_any(I& ima, const mln_value(I)& val)
      {
	trace::entering("level::impl::fill_with_value_site_wise_any");
	
	mln_piter(I) p(ima.domain());
	for_all(p)
	  ima(p) = val;
	
	trace::exiting("level::impl::fill_with_value_site_wise_any");
      }


      template <typename I, typename V>
      inline
      void fill_with_value_site_wise_one_block(I& ima, const V& val)
      {
	trace::entering("level::impl::fill_with_value_site_wise_one_block");

	// level::memset_(ima, ima.point_at_index(0), v, ima.nelements());
	const unsigned n = ima.nelements();
	mln_value(I)* ptr = ima.buffer();
	for (unsigned i = 0; i < n; ++i)
	  *ptr++ = val;

	trace::exiting("level::impl::fill_with_value_site_wise_one_block");
      }


      template <typename I, typename V>
      inline
      void fill_with_value_cell_wise(I& ima, const V& val)
      {
	trace::entering("level::impl::fill_with_value_cell_wise");

	abort();
// 	mln_viter(I) v(ima.values());
// 	for_all(v)
// 	  v.change_to(val);
	
	trace::exiting("level::impl::fill_with_value_cell_wise");
      }


      namespace dispatch
      {

	// Cases for "value_browsing == site_wise_only" w.r.t. value_storage.

	template <typename I, typename V>
	inline
	void fill_with_value_site_wise_(mln::trait::image::value_storage::any,
					I& ima, const V& val)
	{
	  fill_with_value_site_wise_any(ima, val);
	}

	template <typename I, typename V>
	inline
	void fill_with_value_site_wise_(mln::trait::image::value_storage::one_block,
					I& ima, const V& val)
	{
	  fill_with_value_site_wise_one_block(ima, val);
	}

	// Case site_wise -> selector w.r.t. value_storage.

	template <typename I, typename V>
	inline
	void fill_with_value_site_wise(I& ima, const V& val)
	{
	  fill_with_value_site_wise_(mln_trait_image_value_storage(I)(),
				     ima, val);
	}


	// Cases w.r.t. value_browsing.

	template <typename I, typename V>
	inline
	void fill_with_value_(mln::trait::image::value_browsing::site_wise_only,
			      I& ima, const V& val)
	{
	  fill_with_value_site_wise(ima, val);
	}

	template <typename I, typename V>
	inline
	void fill_with_value_(mln::trait::image::value_browsing::cell_wise,
			      I& ima, const V& val)
	{
	  fill_with_value_cell_wise(ima, val);
	}

	// Selector w.r.t. value_browsing.

	template <typename I, typename V>
	inline
	void fill_with_value_(I& ima, const V& val)
	{
	  fill_with_value_(mln_trait_image_value_browsing(I)(),
			   ima, val);
	}

      } // end of namespace mln::level::impl::dispatch

    } // end of namespace mln::level::impl



    /// Facade.

    template <typename I, typename V>
    inline
    void fill_with_value(Image<I>& ima, const V& val)
    {
      trace::entering("level::fill");

      mlc_is(mln_trait_image_value_io(I),
	     mln::trait::image::value_io::read_write)::check();
      mlc_converts_to(V, mln_value(I))::check();

      mln_precondition(exact(ima).has_data());

      impl::dispatch::fill_with_value_(exact(ima), exact(val));

      trace::exiting("level::fill");
    }


    // Un-instant.

    template <typename I, typename V>
    void fill_with_value(const Image< instant_<I> >& ima, const V& val)
    {
      fill_with_value(exact(ima).un_instant_(), val);
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::level

} // end of namespace mln


#endif // ! MLN_LEVEL_FILL_WITH_VALUE_HH
