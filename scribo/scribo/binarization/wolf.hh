// Copyright (C) 2012, 2013 EPITA Research and Development Laboratory
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

#ifndef SCRIBO_BINARIZATION_WOLF_HH
# define SCRIBO_BINARIZATION_WOLF_HH

/// \file
///
///

# include <mln/core/concept/image.hh>
# include <scribo/binarization/internal/wolf_functor.hh>
# include <scribo/binarization/internal/local_threshold_core.hh>
# include <scribo/binarization/internal/global_max_stddev_functor.hh>
# include <scribo/util/integral_sum_sum2_global_min_functor.hh>

namespace scribo
{

  namespace binarization
  {

    using namespace mln;


    /*! \brief Convert an image into a binary image.

      \param[in]  input       An image.
      \param[in]  window_size The window size.
      \param[in]  K           Wolf's formulae constant.

      \return A binary image.

      This implementation is based on article "Text Localization,
      Enhancement and Binarization in Multimedia Documents", Christian
      Wolf, Jean-Michel Jolion, Françoise Chassaing, ICPR 2002.

      \ingroup grpalgobinwolf
     */
    template <typename I>
    mln_ch_value(I, bool)
    wolf(const Image<I>& input, unsigned window_size, double K);



    /*! \brief Convert an image into a binary image.

      Wolf's formulae constant K is set to 0.34.

      \param[in]  input       An image.
      \param[in]  window_size The window size.

      \return A binary image.

      \ingroup grpalgobinwolf
     */
    template <typename I>
    mln_ch_value(I, bool)
    wolf(const Image<I>& input, unsigned window_size);


    /*! \overload
      The window size is set to 11.

      \ingroup grpalgobinwolf
    */
    template <typename I>
    mln_ch_value(I, bool)
    wolf(const Image<I>& input);


# ifndef MLN_INCLUDE_ONLY

    // Facades

    template <typename I>
    mln_ch_value(I, bool)
      wolf(const Image<I>& input, unsigned window_size, double K)
    {
      mln_trace("scribo::binarization::wolf");

      mln_precondition(exact(input).is_valid());

      int integral_size_ratio = internal::wolf_functor<I>::step;

      // Make sure the image sizes are a multiple of 3 in each
      // dimension. (browsing while binarizing relies on that
      // property).
      mln::util::array<mln::util::couple<box2d, unsigned> >
	sub_domains = scribo::util::compute_sub_domains(input, 1, integral_size_ratio);

      border::adjust(input, sub_domains(1).second());
      border::mirror(input);

      scribo::util::integral_sum_sum2_global_min_functor<value::int_u8, double> fi;
      image2d<mln::util::couple<double,double> >
	integral = scribo::util::init_integral_image(input, integral_size_ratio, fi,
						     sub_domains[2].first(),
						     sub_domains[2].second());

      window_size /= integral_size_ratio;
      if (window_size % 2)
	window_size += 2;
      else
	window_size += 1;


      // Compute max(stddev) of all windows.
      internal::global_max_stddev<I> f_max_stddev;
      scribo::canvas::integral_browsing(integral, 1, window_size,
					window_size, integral_size_ratio, f_max_stddev);

      // Binarize !
      internal::wolf_functor<I>
	f(input, K, fi.global_min(), f_max_stddev.max_stddev);
      scribo::canvas::integral_browsing(integral, 1, window_size,
					window_size, integral_size_ratio, f);

      return f.output;
    }


    template <typename I>
    mln_ch_value(I, bool)
    wolf(const Image<I>& input, unsigned window_size)
    {
      return wolf(input, window_size, SCRIBO_DEFAULT_WOLF_K);
    }


    template <typename I>
    mln_ch_value(I, bool)
    wolf(const Image<I>& input)
    {
      return wolf(input, 11);
    }


# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace scribo::binarization

} // end of namespace scribo


#endif // ! SCRIBO_BINARIZATION_WOLF_HH
