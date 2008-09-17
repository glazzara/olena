// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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


#ifndef MLN_CORE_EXTENSION_FILL_HH
# define MLN_CORE_EXTENSION_FILL_HH

/*! \file mln/extension/fill.hh
 *
 * \brief Define function that fills domain extension.
 *
 */

# include <mln/core/concept/image.hh>
# include <mln/trait/image/props.hh>
# include <mln/border/fill.hh>
# include <mln/level/fill_with_value.hh>

namespace mln
{

  namespace extension
  {

    /*! Fill the domain extension of image \p ima with the
     *  single value \p v.
     *
     * \param[in,out] ima The image whose domain extension is to be filled.
     * \param[in] v The value to assign.
     *
     * \pre \p ima has to be initialized.
     *
     * \todo Optimize with memset if possible.
     */
    template <typename I>
    void fill(const Image<I>& ima, const mln_value(I)& v);

    namespace impl
    {
      template <typename I>
      void fill_fixed_extension(const Image<I>& ima, const mln_value(I)& v);
    }

# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    void fill(const Image<I>& ima, const mln_value(I)& v)
    {
      // Case of ext_io::read_only.
      if (mlc_equal(mln_trait_image_ext_io(I),
		    mln::trait::image::ext_io::read_only)::value == true)
	return; // No-op.

      // Case of ext_domain::none.
      if (mlc_equal(mln_trait_image_ext_domain(I),
		    mln::trait::image::ext_domain::none)::value == true)
	return; // No-op.

      // Case of ext_domain::extendable.
      if (mlc_equal(mln_trait_image_ext_domain(I),
		    mln::trait::image::ext_domain::extendable)::value == true)
	return mln::border::fill(ima, v);


      // Case of ext_domain::fixed.
      if (mlc_equal(mln_trait_image_ext_domain(I),
		    mln::trait::image::ext_domain::fixed)::value == true)

	// Case of ext_value::single.
	if (mlc_equal(mln_trait_image_ext_value(I),
		      mln::trait::image::ext_value::single)::value == true)
	  return impl::fill_fixed_extension(const_cast<I&>(exact(ima)), v);
	else
	  return mln::level::fill_with_value(const_cast<I&>(exact(ima)), v);

    }

    namespace impl
    {
      template <typename I>
      void fill_fixed_extension(Image<I>& ima_, const mln_value(I)& v)
      {
	I& ima = exact(ima_);
	ima.extension() = v;
      }
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::extension

} // end of namespace mln


#endif // ! MLN_CORE_EXTENSION_FILL_HH
