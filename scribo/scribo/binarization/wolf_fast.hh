// Copyright (C) 2012 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_BINARIZATION_WOLF_FAST_HH
# define SCRIBO_BINARIZATION_WOLF_FAST_HH

/// \file
///
///

# include <mln/core/concept/image.hh>
# include <scribo/binarization/internal/wolf_functor_fast.hh>
# include <scribo/binarization/internal/local_threshold_core.hh>
# include <scribo/util/integral_sum_sum2_global_min_functor.hh>

namespace scribo
{

  namespace binarization
  {

    using namespace mln;


    /*! \brief Convert an image into a binary image.

      \input[in]  input       An image.
      \input[in]  window_size The window size.
      \input[in]  K           Wolf's formulae constant.

      \return A binary image.

      This implementation is based on article "Text Localization,
      Enhancement and Binarization in Multimedia Documents", Christian
      Wolf, Jean-Michel Jolion, Françoise Chassaing, ICPR 2002.

      This implementation gives an approximation of the results.  It
      is faster than the original implementation thanks to the use of
      integral images.

     */
    template <typename I>
    mln_ch_value(I, bool)
    wolf_fast(const Image<I>& input, unsigned window_size, double K);



    /*! \overload

      Wolf's formulae constant K is set to 0.34.

     */
    template <typename I>
    mln_ch_value(I, bool)
    wolf_fast(const Image<I>& input, unsigned window_size);


    /// \overload
    /// The window size is set to 11.
    //
    template <typename I>
    mln_ch_value(I, bool)
    wolf_fast(const Image<I>& input);


# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {

      template <typename I>
      struct global_max_stddev
      {
	global_max_stddev()
	  : max_stddev(0)
	{
	}

	void init()
	{
	}

	// Run every 4 pixels.
	void exec(double mean, double stddev)
	{
	  (void) mean;
	  if (max_stddev < stddev)
	    max_stddev = stddev;
	}

	void end_of_row(int)
	{
	}

	void finalize()
	{
	}

	double max_stddev;
      };

    } // end of namespace scribo::binarization::internal


    // Facades

    template <typename I>
    mln_ch_value(I, bool)
      wolf_fast(const Image<I>& input, unsigned window_size, double K)
    {
      mln_trace("scribo::binarization::wolf_fast");

      mln_precondition(exact(input).is_valid());


      // Make sure the image sizes are a multiple of 3 in each
      // dimension. (browsing while binarizing relies on that
      // property).
      mln::util::array<mln::util::couple<box2d, unsigned> >
	sub_domains = scribo::util::compute_sub_domains(input, 1, 3);

      border::adjust(input, sub_domains(1).second());
      border::mirror(input);

      scribo::util::integral_sum_sum2_global_min_functor<value::int_u8, double> fi;
      image2d<mln::util::couple<double,double> >
	integral = scribo::util::init_integral_image(input, 3, fi,
						     sub_domains[2].first(),
						     sub_domains[2].second());

      window_size /= 3;
      if (window_size % 2)
	window_size += 2;
      else
	window_size += 1;


      // Compute max(stddev) of all windows.
      internal::global_max_stddev<I> f_max_stddev;
      scribo::canvas::integral_browsing(integral, 1, window_size,
					window_size, 3, f_max_stddev);

      // Binarize !
      internal::wolf_functor_fast<I>
	f(input, K, fi.global_min(), f_max_stddev.max_stddev);
      scribo::canvas::integral_browsing(integral, 1, window_size,
					window_size, 3, f);

      return f.output;
    }


    template <typename I>
    mln_ch_value(I, bool)
    wolf_fast(const Image<I>& input, unsigned window_size)
    {
      return wolf_fast(input, window_size, SCRIBO_DEFAULT_WOLF_K);
    }


    template <typename I>
    mln_ch_value(I, bool)
    wolf_fast(const Image<I>& input)
    {
      return wolf_fast(input, 11);
    }


# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace scribo::binarization

} // end of namespace scribo


#endif // ! SCRIBO_BINARIZATION_WOLF_FAST_HH
