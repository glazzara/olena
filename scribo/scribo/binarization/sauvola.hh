// Copyright (C) 2009, 2010 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef SCRIBO_BINARIZATION_SAUVOLA_HH
# define SCRIBO_BINARIZATION_SAUVOLA_HH

/// \file
///
///

# include <mln/core/concept/image.hh>
# include <mln/data/transform.hh>
# include <mln/value/int_u8.hh>

# include <scribo/binarization/sauvola_threshold_image.hh>
# include <scribo/binarization/local_threshold.hh>
# include <scribo/binarization/internal/sauvola_debug.hh>

# ifdef SCRIBO_SAUVOLA_DEBUG
#  include <mln/io/pgm/save.hh>
#  include <mln/io/pbm/save.hh>
#  include <mln/data/saturate.hh>
# endif // ! SCRIBO_SAUVOLA_DEBUG

namespace scribo
{

  namespace binarization
  {

    using namespace mln;


    /*! \brief Convert an image into a binary image.

      \input[in]  input       An image.
      \input[in]  window_size The window size.
      \input[in]  K           Sauvola's formulae constant.

      \return A binary image.

     */
    template <typename I>
    mln_ch_value(I, bool)
    sauvola(const Image<I>& input, unsigned window_size, double K);



    /*! \brief Convert an image into a binary image.

      Sauvola's formulae constant K is set to 0.34.

      \input[in]  input       An image.
      \input[in]  window_size The window size.

      \return A binary image.

     */
    template <typename I>
    mln_ch_value(I, bool)
    sauvola(const Image<I>& input, unsigned window_size);


    /// \overload
    /// The window size is set to 11.
    //
    template <typename I>
    mln_ch_value(I, bool)
    sauvola(const Image<I>& input);


# ifndef MLN_INCLUDE_ONLY


    // Implementations.

    namespace impl
    {

      namespace generic
      {

	template <typename I>
	mln_ch_value(I, bool)
	sauvola(const Image<I>& input, unsigned window_size, double K)
	{
	  trace::entering("scribo::binarization::impl::generic::sauvola");
	  mln_precondition(exact(input).is_valid());

	  mln_ch_value(I, bool)
	    output = local_threshold(input,
				     binarization::sauvola_threshold_image(input,
									   window_size,
									   K));

	  trace::exiting("scribo::binarization::impl::generic::sauvola");
	  return output;
	}

      } // end of namespace scribo::binarization::impl::generic


    } // end of namespace scribo::binarization::impl



    // Dispatch

    namespace internal
    {

      template <typename I>
      mln_ch_value(I, bool)
	sauvola_dispatch(const mln_value(I)&,
			 const Image<I>& input, unsigned window_size,
			 double K)
      {
	return impl::generic::sauvola(input, window_size, K);
      }


      template <typename I>
      mln_ch_value(I, bool)
      sauvola_dispatch(const Image<I>& input, unsigned window_size,
		       double K)
      {
	typedef mln_value(I) V;
	return sauvola_dispatch(V(), input, window_size, K);
      }

    } // end of namespace scribo::binarization::internal



    // Facades

    template <typename I>
    mln_ch_value(I, bool)
      sauvola(const Image<I>& input, unsigned window_size, double K)
    {
      trace::entering("scribo::binarization::sauvola");

      mln_precondition(exact(input).is_valid());

      mln_ch_value(I, bool)
	output = internal::sauvola_dispatch(input, window_size, K);

# ifdef SCRIBO_SAUVOLA_DEBUG
      if (internal::stddev_image_output)
	io::pgm::save(data::saturate(value::int_u8(), internal::debug_stddev),
		      internal::stddev_image_output);
      if (internal::mean_image_output)
	io::pgm::save(data::saturate(value::int_u8(), internal::debug_mean),
		      internal::mean_image_output);
      if (internal::threshold_image_output)
	io::pgm::save(data::saturate(value::int_u8(), internal::debug_threshold),
		      internal::threshold_image_output);

      if (internal::alpham_image_output)
	io::pgm::save(data::saturate(value::int_u8(), internal::debug_alpham),
		      internal::alpham_image_output);
      if (internal::alphacond_image_output)
	io::pbm::save(internal::debug_alphacond, internal::alphacond_image_output);
# endif // ! SCRIBO_SAUVOLA_DEBUG


      trace::exiting("scribo::binarization::sauvola");
      return output;
    }


    template <typename I>
    mln_ch_value(I, bool)
    sauvola(const Image<I>& input, unsigned window_size)
    {
      return sauvola(input, window_size, SCRIBO_DEFAULT_SAUVOLA_K);
    }


    template <typename I>
    mln_ch_value(I, bool)
    sauvola(const Image<I>& input)
    {
      return sauvola(input, 11);
    }


# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace scribo::binarization

} // end of namespace scribo


#endif // ! SCRIBO_BINARIZATION_SAUVOLA_HH
