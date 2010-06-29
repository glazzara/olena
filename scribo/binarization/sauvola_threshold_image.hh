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

# include <mln/debug/println.hh>

# include <scribo/core/init_integral_image.hh>

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
			    Image<J>& simple,
			    Image<J>& squared);


    /// \overload
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
      inline
      double
      sauvola_threshold_formula(const double m_x_y, const double s_x_y,
				const double k, const double R)
      {
      	return m_x_y * (1.0 + k * ((s_x_y / R) - 1.0));
      }


      /// \overload
      //
      inline
      double
      sauvola_threshold_formula(double m_x_y, double s_x_y)
      {
	// Badekas et al. said 0.34 was best.
	const double k = 0.34;

	// 128 is best for grayscale documents.
	const double R = 128;

	return sauvola_threshold_formula(m_x_y, s_x_y, k, R);
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
				int win_width, double k, double R)
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
	double t_x_y = sauvola_threshold_formula(m_x_y, s_x_y, k, R);

	return t_x_y;
      }


      template <typename P, typename J>
      double
      compute_sauvola_threshold_single_image(const P& p,
					     const J& integral,
					     int win_width)
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
	double t_x_y = m_x_y * (1.0 + 0.34 * ((s_x_y / 128) - 1.0));

	return t_x_y;
      }



      template <typename P, typename J>
      double
      compute_sauvola_threshold(const P& p,
				const J& simple,
				const J& squared,
				int win_width)
      {
	// Badekas et al. said 0.34 was best.
	const double k = 0.34;

	// 128 is best for grayscale documents.
	const double R = 128;

	return compute_sauvola_threshold(p, simple, squared, win_width, k, R);
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

	  const def::coord nrows = static_cast<def::coord>(input.nrows());
	  const def::coord ncols = static_cast<def::coord>(input.ncols());

	  for(def::coord row = 0; row < nrows; ++row)
	    for(def::coord col = 0; col < ncols; ++col)
	      output.at_(row, col)
		= internal::compute_sauvola_threshold(P(row, col), simple,
						      squared, window_size);

	  trace::exiting("scribo::binarization::impl::generic::sauvola_threshold");
	  return output;
	}

      } // end of namespace scribo::binarization::impl::generic



      template <typename I, typename J>
      inline
      mln_concrete(I)
      sauvola_threshold_image_gl(const I& input, unsigned window_size,
			   Image<J>& simple,
			   Image<J>& squared)
      {
	return impl::generic::sauvola_threshold_image(input, window_size,
						      simple, squared);
      }


      template <typename I, typename J>
      inline
      mln_ch_value(I, value::int_u8)
      sauvola_threshold_image_rgb8(const I& input, unsigned window_size,
				   Image<J>& simple,
				   Image<J>& squared)
      {
	trace::entering("scribo::binarization::impl::sauvola_threshold_image_rgb8");

	mln_ch_value(I, value::int_u8) gima;
	gima = data::transform(input,
			       mln::fun::v2v::rgb_to_int_u<8>());

	mln_ch_value(I, value::int_u8)
	  output = impl::generic::sauvola_threshold_image(gima, window_size,
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
				       J& simple,
				       J& squared)
      {
	return impl::sauvola_threshold_image_gl(input, window_size, simple, squared);
      }

      template <typename I, typename J>
      inline
      mln_ch_value(I, value::int_u8)
      sauvola_threshold_image_dispatch(const value::rgb8&, const I& input,
				       unsigned window_size,
				       J& simple,
				       J& squared)
      {
	return impl::sauvola_threshold_image_rgb8(input, window_size,
						  simple, squared);
      }

      template <typename I, typename J>
      inline
      mln_ch_value(I, value::int_u8)
      sauvola_threshold_image_dispatch(const mln_value(I)&, const I& input,
				       unsigned window_size,
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
		      Image<J>& simple,
		      Image<J>& squared)
    {
      trace::entering("scribo::binarization::sauvola_threshold_image");

      mln_precondition(mln_site_(I)::dim == 2);
      mln_precondition(exact(input).is_valid());


//       {
// 	J& simple_ = exact(simple);
// 	J& squared_ = exact(squared);
// 	mln_piter(J) p(simple_.domain());
// 	for_all(p)
// 	{
// 	  std::cout << simple_(p) << ", ";
// 	}
// 	std::cout << std::endl << " ------- " << std::endl;
// 	for_all(p)
// 	{
// 	  std::cout << squared_(p) << ", ";
// 	}
// 	std::cout << std::endl << " ------- " << std::endl;
//       }



      typedef mln_value(I) value_t;
      mln_ch_value(I, value::int_u8)
	output = internal::sauvola_threshold_image_dispatch(value_t(), exact(input),
							    window_size,
							    exact(simple),
							    exact(squared));

      trace::exiting("scribo::text::ppm2pbm");
      return output;
    }


    template <typename I>
    inline
    mln_ch_value(I, value::int_u8)
    sauvola_threshold_image(const Image<I>& input, unsigned window_size)
    {
      mln_ch_value(I, double)
	simple = init_integral_image(input, scribo::internal::identity_),
	squared = init_integral_image(input, scribo::internal::square_);

//       debug::println(input);
//       std::cout << "============" << std::endl;
// //       debug::println(simple);
// //       std::cout << "============" << std::endl;
// //       debug::println(squared);

      return sauvola_threshold_image(input, window_size, simple, squared);
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
