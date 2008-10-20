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

#ifndef MLN_LEVEL_PASTE_SPE_HH
# define MLN_LEVEL_PASTE_SPE_HH

/*! \file mln/level/paste.spe.hh
 *
 * \brief Specializations for mln::level::paste.
 *
 * \todo Rewrite the dispatch!!!
 */

# ifndef MLN_LEVEL_PASTE_HH
#  error "Forbidden inclusion of *.spe.hh"
# endif // ! MLN_LEVEL_PASTE_HH

# include <mln/level/memcpy_.hh>



# ifndef MLN_INCLUDE_ONLY

namespace mln
{

  namespace level
  {

    namespace internal
    {


      template <typename I, typename J>
      inline
      void paste_tests(const Image<I>& input, Image<J>& output);
    }

    namespace impl
    {

      namespace generic
      {
	template <typename I, typename J>
	void paste_(const Image<I>& input, Image<J>& output);
      }



//       template <typename I, typename J>
//       inline
//       void paste_lines_(const I& data, J& destination)
//       {
// 	trace::entering("level::impl::paste_lines_");
// 	typedef mln_psite(I) P;
// 	std::size_t n = data.bbox().len(P::dim - 1);
// 	// FIXME: Works only for raw data images
// 	// FIXME: For linear data images, we should get the len for each line...

// 	typename I::line_piter p(data.domain()); // FIXME: Alias mln_line_piter!
// 	//	mln_line_piter(I) p(data.domain());
// 	for_all(p)
// 	{
//           FIXME: TYPE pix = make::pixel(destination, p);
// 	  memcpy_(pix,
// 		  make::pixel(data, p),
// 		  n);
// 	  }
// 	trace::exiting("level::impl::paste_lines_");
//       }


      template <typename I, typename J>
      void paste_fast(const Image<I>& input_, Image<J>& output_)
      {
        trace::entering("level::impl::paste_fast");

        const I& input = exact(input_);
        J& output      = exact(output_);

        level::internal::paste_tests(input, output);

        mln_pixter(const I) pi(input);
        mln_pixter(J) po(output);

        po.start();
        for_all(pi)
        {
          po.val() = pi.val();
        }
        trace::entering("level::impl::paste_fast");
      }

    } // end of namespace impl.


    // Dispatch.
    namespace internal
    {


      template <typename I, typename J>
      inline
      void paste_(trait::image::value_storage::any,
                  trait::image::value_storage::any,
                  const Image<I>& input,
                  Image<J>& output)
      {
        impl::generic::paste_(input, output);
      }

      template <typename I, typename J>
      inline
      void paste_(trait::image::value_storage::one_block,
                  trait::image::value_storage::one_block,
                  const Image<I>& input_,
                  Image<J>& output_)
      {
        const I& input  = exact(input_);
        J& output       = exact(output_);

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
                 input.border() == output.border()) ||
                (mlc_is(mln_trait_image_ext_domain(I),
                        trait::image::ext_domain::none)::value &&
                 mlc_is(mln_trait_image_ext_domain(J),
                        trait::image::ext_domain::none)::value))
              {
                /// Check domain
                if (input.domain() == output.domain())
                  impl::paste_fast(input, output);
              }
          }
        impl::generic::paste_(input, output);
      }


      template <typename I, typename J>
      inline
      void paste_(const Image<I>& input, Image<J>& output)
      {
        paste_(mln_trait_image_value_storage(I)(),
               mln_trait_image_value_storage(J)(),
               input, output);
      }

    } // end of namespace mln::level::internal

  } // end of namespace mln::level

} // end of namespace mln

# endif // ! MLN_INCLUDE_ONLY


#endif // ! MLN_LEVEL_PASTE_HH
