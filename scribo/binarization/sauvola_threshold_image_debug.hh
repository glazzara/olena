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

#ifndef SCRIBO_BINARIZATION_SAUVOLA_THRESHOLD_IMAGE_DEBUG_HH
# define SCRIBO_BINARIZATION_SAUVOLA_THRESHOLD_IMAGE_DEBUG_HH

/// \file
///
/// Compute an image of local threshold using Sauvola algorithm.

/// \fixme return type too restrictive!

# include <algorithm>
# include <cmath>

# include <mln/core/image/image2d.hh>
# include <mln/value/rgb8.hh>
# include <mln/value/int_u.hh>
# include <mln/value/int_u8.hh>

# include <mln/data/transform.hh>
# include <mln/pw/all.hh>
# include <mln/core/routine/duplicate.hh>

# include <mln/fun/v2v/rgb_to_int_u.hh>

# include <scribo/core/init_integral_image.hh>
# include <scribo/binarization/sauvola_threshold_image.hh>


// Setup default Sauvola's formulae parameters values.
// These macros may be used in other variant of Sauvola's algorithm.
//
// Values are set according to the following reference: "Automatic
// Evaluation of Document Binarization Results", Badekas and al, 2005
//
// Badekas et al. said 0.34 was best.
# define SCRIBO_DEFAULT_SAUVOLA_K 0.34
//
// 128 is best for grayscale documents.
# define SCRIBO_DEFAULT_SAUVOLA_R 128



namespace scribo
{

  namespace binarization
  {

    using namespace mln;

    /*! \brief Compute an image of local threshold using Sauvola algorithm.

      \input[in]  input       An image.
      \input[in]  window_size The window size.
      \input[out] simple      The sum of all intensities of \p input.
      \input[out] squared     The sum of all squared intensities of \p
                              input.

      \return An image of local thresholds.

     */
    template <typename I, typename J>
    mln_ch_value(I, value::int_u8)
    sauvola_threshold_image_debug(const Image<I>& input, unsigned window_size,
				  double K,
				  Image<J>& simple,
				  Image<J>& squared);

    /// \overload
    template <typename I>
    mln_ch_value(I, value::int_u8)
    sauvola_threshold_image_debug(const Image<I>& input, unsigned window_size,
				  double K);



# ifndef MLN_INCLUDE_ONLY


    namespace internal
    {

      unsigned mean_debug_factor   = 1;
      unsigned stddev_debug_factor = 10;

      /*! \brief Compute a point wise threshold according Sauvola's
          binarization.

          \param[in] p A site.
          \param[in] simple An integral image of mean values.
          \param[in] squared An integral image of squared mean values.
          \param[in] win_width Window width.
          \param[in] k Control the threshold value in the local
                       window. The higher, the lower the threshold
                       form the local mean m(x, y).
          \param[in] R Maximum value of the standard deviation (128
                       for grayscale documents).

	  \return A threshold.
      */
      template <typename P, typename M, typename J>
      double
      compute_sauvola_threshold(const P& p,
				M& mean, M& stddev,
				const J& simple,
				const J& squared,
				int win_width, double K, double R)
      {
	mln_precondition(simple.nrows() == squared.nrows());
	mln_precondition(simple.ncols() == squared.ncols());

	// Window half width.
	int w_2 = win_width >> 1;

	int row_min = std::max(0, p.row() - w_2 - 1);
	int col_min = std::max(0, p.col() - w_2 - 1);

	int row_max = std::min(static_cast<int>(simple.nrows()) - 1,
			       p.row() + w_2);
	int col_max = std::min(static_cast<int>(simple.ncols()) - 1,
			       p.col() + w_2);


	double wh = (row_max - row_min) * (col_max - col_min);

	// Mean.
	double m_x_y_tmp = (simple.at_(row_max, col_max)
			    + simple.at_(row_min, col_min)
			    - simple.at_(row_max, col_min)
			    - simple.at_(row_min, col_max));

	double m_x_y = m_x_y_tmp / wh;

	mean(p) = m_x_y;// * mean_debug_factor;

	// Standard deviation.
	double s_x_y_tmp = (squared.at_(row_max, col_max)
			    + squared.at_(row_min, col_min)
			    - squared.at_(row_max, col_min)
			    - squared.at_(row_min, col_max));

	double s_x_y = std::sqrt((s_x_y_tmp - (m_x_y_tmp * m_x_y_tmp) / wh) / (wh - 1.f));

	stddev(p) = s_x_y;// * stddev_debug_factor;

	// Thresholding.
	double t_x_y = sauvola_threshold_formula(m_x_y, s_x_y, K, R);

	return t_x_y;
      }


    } // end of namespace scribo::binarization::internal

  } // end of namespace scribo::binarization

} // end of namespace scribo



namespace scribo
{

  namespace binarization
  {

    // Implementation


    namespace impl
    {

      namespace generic
      {

	template <typename I, typename M, typename J>
	inline
	mln_concrete(I)
	sauvola_threshold_image_debug(const Image<I>& input_,
				      unsigned window_size,
				      double K,
				      Image<M>& mean_, Image<M>& stddev_,
				      Image<J>& simple_,
				      Image<J>& squared_)
	{
	  trace::entering("scribo::binarization::impl::generic::sauvola_threshold_image_debug");

	  const I& input = exact(input_);
	  M& mean = exact(mean_);
	  M& stddev = exact(stddev_);
	  J& simple = exact(simple_);
	  J& squared = exact(squared_);

	  mln_assertion(input.is_valid());
	  mln_assertion(simple.is_valid());
	  mln_assertion(squared.is_valid());

	  typedef mln_value(I) V;
	  typedef mln_site(I) P;

	  // Savaula Algorithm with I.I.

	  mln_concrete(I) output;
	  initialize(output, input);

	  const def::coord nrows = static_cast<def::coord>(input.nrows());
	  const def::coord ncols = static_cast<def::coord>(input.ncols());

	  for(def::coord row = 0; row < nrows; ++row)
	    for(def::coord col = 0; col < ncols; ++col)
	      output.at_(row, col)
		= internal::compute_sauvola_threshold(P(row, col),
						      mean, stddev,
						      simple, squared,
						      window_size, K,
						      SCRIBO_DEFAULT_SAUVOLA_R);

	  trace::exiting("scribo::binarization::impl::generic::sauvola_threshold");
	  return output;
	}

      } // end of namespace scribo::binarization::impl::generic



      template <typename I, typename M, typename J>
      inline
      mln_concrete(I)
      sauvola_threshold_image_debug_gl(const Image<I>& input,
				       unsigned window_size,
				       double K,
				       Image<M>& mean, Image<M>& stddev,
				       Image<J>& simple,
				       Image<J>& squared)
      {
	return impl::generic::sauvola_threshold_image_debug(input, window_size,
							    K,
							    mean, stddev,
							    simple, squared);
      }


      template <typename I, typename M, typename J>
      inline
      mln_ch_value(I, value::int_u8)
      sauvola_threshold_image_debug_rgb8(const Image<I>& input,
					 unsigned window_size,
					 double K,
					 Image<M>& mean, Image<M>& stddev,
					 Image<J>& simple,
					 Image<J>& squared)
      {
	trace::entering("scribo::binarization::impl::sauvola_threshold_image_debug_rgb8");

	mln_ch_value(I, value::int_u8) gima;
	gima = data::transform(input,
			       mln::fun::v2v::rgb_to_int_u<8>());

	mln_ch_value(I, value::int_u8)
	  output = impl::generic::sauvola_threshold_image_debug(gima, window_size,
								K,
								mean, stddev,
								simple, squared);

	trace::exiting("scribo::binarization::impl::sauvola_threshold_image_debug_rgb8");
	return output;
      }


    } // end of namespace scribo::binarization::impl




      // Dispatch

    namespace internal
    {

      template <unsigned n, typename I, typename M, typename J>
      inline
      mln_ch_value(I, value::int_u<n>)
      sauvola_threshold_image_debug_dispatch(const value::int_u<n>&,
					     const I& input,
					     unsigned window_size,
					     double K,
					     M& mean, M& stddev,
					     J& simple,
					     J& squared)
      {
	return impl::sauvola_threshold_image_debug_gl(input, window_size, K,
						      mean, stddev,
						      simple, squared);
      }

      template <typename I, typename M, typename J>
      inline
      mln_ch_value(I, value::int_u8)
      sauvola_threshold_image_debug_dispatch(const value::rgb8&, const I& input,
					     unsigned window_size,
					     double K,
					     M& mean, M& stddev,
					     J& simple,
					     J& squared)
      {
	return impl::sauvola_threshold_image_debug_rgb8(input, window_size,
							K, mean, stddev,
							simple, squared);
      }

      template <typename I, typename M, typename J>
      inline
      mln_ch_value(I, value::int_u8)
      sauvola_threshold_image_debug_dispatch(const mln_value(I)&, const I& input,
					     M& mean, M& stddev,
					     unsigned window_size,
					     double K,
					     J& simple,
					     J& squared)
      {
	// No dispatch for this kind of value type.
	mlc_abort(I)::check();

	typedef mln_ch_value(I,bool) output_t;
	return output_t();
      }


    } // end of namespace scribo::binarization::internal



    template <typename I, typename M, typename J>
    mln_ch_value(I, value::int_u8)
    sauvola_threshold_image_debug(const Image<I>& input, unsigned window_size,
				  double K,
				  Image<M>& mean, Image<M>& stddev,
				  Image<J>& simple,
				  Image<J>& squared)
    {
      trace::entering("scribo::binarization::sauvola_threshold_image_debug");

      mln_precondition(mln_site_(I)::dim == 2);
      mln_precondition(exact(input).is_valid());

      typedef mln_value(I) value_t;
      mln_ch_value(I, value::int_u8)
	output = internal::sauvola_threshold_image_debug_dispatch(value_t(),
								  exact(input),
								  window_size,
								  K,
								  exact(mean),
								  exact(stddev),
								  exact(simple),
								  exact(squared));

      trace::exiting("scribo::text::ppm2pbm");
      return output;
    }


    template <typename I, typename M>
    inline
    mln_ch_value(I, value::int_u8)
    sauvola_threshold_image_debug(const Image<I>& input, unsigned window_size,
				  double K,
				  Image<M>& mean, Image<M>& stddev)
    {
      mln_ch_value(I, double)
	simple = init_integral_image(input, scribo::internal::identity_),
	squared = init_integral_image(input, scribo::internal::square_);

      return sauvola_threshold_image_debug(input, window_size, K,
					   mean, stddev,
					   simple, squared);
    }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::binarization

} // end of namespace scribo


#endif // ! SCRIBO_BINARIZATION_SAUVOLA_THRESHOLD_IMAGE_DEBUG_HH
