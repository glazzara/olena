// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_EXTENSION_FILL_HH
# define MLN_EXTENSION_FILL_HH

/// \file
///
/// Define function that fills domain extension.
///
/// \todo Test the compatibility between val and mln_value(I) because,
/// while unmorphing, this type can change...

# include <mln/core/concept/image.hh>
# include <mln/trait/image/props.hh>
# include <mln/border/fill.hh>
# include <mln/data/fill_with_value.hh>


namespace mln
{

  namespace extension
  {

    /*! Fill the domain extension of image \p ima with the
     *  single value \p v.
     *
     * \param[in,out] ima The image whose domain extension is to be filled.
     * \param[in] val The value to assign.
     *
     * \pre \p ima has to be initialized.
     *
     * \todo Optimize with memset if possible.
     */
    template <typename I>
    void fill(const Image<I>& ima, const mln_value(I)& val);


# ifndef MLN_INCLUDE_ONLY


    namespace internal
    {

      // Do it.

      template <typename I>
      void do_fill(I& ima, const mln_value(I)& val); // Entry point.

      template <typename I>
      void do_fill(mln::trait::image::ext_domain::some,
		   mln::trait::image::ext_value::single,
		   mln::trait::image::ext_domain::none,
		   I& ima, const mln_value(I)& val)
      {
	ima.change_extension(val);
      }

      template <typename I>
      void do_fill(mln::trait::image::ext_domain::some,
		   mln::trait::image::ext_value::multiple,
		   mln::trait::image::ext_domain::none,
		   I& ima, const mln_value(I)& val)
      {
	border::fill(ima, val);
	ima.change_extension(val);
      }

      template <typename I>
      void do_fill(mln::trait::image::ext_domain::some,
		   mln::trait::image::ext_value::any,
		   mln::trait::image::ext_domain::some,
		   I& ima, const mln_value(I)& val)
      {
	mln::extension::fill(ima.unmorph_(), val);
      }

      template <typename I>
      void do_fill(mln::trait::image::ext_domain::none,
		   mln::trait::image::ext_value::any,
		   mln::trait::image::ext_domain::any,
		   I& ima, const mln_value(I)& val)
      {
	(void) ima;
	(void) val;
	// Oops...
      }

      template <typename I>
      void do_fill(I& ima, const mln_value(I)& val)
      {
	typedef typename I::delegatee D;
	do_fill(mln_trait_image_ext_domain(I)(),
		mln_trait_image_ext_value(I)(),
		mln_trait_image_ext_domain(D)(),
		ima, val);
      }



      // Dispatch.

      template <typename I>
      void fill_dispatch(mln::trait::image::ext_domain::none,
			 mln::trait::image::ext_io::any,
			 I& /* ima */, const mln_value(I)& /* val */)
      {
	// No-op cause no extension domain, no border.
      }

      template <typename I>
      void fill_dispatch(mln::trait::image::ext_domain::some,
			 mln::trait::image::ext_io::read_only,
			 I& ima, const mln_value(I)& val)
      {
	// No-op for the extension domain, yet:
	border::fill(ima, val);
      }

      template <typename I>
      void fill_dispatch(mln::trait::image::ext_domain::extendable,
			 mln::trait::image::ext_io::read_write,
			 I& ima, const mln_value(I)& val)
      {
	// Just fill the border.
	border::fill(ima, val);
      }

      template <typename I>
      void fill_dispatch(mln::trait::image::ext_domain::some,
			 mln::trait::image::ext_io::read_write,
			 I& ima, const mln_value(I)& val)
      {
	do_fill(ima, val);
      }

      template <typename I>
      void fill_dispatch(const Image<I>& ima_, const mln_value(I)& val)
      {
	I& ima = const_cast<I&>(exact(ima_));
	fill_dispatch(mln_trait_image_ext_domain(I)(),
		      mln_trait_image_ext_io(I)(),
		      ima, val);
      }

    } // end of namespace mln::extension::internal


    // Facade.

    template <typename I>
    void fill(const Image<I>& ima, const mln_value(I)& val)
    {
      trace::entering("extension::fill");

      mln_precondition(exact(ima).is_valid());
      internal::fill_dispatch(ima, val);

      trace::exiting("extension::fill");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::extension

} // end of namespace mln


#endif // ! MLN_EXTENSION_FILL_HH
