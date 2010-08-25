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

#ifndef SCRIBO_BINARIZATION_SAUVOLA_THRESHOLD_IMAGE_HH
# define SCRIBO_BINARIZATION_SAUVOLA_THRESHOLD_IMAGE_HH

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
    sauvola_threshold_image(const Image<I>& input, unsigned window_size,
			    double K,
			    Image<J>& simple,
			    Image<J>& squared);

    /// \overload
    template <typename I>
    mln_ch_value(I, value::int_u8)
    sauvola_threshold_image(const Image<I>& input, unsigned window_size,
			    double K);

    /// \overload
    /// K is set to 0.34
    template <typename I>
    mln_ch_value(I, value::int_u8)
    sauvola_threshold_image(const Image<I>& input, unsigned window_size);


    /// \overload
    /// The window size is set to 11.
    //
    template <typename I>
    mln_ch_value(I, value::int_u8)
    sauvola_threshold_image(const Image<I>& input);



# ifndef MLN_INCLUDE_ONLY


    namespace internal
    {

#  ifdef SCRIBO_SAUVOLA_DEBUG
      // Declare debug images.
      image2d<value::int_u8> debug_k;
      image2d<float> debug_s_n;
      image2d<float> debug_k_l;
#  endif // ! SCRIBO_SAUVOLA_DEBUG


      /*! \brief compute Sauvola's threshold applying directly the formula.

	\param[in] m_x_y Mean value.
	\param[in] s_x_y Standard deviation.
        \param[in] k Control the threshold value in the local
                     window. The higher, the lower the threshold
                     form the local mean m(x, y).
        \param[in] R Maximum value of the standard deviation (128
                     for grayscale documents).

	\return A threshold.
       */
#  ifdef SCRIBO_SAUVOLA_DEBUG
      inline
      double
      sauvola_threshold_formula(const double m_x_y, const double s_x_y,
				const double K, const double R,
				value::int_u8& dbg_k, float& dbg_s_n,
				float& dbg_k_l)
#  else
      inline
      double
      sauvola_threshold_formula(const double m_x_y, const double s_x_y,
				const double K, const double R)
#  endif // ! SCRIBO_SAUVOLA_DEBUG
      {
//	double s_N = s_x_y / 256;
	double K_2 = K;
//	double K_2 = exp(K * log(s_x_y / 256));
// 	if (s_x_y < 30)
// 	  K_2 = 0.01;
// 	else if (s_x_y < 80)
// 	  K_2 = 0.1;
// 	else if (s_x_y > 80)
// 	  K_2 = K;


// Results_0.1_0.34
//
// 	if (s_N < 0.1f)
// 	{
// 	  K_2 = 0.1f;
// #  ifdef SCRIBO_SAUVOLA_DEBUG
// 	  dbg_k = 0;
// 	  dbg_s_n = s_N;
// #  endif // !SCRIBO_SAUVOLA_DEBUG
// 	}
// 	else if (s_N > 0.34)
// 	{
// 	  K_2 = 0.34;
// #  ifdef SCRIBO_SAUVOLA_DEBUG
// 	  dbg_k = 255;
// 	  dbg_s_n = s_N;
// #  endif // !SCRIBO_SAUVOLA_DEBUG
// 	}
// 	else
// 	{
// 	  K_2 = s_N;
// #  ifdef SCRIBO_SAUVOLA_DEBUG
// 	  dbg_k = 150;
// 	  dbg_s_n = s_N;
// #  endif // !SCRIBO_SAUVOLA_DEBUG
// 	}


// 	const double k_min = 0.1f;
// 	const double k_max = 1.0f;
// 	const double s_1 = 0.05f;
// 	const double s_2 = 0.50f;

// 	double k_b = (k_max - k_min) / (double)(s_2 - s_1);
// 	double k_a = 0.1f - s_1  * k_b;
// 	K_2 = k_a + k_b * s_N;

// 	dbg_s_n = s_N;
// 	if (K_2 < k_min)
// 	  dbg_k = 0;
// 	else if (K_2 > k_max)
// 	  dbg_k = 255;
// 	else
// 	  dbg_k = 150;



// 	if (s_N < 0.1f)
// 	{
// 	  K_2 = 0.1f;
// 	  dbg_k = 0;
// 	  dbg_s_n = s_N;
// 	  dbg_k_l = 0.1;
// 	}
// 	else
// 	{
// //	  double K_L = ((long int)((s_N * 11) + 0.49999)) * s_N;
// 	  double K_L = s_N * K / 3.0f;
// //	  K_2 = std::min(K_L, (double) 1.0);
// 	  K_2 = K_L;
// 	  if (K_L > 1.0f)
// 	    dbg_k = 255;
// 	  else
// 	    dbg_k = 150;

// 	  dbg_s_n = s_N;
// 	  dbg_k_l = K_L;
// 	}


	return m_x_y * (1.0 + K_2 * ((s_x_y / R) - 1.0));
      }


      /// \overload
      //
      inline
      double
      sauvola_threshold_formula(double m_x_y, double s_x_y)
      {
#  ifdef SCRIBO_SAUVOLA_DEBUG
	std::cout << "This overload of sauvola_threshold_formula is disabled in debug mode!" << std::endl;
	return 0;
#  else
 	return sauvola_threshold_formula(m_x_y, s_x_y,
 					 SCRIBO_DEFAULT_SAUVOLA_K,
 					 SCRIBO_DEFAULT_SAUVOLA_R);
#  endif // !SCRIBO_SAUVOLA_DEBUG
      }


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
      template <typename P, typename J>
      double
      compute_sauvola_threshold(const P& p,
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

	// Standard deviation.
	double s_x_y_tmp = (squared.at_(row_max, col_max)
			    + squared.at_(row_min, col_min)
			    - squared.at_(row_max, col_min)
			    - squared.at_(row_min, col_max));

	double s_x_y = std::sqrt((s_x_y_tmp - (m_x_y_tmp * m_x_y_tmp) / wh) / (wh - 1.f));

	// Thresholding.
#  ifdef SCRIBO_SAUVOLA_DEBUG
	double t_x_y = sauvola_threshold_formula(m_x_y, s_x_y, K, R, debug_k(p), debug_s_n(p), debug_k_l(p));
#  else
	double t_x_y = sauvola_threshold_formula(m_x_y, s_x_y, K, R);
#  endif // ! SCRIBO_SAUVOLA_DEBUG

	return t_x_y;
      }


      template <typename P, typename J>
      double
      compute_sauvola_threshold_single_image(const P& p,
					     const J& integral,
					     int win_width,
					     double K, double R)
      {
	// Window half width.
	int w_2 = win_width >> 1;

	int row_min = std::max(0, p.row() - w_2);
	int col_min = std::max(0, p.col() - w_2);

	int row_max = std::min(static_cast<int>(integral.nrows()) - 1,
			       p.row() + w_2);
	int col_max = std::min(static_cast<int>(integral.ncols()) - 1,
			       p.col() + w_2);


	double wh = (row_max - row_min + 1) * (col_max - col_min + 1);

	// Mean.
	double m_x_y_tmp = (integral.at_(row_max, col_max).first()
			    + integral.at_(row_min, col_min).first()
			    - integral.at_(row_max, col_min).first()
			    - integral.at_(row_min, col_max).first());

	double m_x_y = m_x_y_tmp / wh;

	// Standard deviation.
	double s_x_y_tmp = (integral.at_(row_max, col_max).second()
			    + integral.at_(row_min, col_min).second()
			    - integral.at_(row_max, col_min).second()
			    - integral.at_(row_min, col_max).second());

	double s_x_y = std::sqrt((s_x_y_tmp - (m_x_y_tmp * m_x_y_tmp) / wh) / (wh - 1.f));

	// Thresholding.
#  ifdef SCRIBO_SAUVOLA_DEBUG
	double t_x_y = sauvola_threshold_formula(m_x_y, s_x_y, K, R, debug_k(p), debug_s_n(p), debug_k_l(p));
#  else
	double t_x_y = sauvola_threshold_formula(m_x_y, s_x_y, K, R);
#  endif // !SCRIBO_SAUVOLA_DEBUG

	return t_x_y;
      }



      template <typename P, typename J>
      double
      compute_sauvola_threshold(const P& p,
				const J& simple,
				const J& squared,
				int win_width)
      {
	return compute_sauvola_threshold(p, simple, squared, win_width,
					 SCRIBO_DEFAULT_SAUVOLA_K,
					 SCRIBO_DEFAULT_SAUVOLA_R);
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

	template <typename I, typename J>
	inline
	mln_concrete(I)
	sauvola_threshold_image(const Image<I>& input_, unsigned window_size,
				double K,
				Image<J>& simple_,
				Image<J>& squared_)
	{
	  trace::entering("scribo::binarization::impl::generic::sauvola_threshold_image");

	  const I& input = exact(input_);
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

	  const mln::def::coord
	    nrows = static_cast<mln::def::coord>(input.nrows()),
	    ncols = static_cast<mln::def::coord>(input.ncols());


	  for(def::coord row = 0; row < nrows; ++row)
	    for(def::coord col = 0; col < ncols; ++col)
	      mln::convert::from_to(
		internal::compute_sauvola_threshold(P(row, col), simple,
						    squared, window_size,
						    K,
						    SCRIBO_DEFAULT_SAUVOLA_R),
		output.at_(row, col));

	  trace::exiting("scribo::binarization::impl::generic::sauvola_threshold");
	  return output;
	}

      } // end of namespace scribo::binarization::impl::generic



      template <typename I, typename J>
      inline
      mln_concrete(I)
      sauvola_threshold_image_gl(const I& input, unsigned window_size,
				 double K,
				 Image<J>& simple,
				 Image<J>& squared)
      {
	return impl::generic::sauvola_threshold_image(input, window_size, K,
						      simple, squared);
      }


      template <typename I, typename J>
      inline
      mln_ch_value(I, value::int_u8)
      sauvola_threshold_image_rgb8(const I& input, unsigned window_size,
				   double K,
				   Image<J>& simple,
				   Image<J>& squared)
      {
	trace::entering("scribo::binarization::impl::sauvola_threshold_image_rgb8");

	mln_ch_value(I, value::int_u8) gima;
	gima = data::transform(input,
			       mln::fun::v2v::rgb_to_int_u<8>());

	mln_ch_value(I, value::int_u8)
	  output = impl::generic::sauvola_threshold_image(gima, window_size,
							  K,
							  simple, squared);

	trace::exiting("scribo::binarization::impl::sauvola_threshold_image_rgb8");
	return output;
      }


    } // end of namespace scribo::binarization::impl




      // Dispatch

    namespace internal
    {

      template <unsigned n, typename I, typename J>
      inline
      mln_ch_value(I, value::int_u<n>)
      sauvola_threshold_image_dispatch(const value::int_u<n>&, const I& input,
				       unsigned window_size,
				       double K,
				       J& simple,
				       J& squared)
      {
	return impl::sauvola_threshold_image_gl(input, window_size, K,
						simple, squared);
      }

      template <typename I, typename J>
      inline
      mln_ch_value(I, value::int_u8)
      sauvola_threshold_image_dispatch(const value::rgb8&, const I& input,
				       unsigned window_size,
				       double K,
				       J& simple,
				       J& squared)
      {
	return impl::sauvola_threshold_image_rgb8(input, window_size,
						  K, simple, squared);
      }

      template <typename I, typename J>
      inline
      mln_ch_value(I, value::int_u8)
      sauvola_threshold_image_dispatch(const mln_value(I)&, const I& input,
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



    template <typename I, typename J>
    mln_ch_value(I, value::int_u8)
    sauvola_threshold_image(const Image<I>& input, unsigned window_size,
			    double K,
			    Image<J>& simple,
			    Image<J>& squared)
    {
      trace::entering("scribo::binarization::sauvola_threshold_image");

      mln_precondition(mln_site_(I)::dim == 2);
      mln_precondition(exact(input).is_valid());

      typedef mln_value(I) value_t;
      mln_ch_value(I, value::int_u8)
	output = internal::sauvola_threshold_image_dispatch(value_t(),
							    exact(input),
							    window_size,
							    K,
							    exact(simple),
							    exact(squared));

      trace::exiting("scribo::text::ppm2pbm");
      return output;
    }


    template <typename I>
    inline
    mln_ch_value(I, value::int_u8)
    sauvola_threshold_image(const Image<I>& input, unsigned window_size,
			    double K)
    {
      mln_ch_value(I, double)
	simple = init_integral_image(input, scribo::internal::identity_),
	squared = init_integral_image(input, scribo::internal::square_);

      return sauvola_threshold_image(input, window_size,
				     K, simple, squared);
    }


    template <typename I>
    inline
    mln_ch_value(I, value::int_u8)
    sauvola_threshold_image(const Image<I>& input, unsigned window_size)
    {
      return sauvola_threshold_image(input, window_size,
				     SCRIBO_DEFAULT_SAUVOLA_K);
    }


    template <typename I>
    inline
    mln_ch_value(I, value::int_u8)
    sauvola_threshold_image(const Image<I>& input)
    {
      return sauvola_threshold_image(input, 11);
    }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::binarization

} // end of namespace scribo


#endif // ! SCRIBO_BINARIZATION_SAUVOLA_THRESHOLD_IMAGE_HH
