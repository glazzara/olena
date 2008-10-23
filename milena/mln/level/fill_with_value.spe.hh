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

#ifndef MLN_LEVEL_FILL_WITH_VALUE_SPE_HH
# define MLN_LEVEL_FILL_WITH_VALUE_SPE_HH

/// \file mln/level/fill_with_value.spe.hh
/// Specializations for mln::level::fill_with_value.
///
/// \todo See todos in mln/level/fill.hh

# ifndef MLN_LEVEL_FILL_WITH_VALUE_HH
#  error "Forbidden inclusion of *.spe.hh"
# endif // ! MLN_LEVEL_FILL_WITH_VALUE_HH

# include <mln/level/memset_.hh>


# ifndef MLN_INCLUDE_ONLY

namespace mln
{

  namespace level
  {

    namespace internal
    {
      template <typename I, typename V>
      inline
      void fill_with_value_tests(Image<I>& ima, const V& val);
    }


    // Implementations.
    // ----------------

    namespace impl
    {

      namespace generic
      {
	template <typename I, typename V>
	void fill_with_value(Image<I>& ima, const V& val);
      }


      // FIXME: Add specializations here...


      template <typename I, typename V>
      inline
      void fill_with_value_one_block(Image<I>& ima_, const V& val)
      {
	trace::entering("level::impl::fill_with_value_one_block");


	I& ima = exact(ima_);
	internal::fill_with_value_tests(ima, val);
        mln_precondition(((mlc_is(mln_trait_image_pw_io(I),
                                  trait::image::pw_io::read_write)::value ||
                           mlc_is(mln_trait_image_vw_io(I),
                                  trait::image::vw_io::read_write)::value) &&
                          mlc_is(mln_trait_image_value_access(I),
                                 trait::image::value_access::direct)::value));

        level::memset_(ima, ima.point_at_index(0), val, ima.nelements());

	trace::exiting("level::impl::fill_with_value_one_block");
      }

      template <typename I, typename V>
      inline
      void fill_with_value_cell_wise(Image<I>& ima_, const V& val)
      {
	trace::entering("level::impl::fill_with_value_cell_wise");

	I& ima = exact(ima_);

	internal::fill_with_value_tests(ima, val);

	mln_viter(I) v(ima.cells());
	for_all(v)
	  v.change_to(val);

	trace::exiting("level::impl::fill_with_value_cell_wise");
      }

      template <typename I, typename V>
      inline
      void fill_with_value_singleton(Image<I>& ima_, const V& val)
      {
        trace::entering("level::impl::fill_with_value_singleton");

	I& ima = exact(ima_);

	internal::fill_with_value_tests(ima, val);
        mln_precondition(((mlc_is(mln_trait_image_pw_io(I),
                                  trait::image::pw_io::read_write)::value ||
                           mlc_is(mln_trait_image_vw_io(I),
                                  trait::image::vw_io::read_write)::value) &&
                          mlc_is(mln_trait_image_value_access(I),
                                 trait::image::value_access::direct)::value));

	ima.val() = val;

	trace::exiting("level::impl::fill_with_value_singleton");
      }

    } // end of namespace mln::level::impl



    // Dispatch.
    // ---------

    namespace internal
    {

      template <typename I, typename V>
      void fill_with_value_dispatch(trait::image::value_access::direct,
                                    Image<I>& ima, const V& val)
      {
        if (mlc_is(mln_trait_image_pw_io(I),
                   trait::image::pw_io::read_write)::value ||
            mlc_is(mln_trait_image_vw_io(I),
                   trait::image::vw_io::read_write)::value)
          impl::fill_with_value_one_block(ima, val);
        else
          impl::generic::fill_with_value(ima, val);
      }

      template <typename I, typename V>
      void fill_with_value_dispatch(trait::image::value_access::any,
                                    Image<I>& ima, const V& val)
      {
        impl::generic::fill_with_value(ima, val);
      }



      template <typename I, typename V>
      void fill_with_value_dispatch(trait::image::value_storage::one_block,
                                    trait::image::vw_io::any,
                                    Image<I>& ima, const V& val)
      {
        fill_with_value_dispatch(mln_trait_image_value_access(I)(),
                                 ima, val);
      }

      template <typename I, typename V>
      void fill_with_value_dispatch(trait::image::value_storage::singleton,
                                    trait::image::vw_io::any,
                                    Image<I>& ima, const V& val)
      {
          impl::fill_with_value_singleton(ima, val);
      }

      template <typename I, typename V>
      void fill_with_value_dispatch(trait::image::value_storage::piecewise,
                                    trait::image::vw_io::any,
                                    Image<I>& ima, const V& val)
      {
        impl::generic::fill_with_value(ima, val);
      }


      template <typename I, typename V>
      void fill_with_value_dispatch(trait::image::value_storage::disrupted,
                                    trait::image::vw_io::read_write,
                                    Image<I>& ima, const V& val)
      {
        // FIXME
        //impl::fill_with_value_cell_wise(ima, val);
        impl::generic::fill_with_value(ima, val);
      }



      template <typename I, typename V>
      void fill_with_value_dispatch(trait::image::value_storage::disrupted,
                                    trait::image::vw_io::any,
                                    Image<I>& ima, const V& val)
      {
        impl::generic::fill_with_value(ima, val);
      }


      /// FIXME:
      // This specialization is only here to deal with image which have
      // non-updated properties (vw_io).
      template <typename I, typename V>
      void fill_with_value_dispatch(trait::image::value_storage::any,
                                    trait::undef,
                                    Image<I>& ima, const V& val)
      {
        impl::generic::fill_with_value(ima, val);
      }



      template <typename I,  typename V>
      void fill_with_value_dispatch(Image<I>& ima, const V& val)
      {
        fill_with_value_dispatch(mln_trait_image_value_storage(I)(),
                                 mln_trait_image_vw_io(I)(),
                                 ima, val);
      }

    } // end of namespace mln::level::internal


  } // end of namespace mln::level

} // end of namespace mln


# endif // ! MLN_INCLUDE_ONLY


#endif // ! MLN_LEVEL_FILL_WITH_VALUE_SPE_HH
