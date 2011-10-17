// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_BINARIZATION_NIBLACK_HH
# define SCRIBO_BINARIZATION_NIBLACK_HH

/// \file
///
///

# include <mln/core/concept/image.hh>
# include <mln/data/transform.hh>
# include <mln/value/int_u8.hh>

# include <scribo/binarization/niblack_threshold.hh>
# include <scribo/binarization/local_threshold.hh>
# include <scribo/binarization/internal/local_threshold_debug.hh>

# ifdef SCRIBO_LOCAL_THRESHOLD_DEBUG
#  include <mln/io/pgm/save.hh>
#  include <mln/io/pbm/save.hh>
#  include <mln/data/saturate.hh>
# endif // ! SCRIBO_LOCAL_THRESHOLD_DEBUG

namespace scribo
{

  namespace binarization
  {

    using namespace mln;


    /*! \brief Convert an image into a binary image.

      \input[in]  input       An image.
      \input[in]  window_size The window size.
      \input[in]  K           Niblack's formulae constant.

      \return A binary image.

     */
    template <typename I>
    mln_ch_value(I, bool)
    niblack(const Image<I>& input, unsigned window_size, double K);



    /*! \brief Convert an image into a binary image.

      Niblack's formulae constant K is set to 0.34.

      \input[in]  input       An image.
      \input[in]  window_size The window size.

      \return A binary image.

     */
    template <typename I>
    mln_ch_value(I, bool)
    niblack(const Image<I>& input, unsigned window_size);


    /// \overload
    /// The window size is set to 11.
    //
    template <typename I>
    mln_ch_value(I, bool)
    niblack(const Image<I>& input);


# ifndef MLN_INCLUDE_ONLY


    // Implementations.

    namespace impl
    {

      namespace generic
      {

	template <typename I>
	mln_ch_value(I, bool)
	niblack(const Image<I>& input, unsigned window_size, double K)
	{
	  trace::entering("scribo::binarization::impl::generic::niblack");
	  mln_precondition(exact(input).is_valid());

	  mln_ch_value(I,value::int_u8)
	    threshold_image = binarization::niblack_threshold(input, window_size, K);

	  mln_ch_value(I, bool)
	    output = local_threshold(input, threshold_image);

	  trace::exiting("scribo::binarization::impl::generic::niblack");
	  return output;
	}

      } // end of namespace scribo::binarization::impl::generic


    } // end of namespace scribo::binarization::impl



    // Dispatch

    namespace internal
    {

      template <typename I>
      mln_ch_value(I, bool)
	niblack_dispatch(const mln_value(I)&,
			 const Image<I>& input, unsigned window_size,
			 double K)
      {
	return impl::generic::niblack(input, window_size, K);
      }


      template <typename I>
      mln_ch_value(I, bool)
      niblack_dispatch(const Image<I>& input, unsigned window_size,
		       double K)
      {
	typedef mln_value(I) V;
	return niblack_dispatch(V(), input, window_size, K);
      }

    } // end of namespace scribo::binarization::internal



    // Facades

    template <typename I>
    mln_ch_value(I, bool)
      niblack(const Image<I>& input, unsigned window_size, double K)
    {
      trace::entering("scribo::binarization::niblack");

      mln_precondition(exact(input).is_valid());

      mln_ch_value(I, bool)
	output = internal::niblack_dispatch(input, window_size, K);

# ifdef SCRIBO_LOCAL_THRESHOLD_DEBUG
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
# endif // ! SCRIBO_LOCAL_THRESHOLD_DEBUG


      trace::exiting("scribo::binarization::niblack");
      return output;
    }


    template <typename I>
    mln_ch_value(I, bool)
    niblack(const Image<I>& input, unsigned window_size)
    {
      return niblack(input, window_size, SCRIBO_DEFAULT_NIBLACK_K);
    }


    template <typename I>
    mln_ch_value(I, bool)
    niblack(const Image<I>& input)
    {
      return niblack(input, 11);
    }


# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace scribo::binarization

} // end of namespace scribo


#endif // ! SCRIBO_BINARIZATION_NIBLACK_HH
