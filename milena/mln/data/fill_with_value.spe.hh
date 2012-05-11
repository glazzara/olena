// Copyright (C) 2008, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_DATA_FILL_WITH_VALUE_SPE_HH
# define MLN_DATA_FILL_WITH_VALUE_SPE_HH

/// \file
///
/// Specializations for mln::data::fill_with_value.

# ifndef MLN_DATA_FILL_WITH_VALUE_HH
#  error "Forbidden inclusion of *.spe.hh"
# endif // ! MLN_DATA_FILL_WITH_VALUE_HH

# include <mln/data/memset_.hh>
# include <mln/opt/value.hh>
# include <mln/opt/element.hh>


# ifndef MLN_INCLUDE_ONLY

namespace mln
{

  namespace data
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
	trace::entering("data::impl::fill_with_value_one_block");

	I& ima = exact(ima_);

	internal::fill_with_value_tests(ima, val);
        mlc_and(mlc_or(mlc_is(mln_trait_image_pw_io(I),
                              trait::image::pw_io::read_write),
                       mlc_is(mln_trait_image_vw_io(I),
                              trait::image::vw_io::read_write)),
                mlc_is(mln_trait_image_value_access(I),
                       trait::image::value_access::direct))::check();

	mln_value(I) v = static_cast<mln_value(I)>(val);
        data::memset_(ima, ima.point_at_offset(0), v, opt::nelements(ima));

	trace::exiting("data::impl::fill_with_value_one_block");
      }

      template <typename I, typename V>
      inline
      void fill_with_value_cell_wise(Image<I>& ima_, const V& val)
      {
	trace::entering("data::impl::fill_with_value_cell_wise");

	I& ima = exact(ima_);

	internal::fill_with_value_tests(ima, val);

	mln_viter(I) v(ima.cells());
	for_all(v)
	  v.change_to(val);

	trace::exiting("data::impl::fill_with_value_cell_wise");
      }

      template <typename I, typename V>
      inline
      void fill_with_value_singleton(Image<I>& ima_, const V& val)
      {
        trace::entering("data::impl::fill_with_value_singleton");

	I& ima = exact(ima_);

	internal::fill_with_value_tests(ima, val);
        mlc_or(mlc_is(mln_trait_image_pw_io(I),
                      trait::image::pw_io::read_write),
               mlc_is(mln_trait_image_vw_io(I),
                      trait::image::vw_io::read_write))::check();

        opt::value(ima) = static_cast<mln_value(I)>(val);

	trace::exiting("data::impl::fill_with_value_singleton");
      }

    } // end of namespace mln::data::impl



    // Dispatch.
    // ---------

    namespace internal
    {

      template <typename I, typename V>
      void fill_with_value_dispatch(trait::image::value_access::direct,
                                    Image<I>& ima, const V& val)
      {
        impl::fill_with_value_one_block(ima, val);
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
        impl::fill_with_value_cell_wise(ima, val);
      }


      template <typename I, typename V>
      void fill_with_value_dispatch(trait::image::value_storage::disrupted,
                                    trait::image::vw_io::any,
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

    } // end of namespace mln::data::internal


  } // end of namespace mln::data

} // end of namespace mln


# endif // ! MLN_INCLUDE_ONLY


#endif // ! MLN_DATA_FILL_WITH_VALUE_SPE_HH
