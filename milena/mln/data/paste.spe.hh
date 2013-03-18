// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
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

#ifndef MLN_DATA_PASTE_SPE_HH
# define MLN_DATA_PASTE_SPE_HH

/// \file
///
/// Specializations for mln::data::paste.

# ifndef MLN_DATA_PASTE_HH
#  error "Forbidden inclusion of *.spe.hh"
# endif // ! MLN_DATA_PASTE_HH

# include <mln/core/pixel.hh>
# include <mln/data/fill_with_value.hh>
# include <mln/data/memcpy_.hh>
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
      void paste_tests(const Image<I>& input, Image<J>& output);
    }

    namespace impl
    {

      namespace generic
      {
	// Forward declaration.
	template <typename I, typename J>
	void paste(const Image<I>& input, Image<J>& output);
      }


      template <typename I, typename J>
      void paste_fast(const Image<I>& input_, Image<J>& output_)
      {
        mln_trace("data::impl::paste_fast");

        const I& input = exact(input_);
        J& output      = exact(output_);

        data::internal::paste_tests(input, output);

        mln_pixter(const I) pi(input);
        mln_pixter(J) po(output);

        po.start();
        for_all(pi)
        {
          po.val() = static_cast<mln_value(J)>(pi.val());
          po.next();
        }
      }

      template <typename I, typename J>
      void paste_fastest(const Image<I>& input_, Image<J>& output_)
      {
        mln_trace("data::impl::paste_fastest");

        const I& input = exact(input_);
        J& output      = exact(output_);

        data::internal::paste_tests(input, output);

        pixel<const I> src (input);
        pixel<J> dst(output);
        *(src.address_()) = input.buffer();
        *(dst.address_()) = output.buffer();

        memcpy_(dst, src, opt::nelements(input));

      }

      template <typename I, typename J>
      inline
      void paste_lines(const Image<I>& input_, Image<J>& output_)
      {
	mln_trace("data::impl::paste_lines");

        const I& input = exact(input_);
        J& output      = exact(output_);

        data::internal::paste_tests(input, output);

	mln_box_runstart_piter(I) p(input.domain());
	for_all(p)
	{
          pixel<J> dst(output, p);
	  memcpy_(dst, make::pixel(input, p), p.run_length());
        }

      }

      template <typename I, typename J>
      void paste_singleton(const Image<I>& input_, Image<J>& output_)
      {
        mln_trace("data::impl::paste_singleton");

        const I& input  = exact(input_);

        data::fill_with_value((output_ | input.domain()).rw(),
                              opt::value(input));

      }

    } // end of namespace impl.


    // Dispatch.

    namespace internal
    {

      template <typename I, typename J>
      inline // Direct access and extension.
      void paste_direct_ext_dispatch(metal::true_, // Same value size.
				     const I& input,
				     J& output)
      {
	if (input.domain() == output.domain()
	    && border::get(input) == border::get(output))
	  impl::paste_fastest(input, output);
	else
	  impl::paste_lines(input, output);
      }

      template <typename I, typename J>
      inline // Direct access and extension.
      void paste_direct_ext_dispatch(metal::false_, // Different value size.
				     const I& input,
				     J& output)
      {
	impl::paste_fast(input, output);
      }

      template <typename I, typename J>
      inline
      void paste_dispatch(trait::image::value_access::direct,
			  trait::image::value_access::direct,
			  trait::image::ext_domain::some,
			  trait::image::ext_domain::some,
			  const Image<I>& input,
			  Image<J>& output)
      {
	enum { same_size = sizeof(mln_value(I)) == sizeof(mln_value(J)) };
	paste_direct_ext_dispatch(metal::bool_<same_size>(),
				  exact(input), exact(output));
      }

      template <typename I, typename J>
      inline // Direct access and no extension.
      void paste_direct_noext_dispatch(metal::true_, // Same value size.
				       const I& input,
				       J& output)
      {
	if (input.domain() == output.domain())
	  impl::paste_fastest(input, output);
	else
	  impl::paste_lines(input, output);
      }

      template <typename I, typename J>
      inline // Direct access and no extension.
      void paste_direct_noext_dispatch(metal::false_, // Different value size.
				       const I& input,
				       J& output)
      {
	impl::paste_fast(input, output);
      }

      template <typename I, typename J>
      inline
      void paste_dispatch(trait::image::value_access::direct,
			  trait::image::value_access::direct,
			  trait::image::ext_domain::none,
			  trait::image::ext_domain::none,
			  const Image<I>& input,
			  Image<J>& output)
      {
	enum { same_size = sizeof(mln_value(I)) == sizeof(mln_value(J)) };
	paste_direct_noext_dispatch(metal::bool_<same_size>(),
				    exact(input), exact(output));
      }

      template <typename I, typename J>
      inline
      void paste_dispatch(trait::image::value_access::any,
			  trait::image::value_access::any,
			  trait::image::ext_domain::any,
			  trait::image::ext_domain::any,
			  const Image<I>& input,
			  Image<J>& output)
      {
        impl::generic::paste(input, output);
      }


      // Dispatch w.r.t. "value storage".

      template <typename I, typename J>
      inline
      void paste_dispatch(trait::image::value_storage::any,
			  trait::image::value_storage::any,
			  const Image<I>& input,
			  Image<J>& output)
      {
        impl::generic::paste(input, output);
      }

      template <typename I, typename J>
      inline
      void paste_dispatch(trait::image::value_storage::singleton,
			  trait::image::value_storage::any,
			  const Image<I>& input,
			  Image<J>& output)
      {
        impl::paste_singleton(input, output);
      }


      template <typename I, typename J>
      inline
      void paste_one_block_dispatch(metal::true_, // One-block values and "grid-aligned".
				    const Image<I>& input,
				    Image<J>& output)
      {
	paste_dispatch(mln_trait_image_value_access(I)(),
		       mln_trait_image_value_access(J)(),
		       mln_trait_image_ext_domain(I)(),
		       mln_trait_image_ext_domain(J)(),
		       input, output);
      }

      template <typename I, typename J>
      inline
      void paste_one_block_dispatch(metal::false_, // One-block values but not "grid-aligned".
				    const Image<I>& input,
				    Image<J>& output)
      {
	impl::generic::paste(input, output);
      }

      template <typename I, typename J>
      inline
      void paste_dispatch(trait::image::value_storage::one_block,
			  trait::image::value_storage::one_block,
			  const Image<I>& input,
			  Image<J>& output)
      {
	enum { alignments = mlc_is(mln_trait_image_value_alignment(I),
				   trait::image::value_alignment::with_grid)::value
	       && mlc_is(mln_trait_image_value_alignment(J),
			 trait::image::value_alignment::with_grid)::value };
	paste_one_block_dispatch(metal::bool_<alignments>(), input, output);
      }


      // Dispatch entry.

      template <typename I, typename J>
      inline
      void paste_dispatch(const Image<I>& input, Image<J>& output)
      {

        paste_dispatch(mln_trait_image_value_storage(I)(),
		       mln_trait_image_value_storage(J)(),
		       input, output);
      }

    } // end of namespace mln::data::internal

  } // end of namespace mln::data

} // end of namespace mln

# endif // ! MLN_INCLUDE_ONLY


#endif // ! MLN_DATA_PASTE_SPE_HH
