// Copyright (C) 2009, 2010, 2012 EPITA Research and Development
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

#ifndef MLN_DATA_PASTE_WITHOUT_LOCALIZATION_HH
# define MLN_DATA_PASTE_WITHOUT_LOCALIZATION_HH

/// \file
///
/// Paste the contents of an image into another one without taking
/// into account the localization of sites.

# include <mln/core/concept/image.hh>
# include <mln/core/box_runstart_piter.hh>

# include <mln/border/get.hh>

namespace mln
{

  namespace data
  {

    /*!\brief Paste the contents of image \p input into the image \p
     * output without taking into account the localization of sites.
     *
     * \param[in] input The input image providing pixels values.
     * \param[in,out] output The image in which values are
     * assigned.
     *
     * \ingroup mlndata
     */
    template <typename I, typename J>
    void paste_without_localization(const Image<I>& input, Image<J>& output);



# ifndef MLN_INCLUDE_ONLY


    // Tests

    namespace internal
    {


	template <typename I, typename J>
	inline
	void paste_without_localization_tests(const Image<I>& input,
					      Image<J>& output)
	{
	  mlc_converts_to(mln_value(I), mln_value(J))::check();

	  (void) input;
	  (void) output;
	}

    } // end of namespace mln::data::internal



    // Implementations


    namespace impl
    {

      namespace generic
      {

	template <typename I, typename J>
	inline
	void paste_without_localization(const Image<I>& input_,
					Image<J>& output_)
	{
	  mln_trace("data::impl::generic::paste_without_localization");

	  internal::paste_without_localization_tests(input_, output_);

	  const I& input = exact(input_);
	  J& output = exact(output_);

	  mln_fwd_piter(I) pi(input.domain());
	  pi.start();
	  mln_fwd_piter(J) po(output.domain());
	  po.start();
	  while (pi.is_valid() && po.is_valid())
	  {
	    output(po) = input(pi);
	    pi.next();
	    po.next();
	  }

	}


      } // end of namespace mln::data::impl::generic



      /*! \brief Paste data to an image without using localization.
	Performs a one-block memory copy.

	\p input and \p output must have both the following properties:

	- mln::trait::image::value_alignment::with_grid
	- mln::trait::image::value_storage::one_block
	- mln::trait::image::value_access::direct
	- mln::trait::image::ext_domain::some


	They must also fulfill the following conditions:

	- Same border size.
	- Same domain size.
	- Same value type.

       */
      template <typename I, typename J>
      inline
      void paste_without_localization_fastest(const Image<I>& input_,
					      Image<J>& output_)
      {
	mln_trace("data::impl::paste_without_localization_fastest");

	internal::paste_without_localization_tests(input_, output_);

	const I& input = exact(input_);
	J& output = exact(output_);

	typedef mln_value(I) V;
	memcpy(output.buffer(), input.buffer(), input.nelements() * sizeof(V));

      }



      /*! \brief Paste data to an image without using localization.
	Performs a line-per-line memory copy.

	\p input and \p output must have both the following properties:

	- mln::trait::image::value_alignment::with_grid
	- mln::trait::image::value_storage::one_block
	- mln::trait::image::value_access::direct
	- mln::trait::image::ext_domain::some


	They must also fulfill the following conditions:

	- Same domain size.
	- Same value type.

       */
      template <typename I, typename J>
      inline
      void paste_without_localization_lines(const Image<I>& input_,
					    Image<J>& output_)
      {
	mln_trace("data::impl::paste_without_localization_fastest");

	internal::paste_without_localization_tests(input_, output_);

	const I& input = exact(input_);
	J& output = exact(output_);

	box_runstart_piter<mln_site(I)> pi(input.domain());
	box_runstart_piter<mln_site(J)> po(output.domain());

	typedef mln_value(I) V;

	for_all_2(pi, po)
	  memcpy(&output(po), &input(pi), input.ncols() * sizeof(V));

      }



      /*! \brief Paste data to an image without using localization.
	Performs a point-wise copy.

	\p input and \p output must have both the following properties:

	- mln::trait::image::value_alignment::with_grid
	- mln::trait::image::value_storage::one_block
	- mln::trait::image::value_access::direct
	- mln::trait::image::ext_domain::some


	They must also fulfill the following conditions:

	- Same domain size.

       */
      template <typename I, typename J>
      inline
      void paste_without_localization_fast(const Image<I>& input_,
					   Image<J>& output_)
      {
	mln_trace("data::impl::paste_without_localization_fast");

	internal::paste_without_localization_tests(input_, output_);

	const I& input = exact(input_);
	J& output = exact(output_);

        mln_pixter(const I) pi(input);
        mln_pixter(J) po(output);

        for_all_2(pi, po)
          po.val() = pi.val();

      }



    } // end of namespace mln::data::impl



    // Dispatch

    namespace internal
    {

      template <typename I, typename J>
      inline
      void paste_without_localization_dispatch(
	mln::trait::image::value_access::direct,
	mln::trait::image::value_access::direct,
	mln::trait::image::ext_domain::some,
	mln::trait::image::ext_domain::some,
	const I& input,
	J& output)
      {
        if (sizeof(mln_value(I)) == sizeof(mln_value(J)))
        {
          if (border::get(input) == border::get(output)
	      && input.domain() == output.domain())
            impl::paste_without_localization_fastest(input, output);
          else
            impl::paste_without_localization_lines(input, output);
        }
        else
          impl::paste_without_localization_fast(input, output);

      }


      template <typename I, typename J>
      inline
      void paste_without_localization_dispatch(
	mln::trait::image::value_access::any,
	mln::trait::image::value_access::any,
	mln::trait::image::ext_domain::any,
	mln::trait::image::ext_domain::any,
	const I& input,
	J& output)
      {
	impl::generic::paste_without_localization(input, output);
      }


      template <typename I, typename J>
      inline
      void paste_without_localization_dispatch(
	mln::trait::image::value_storage::any,
	mln::trait::image::value_storage::any,
	const Image<I>& input,
	Image<J>& output)
      {
	impl::generic::paste_without_localization(input, output);
      }


      template <typename I, typename J>
      inline
      void paste_without_localization_dispatch(
	mln::trait::image::value_storage::one_block,
	mln::trait::image::value_storage::one_block,
	const Image<I>& input_,
	Image<J>& output_)
      {
        const I& input  = exact(input_);
        J&       output = exact(output_);


        /// Check basic properties
        if (mlc_is(mln_trait_image_value_alignment(I),
                   trait::image::value_alignment::with_grid)::value &&
            mlc_is(mln_trait_image_value_alignment(J),
                   trait::image::value_alignment::with_grid)::value)
          {
            paste_without_localization_dispatch(
	      mln_trait_image_value_access(I)(),
	      mln_trait_image_value_access(J)(),
	      mln_trait_image_ext_domain(I)(),
	      mln_trait_image_ext_domain(J)(),
	      input, output);
          }
        else
          impl::generic::paste_without_localization(input, output);
      }


      template <typename I, typename J>
      inline
      void paste_without_localization_dispatch(const Image<I>& input,
					       Image<J>& output)
      {
        paste_without_localization_dispatch(mln_trait_image_value_storage(I)(),
					    mln_trait_image_value_storage(J)(),
					    input, output);
      }

    } // end of namespace mln::data::internal




    // Facade

    template <typename I, typename J>
    inline
    void paste_without_localization(const Image<I>& input, Image<J>& output)
    {
      mln_trace("data::paste_without_localization");

      internal::paste_without_localization_tests(input, output);

      internal::paste_without_localization_dispatch(input, output);

    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::data

} // end of namespace mln



#endif // ! MLN_DATA_PASTE_WITHOUT_LOCALIZATION_HH
