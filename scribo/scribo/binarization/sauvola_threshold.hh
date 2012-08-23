// Copyright (C) 2009, 2010, 2011 EPITA Research and Development
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

#ifndef SCRIBO_BINARIZATION_SAUVOLA_THRESHOLD_HH
# define SCRIBO_BINARIZATION_SAUVOLA_THRESHOLD_HH

/// \file
///
/// Compute an image of local threshold using Sauvola algorithm.

/// \fixme return type too restrictive!

# include <algorithm>
# include <cmath>

# include <mln/core/image/image2d.hh>
# include <mln/value/int_u.hh>
# include <mln/value/int_u8.hh>

# include <mln/border/mirror.hh>
# include <mln/border/adjust.hh>

# include <scribo/core/init_integral_image.hh>
# include <scribo/binarization/internal/sauvola_formula.hh>
# include <scribo/binarization/internal/sauvola_threshold_functor.hh>

# include <scribo/util/init_integral_image.hh>
# include <scribo/util/integral_sum_sum2_functor.hh>
# include <scribo/util/compute_sub_domains.hh>

# include <scribo/canvas/integral_browsing.hh>



namespace scribo
{

  namespace binarization
  {

    using namespace mln;

    /*! \brief Compute an image of local threshold using Sauvola algorithm.

      \input[in]  input       A gray level image.
      \input[in]  window_size The window size.
      \input[out] simple      The sum of all intensities of \p input.
      \input[out] squared     The sum of all squared intensities of \p
                              input.

      \return An image of local thresholds.

     */
    template <typename I, typename J>
    mln_ch_value(I, value::int_u8)
    sauvola_threshold(const Image<I>& input, unsigned window_size,
			    double K);

    /// \overload
    /// K is set to 0.34
    template <typename I>
    mln_ch_value(I, value::int_u8)
    sauvola_threshold(const Image<I>& input, unsigned window_size);


    /// \overload
    /// The window size is set to 11.
    //
    template <typename I>
    mln_ch_value(I, value::int_u8)
    sauvola_threshold(const Image<I>& input);



# ifndef MLN_INCLUDE_ONLY


    // Implementation


    namespace impl
    {

      namespace generic
      {

	template <typename I>
	inline
	mln_concrete(I)
	sauvola_threshold(const Image<I>& input_, unsigned window_size,
			  double K)
	{
	  trace::entering("scribo::binarization::impl::generic::sauvola_threshold");

	  // Not implemented
	  mlc_abort(I)::check();

	  typedef mln_concrete(I) output_t;
	  trace::exiting("scribo::binarization::impl::generic::sauvola_threshold");
	  return output_t();
	}

      } // end of namespace scribo::binarization::impl::generic


      template <typename I>
      inline
      mln_concrete(I)
      sauvola_threshold_gl(const Image<I>& input_, unsigned window_size,
			   double K)
      {
	trace::entering("scribo::binarization::impl::sauvola_threshold_gl");

	const I& input = exact(input_);

	mln_precondition(exact(input).is_valid());

	// Make sure the image sizes are a multiple of 3 in each
	// dimension. (browsing while binarizing relies on that
	// property).
	mln::util::array<mln::util::couple<box2d, unsigned> >
	  sub_domains = scribo::util::compute_sub_domains(input, 1, 3);

	border::adjust(input, sub_domains(1).second());
	border::mirror(input);

	typedef mln::util::couple<double,double> V_i;
	scribo::util::integral_sum_sum2_functor<mln_value(I), double> fi;
	mln_ch_value(I,V_i)
	  integral = scribo::util::init_integral_image(input, 3, fi);

#  ifdef SCRIBO_LOCAL_THRESHOLD_DEBUG
	initialize(internal::debug_mean, input);
	initialize(internal::debug_stddev, input);

	initialize(internal::debug_threshold, input);
	initialize(internal::debug_alpham, input);
	initialize(internal::debug_alphacond, input);
#  endif // ! SCRIBO_LOCAL_THRESHOLD_DEBUG

	internal::sauvola_threshold_functor<I> f(input, K,
						 SCRIBO_DEFAULT_SAUVOLA_R);
	scribo::canvas::integral_browsing(integral, 1, window_size / 3,
					  window_size / 3, 3, f);

	trace::exiting("scribo::binarization::impl::sauvola_threshold_gl");
	return f.output;
      }


    } // end of namespace scribo::binarization::impl




      // Dispatch

    namespace internal
    {

      template <unsigned n, typename I>
      inline
      mln_ch_value(I, value::int_u<n>)
      sauvola_threshold_dispatch(const value::int_u<n>&, const I& input,
				 unsigned window_size,
				 double K)
      {
	return impl::sauvola_threshold_gl(input, window_size, K);
      }


      template <typename I>
      inline
      mln_ch_value(I, value::int_u8)
      sauvola_threshold_dispatch(const mln_value(I)&, const I& input,
				 unsigned window_size,
				 double K)
      {
	// No dispatch for this kind of value type.
	mlc_abort(I)::check();

	typedef mln_ch_value(I,bool) output_t;
	return output_t();
      }


    } // end of namespace scribo::binarization::internal



    template <typename I>
    mln_ch_value(I, value::int_u8)
    sauvola_threshold(const Image<I>& input, unsigned window_size,
		      double K)
    {
      trace::entering("scribo::binarization::sauvola_threshold");

      mln_precondition(mln_site_(I)::dim == 2);
      mln_precondition(exact(input).is_valid());

      typedef mln_value(I) value_t;
      mln_ch_value(I, value::int_u8)
	output = internal::sauvola_threshold_dispatch(value_t(),
						      exact(input),
						      window_size,
						      K);

      trace::exiting("scribo::binarization::sauvola_threshold");
      return output;
    }


    template <typename I>
    inline
    mln_ch_value(I, value::int_u8)
    sauvola_threshold(const Image<I>& input, unsigned window_size)
    {
      return sauvola_threshold(input, window_size,
			       SCRIBO_DEFAULT_SAUVOLA_K);
    }


    template <typename I>
    inline
    mln_ch_value(I, value::int_u8)
    sauvola_threshold(const Image<I>& input)
    {
      return sauvola_threshold(input, 11);
    }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::binarization

} // end of namespace scribo


#endif // ! SCRIBO_BINARIZATION_SAUVOLA_THRESHOLD_HH
