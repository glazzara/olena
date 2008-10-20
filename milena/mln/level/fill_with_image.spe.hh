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

#ifndef MLN_LEVEL_FILL_WITH_IMAGE_SPE_HH
# define MLN_LEVEL_FILL_WITH_IMAGE_SPE_HH

/*! \file mln/level/fill_with_image.spe.hh
 *
 * \brief Specializations for mln::level::fill_with_image.
 *
 * \todo Add specializations then rewrite the dispatch.
 */

# ifndef MLN_LEVEL_FILL_WITH_IMAGE_HH
#  error "Forbidden inclusion of *.spe.hh"
# endif // ! MLN_LEVEL_FILL_WITH_IMAGE_HH



# ifndef MLN_INCLUDE_ONLY

namespace mln
{

  namespace level
  {

    namespace internal
    {

      template <typename I, typename J>
      inline
      void fill_with_image_tests(Image<I>& ima, const Image<J>& data);

    } // end of namespace mln::level::internal



    namespace impl
    {

      namespace generic
      {
	template <typename I, typename J>
	void fill_with_image(Image<I>& ima, const Image<J>& data);
      }

      template <typename I, typename J>
      void fill_with_image_fast(Image<I>& ima_, const Image<J>& data_)
      {
        trace::entering("level::impl::fill_with_image_fast");

        I& ima               = exact(ima_);
        const J& data        = exact(data_);

        level::internal::fill_with_image_tests(ima, data);

        mln_pixter(I) pi(ima);
        mln_pixter(const J) d(data);

        d.start();
        for_all(pi)
        {
          pi.val() = d.val();
        }
        trace::entering("level::impl::fill_with_image_fast");
      }
    } // end of namespace mln::level::impl



    // Namespace dispatch
    namespace internal
    {

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
      void fill_with_image_(trait::image::value_storage::one_block,
                            trait::image::value_storage::one_block,
                            Image<I>& ima_,
                            const Image<J>& data_)
      {
        I& ima        = exact(ima_);
        const J& data = exact(data_);

        /// Check basic properties
        if (mlc_is(mln_trait_image_value_access(J),
                   trait::image::value_access::direct)::value &&
            mlc_is(mln_trait_image_value_access(I),
                   trait::image::value_access::direct)::value &&
            mlc_is(mln_trait_image_value_alignement(I),
                   trait::image::value_alignement::with_grid)::value &&
            mlc_is(mln_trait_image_value_alignement(J),
                   trait::image::value_alignement::with_grid)::value)
          {
            /// Check ext_domain
            if (
                ((mlc_is(mln_trait_image_ext_domain(I),
                         trait::image::ext_domain::fixed)::value ||
                  mlc_is(mln_trait_image_ext_domain(I),
                         trait::image::ext_domain::extendable)::value) &&
                 (mlc_is(mln_trait_image_ext_domain(J),
                         trait::image::ext_domain::fixed)::value ||
                  mlc_is(mln_trait_image_ext_domain(J),
                         trait::image::ext_domain::extendable)::value) &&
                 data.border() == data.border()) ||
                (mlc_is(mln_trait_image_ext_domain(I),
                        trait::image::ext_domain::none)::value &&
                 mlc_is(mln_trait_image_ext_domain(J),
                        trait::image::ext_domain::none)::value))
              {
                /// Check domain
                if (data.domain() == data.domain())
                  impl::fill_with_image_fast(ima, data);
              }
          }
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

    } // end of namespace mln::level::internal

  } // end of namespace mln::level

} // end of namespace mln

# endif // ! MLN_INCLUDE_ONLY


#endif // ! MLN_LEVEL_FILL_WITH_IMAGE_HH
