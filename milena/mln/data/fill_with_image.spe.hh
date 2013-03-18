// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_DATA_FILL_WITH_IMAGE_SPE_HH
# define MLN_DATA_FILL_WITH_IMAGE_SPE_HH

/*! \file
 *
 * \brief Specializations for mln::data::fill_with_image.
 *
 */

# ifndef MLN_DATA_FILL_WITH_IMAGE_HH
#  error "Forbidden inclusion of *.spe.hh"
# endif // ! MLN_DATA_FILL_WITH_IMAGE_HH

# include <mln/data/memcpy_.hh>
# include <mln/data/fill_with_value.hh>
# include <mln/core/pixel.hh>
# include <mln/core/box_runstart_piter.hh>
# include <mln/border/get.hh>
# include <mln/opt/value.hh>
# include <mln/opt/element.hh>


# ifndef MLN_INCLUDE_ONLY

namespace mln
{

  namespace data
  {

    namespace internal
    {

      template <typename I, typename J>
      inline
      void fill_with_image_tests(Image<I>& ima, const Image<J>& data);

    } // end of namespace mln::data::internal



    namespace impl
    {

      namespace generic
      {
	template <typename I, typename J>
	void fill_with_image(Image<I>& ima, const Image<J>& data);
      }

      template <typename I, typename J>
      void fill_with_image_fastest(Image<I>& ima_, const Image<J>& data_)
      {
        mln_trace("data::impl::fill_with_image_fastest");

        I& ima = exact(ima_);
        const J& data      = exact(data_);

        data::internal::fill_with_image_tests(ima, data);

        pixel<const J> src (data);
        pixel<I> dst(ima);
        *(src.address_()) = data.buffer();
        *(dst.address_()) = ima.buffer();

        memcpy_(dst, src, opt::nelements(ima));

      }

      template <typename I, typename J>
      void fill_with_image_fast(Image<I>& ima_, const Image<J>& data_)
      {
        mln_trace("data::impl::fill_with_image_fast");

        I& ima               = exact(ima_);
        const J& data        = exact(data_);

        data::internal::fill_with_image_tests(ima, data);

        mln_pixter(I) pi(ima);
        mln_pixter(const J) d(data);

        d.start();
        for_all(pi)
        {
          pi.val() = d.val();
          d.next();
        }
      }

      template <typename I, typename J>
      inline
      void fill_with_image_lines(Image<I>& ima_, const Image<J>& data_)
      {
	mln_trace("data::impl::fill_with_image_lines");

        I& ima        = exact(ima_);
        const J& data = exact(data_);

        data::internal::fill_with_image_tests(ima, data);

	mln_box_runstart_piter(I) p(ima.domain());
	for_all(p)
	{
          pixel<I> dst(ima, p);
	  memcpy_(dst, make::pixel(data, p), p.run_length());
        }

      }

      template <typename I, typename J>
      void fill_with_image_singleton(Image<I>& ima_,
                                     const Image<J>& data_)
      {
        mln_trace("data::impl::fill_with_image_singleton");

        const J& data  = exact(data_);
        data::internal::fill_with_image_tests(ima_, data);

        data::fill_with_value(ima_, opt::value(data));

      }

    } // end of namespace mln::data::impl



    // Namespace dispatch
    namespace internal
    {

      template <typename I, typename J>
      inline
      void fill_with_image_(mln::trait::image::value_access::direct,
                            mln::trait::image::value_access::direct,
                            mln::trait::image::ext_domain::some,
                            mln::trait::image::ext_domain::some,
                            Image<I>& ima_,
                            const Image<J>& data_)
      {
        I& ima        = exact(ima_);
        const J& data = exact(data_);

        if (sizeof(mln_value(I)) == sizeof(mln_value(J)))
        {
          if (border::get(ima) == border::get(data) &&
              ima.domain() == data.domain())
            impl::fill_with_image_fastest(ima, data);
          else
            impl::fill_with_image_lines(ima, data);
        }
        else
          impl::fill_with_image_fast(ima, data);
      }

      template <typename I, typename J>
      inline
      void fill_with_image_(mln::trait::image::value_access::direct,
                            mln::trait::image::value_access::direct,
                            mln::trait::image::ext_domain::none,
                            mln::trait::image::ext_domain::none,
                            Image<I>& ima_,
                            const Image<J>& data_)
      {
        I& ima        = exact(ima_);
        const J& data = exact(data_);

        if (sizeof(mln_value(I)) == sizeof(mln_value(J)))
        {
          if (ima.domain() == data.domain())
            impl::fill_with_image_fastest(ima, data);
          else
            impl::fill_with_image_lines(ima, data);
        }
        else
          impl::fill_with_image_fast(ima, data);
      }


      template <typename I, typename J>
      inline
      void fill_with_image_(mln::trait::image::value_access::any,
                            mln::trait::image::value_access::any,
                            mln::trait::image::ext_domain::any,
                            mln::trait::image::ext_domain::any,
                            Image<I>& ima,
                            const Image<J>& data)
      {
        impl::generic::fill_with_image(ima, data);
      }

      template <typename I, typename J>
      inline
      void fill_with_image_(trait::image::value_storage::any,
                            trait::image::value_storage::any,
                            Image<I>& ima,
                            const Image<J>& data)
      {
        impl::generic::fill_with_image(ima, data);
      }

      template <typename I, typename J>
      inline
      void fill_with_image_(trait::image::value_storage::any,
                            trait::image::value_storage::singleton,
                            Image<I>& ima,
                            const Image<J>& data)
      {
        impl::fill_with_image_singleton(ima, data);
      }

      template <typename I, typename J>
      inline
      void fill_with_image_(mln::trait::image::value_storage::one_block,
                            mln::trait::image::value_storage::one_block,
                            Image<I>& ima_,
                            const Image<J>& data_)
      {
        I& ima        = exact(ima_);
        const J& data = exact(data_);


        /// Check basic properties
        if (mlc_is(mln_trait_image_value_alignment(I),
                   trait::image::value_alignment::with_grid)::value &&
            mlc_is(mln_trait_image_value_alignment(J),
                   trait::image::value_alignment::with_grid)::value)
          {
            fill_with_image_(mln_trait_image_value_access(I)(),
                             mln_trait_image_value_access(J)(),
                             mln_trait_image_ext_domain(I)(),
                             mln_trait_image_ext_domain(J)(),
                             ima, data);
          }
        else
          impl::generic::fill_with_image(ima, data);
      }


      template <typename I, typename J>
      inline
      void fill_with_image_(Image<I>& ima, const Image<J>& data)
      {
        fill_with_image_(mln_trait_image_value_storage(I)(),
                         mln_trait_image_value_storage(J)(),
                         ima, data);
      }

    } // end of namespace mln::data::internal

  } // end of namespace mln::data

} // end of namespace mln

# endif // ! MLN_INCLUDE_ONLY


#endif // ! MLN_DATA_FILL_WITH_IMAGE_SPE_HH
