// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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
/// Convert an image into a binary image.

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


// Forward declaration.
namespace mln { template <typename T> class integral_image; }


namespace scribo
{

  namespace binarization
  {

    using namespace mln;

    /*! \brief Compute a threshold image with Sauvola's algorithm.

      \input[in]  input       An image.
      \input[in]  window_size The window size.x
      \input[out] simple      The sum of all intensities of \p input.
      \input[out] squared     The sum of all squared intensities of \p
                              input.

      \return An image of local thresholds.

     */
    template <typename I, typename T>
    mln_ch_value(I, value::int_u8)
    sauvola_threshold(const Image<I>& input, unsigned window_size,
		      integral_image<T>& simple,
		      integral_image<T>& squared);


    /// \overload
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


    namespace internal
    {

      struct rgb8_to_int_u8 : Function_v2v< rgb8_to_int_u8 >
      {
	typedef value::int_u8 result;
	result operator()(const value::rgb8& c) const
	{
	  return (c.red() + c.green() + c.blue()) / 3;
	}
      };


      unsigned long long square_(const value::int_u8& val)
      {
	unsigned long long v = static_cast<unsigned long long>(val);
	return v * v;
      }

      unsigned long long identity_(const value::int_u8& val)
      {
	return static_cast<unsigned long long>(val);
      }


      /// \brief Compute a point wise threshold according Sauvola's
      /// binarization.
      ///
      /// \param[in] p A site.
      /// \param[in] simple An integral image of mean values.
      /// \param[in] squared An integral image of squared mean values.
      /// \param[in] win_width Window width.
      /// \param[in] k Control the threshold value in the local
      ///              window. The higher, the lower the threshold
      ///              form the local mean m(x, y).
      /// \param[in] R Maximum value of the standard deviation (128
      ///              for grayscale documents).

      template <typename P, typename T>
      double
      compute_sauvola_threshold(const P& p,
				const integral_image<T>& simple,
				const integral_image<T>& squared,
				int win_width, double k, double R)
      {
	mln_precondition(simple.nrows() == squared.nrows());
	mln_precondition(simple.ncols() == squared.ncols());

	// Window half width.
	int w_2 = win_width >> 1;

	int row_min = std::max(0, p.row() - w_2);
	int col_min = std::max(0, p.col() - w_2);
	int row_max = std::min(simple.nrows() - 1, p.row() + w_2);
	int col_max = std::min(simple.ncols() - 1, p.col() + w_2);

	double wh = (row_max - row_min + 1) * (col_max - col_min + 1);

	// Mean.
	double m_x_y_tmp = (simple(row_max, col_max)
			    + simple(row_min, col_min)
			    - simple(row_max, col_min)
			    - simple(row_min, col_max));

	double m_x_y = m_x_y_tmp / wh;

	// Standard deviation.
	double s_x_y_tmp = (squared(row_max, col_max)
			    + squared(row_min, col_min)
			    - squared(row_max, col_min)
			    - squared(row_min, col_max));

	double s_x_y = std::sqrt((s_x_y_tmp - (m_x_y_tmp * m_x_y_tmp) / wh) / (wh - 1.f));

	// Thresholding.
	double t_x_y = m_x_y * (1.0 + k * ((s_x_y / R) - 1.0));

	return t_x_y;
      }


      template <typename P, typename T>
      double
      compute_sauvola_threshold(const P& p,
				const integral_image<T>& simple,
				const integral_image<T>& squared,
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


namespace mln
{

  template <typename T>
  class integral_image
  {
  public:

    integral_image()
      : img_(0), nrows_(0), ncols_(0)
    {}

    template<class F>
    integral_image(const image2d<T>& i, F func)
      : img_(0),
	nrows_(0),
	ncols_(0)
    {
      init_(i, func);
    }

    template<class F>
    void init_(const image2d<T>& i, F func)
    {
      nrows_ = i.nrows();
      ncols_ = i.ncols();

      img_ = (unsigned long long**)malloc(sizeof(unsigned long long*) * nrows_);
      for (int n = 0; n < nrows_; ++n)
	img_[n] = (unsigned long long*)malloc(sizeof(unsigned long long) * ncols_);

      img_[0][0] = func(i.at_(0, 0));

      for (int row = 1; row < nrows_; ++row)
	img_[row][0] = (*this)(row - 1, 0) + func(i.at_(row, 0));

      for (int col = 1; col < ncols_; ++col)
	img_[0][col] = (*this)(0, col - 1)
	  + func(i.at_(0, col));

      for (int row = 1; row < nrows_; ++row)
	for (int col = 1; col < ncols_; ++col)
	  img_[row][col] = (*this)(row - 1, col)
	    + (*this)(row, col - 1)
	    - (*this)(row - 1, col - 1)
	    + func(i.at_(row, col));
    }


    ~integral_image()
    {
      for (int n = 0; n < nrows_; ++n)
	free(img_[n]);
      free(img_);
    }

    bool is_valid() const
    {
      return img_ != 0;
    }

    unsigned long long operator()(int row, int col) const
    {
      return img_[row][col];
    }

    int nrows() const
    {
      return nrows_;
    }

    int ncols() const
    {
      return ncols_;
    }


  private:
    unsigned long long** img_;
    int nrows_;
    int ncols_;
  };

} // end of namespace mln



namespace scribo
{

  namespace binarization
  {

    // Implementation


    namespace impl
    {

      namespace generic
      {

	template <typename I, typename T>
	inline
	mln_concrete(I)
	sauvola_threshold(const I& input, unsigned window_size,
			  integral_image<T>& simple,
			  integral_image<T>& squared)
	{
	  trace::entering("scribo::binarization::impl::generic::sauvola_threshold");

	  typedef mln_value(I) V;
	  typedef mln_site(I) P;

	  // Compute the sum of all intensities of input
	  simple.init_(input, internal::identity_);

	  // Compute the sum of all squared intensities of input
	  squared.init_(input, internal::square_);

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



      template <typename I, typename T>
      inline
      mln_concrete(I)
      sauvola_threshold_gl(const I& input, unsigned window_size,
			   integral_image<T>& simple,
			   integral_image<T>& squared)
      {
	return impl::generic::sauvola_threshold(input, window_size,
						simple, squared);
      }


      template <typename I, typename T>
      inline
      mln_ch_value(I, value::int_u8)
      sauvola_threshold_rgb8(const I& input, unsigned window_size,
			     integral_image<T>& simple,
			     integral_image<T>& squared)
      {
	trace::entering("scribo::binarization::impl::sauvola_threshold_rgb8");

	mln_ch_value(I, value::int_u8) gima;
	gima = data::transform(input,
			       internal::rgb8_to_int_u8());

	mln_ch_value(I, value::int_u8)
	  output = impl::generic::sauvola_threshold(gima, window_size,
						    simple, squared);

	trace::exiting("scribo::binarization::impl::sauvola_threshold_rgb8");
	return output;
      }


    } // end of namespace scribo::binarization::impl




      // Dispatch

    namespace internal
    {

      template <unsigned n, typename I, typename T>
      inline
      mln_ch_value(I, value::int_u<n>)
      sauvola_threshold_dispatch(const value::int_u<n>&, const I& input,
		       unsigned window_size,
		       integral_image<T>& simple,
		       integral_image<T>& squared)
      {
	return impl::sauvola_threshold_gl(input, window_size, simple, squared);
      }

      template <typename I, typename T>
      inline
      mln_ch_value(I, value::int_u8)
      sauvola_threshold_dispatch(const value::rgb8&, const I& input,
		       unsigned window_size,
		       integral_image<T>& simple,
		       integral_image<T>& squared)
      {
	return impl::sauvola_threshold_rgb8(input, window_size,
					    simple, squared);
      }

      template <typename I, typename T>
      inline
      mln_ch_value(I, value::int_u8)
      sauvola_threshold_dispatch(const mln_value(I)&, const I& input,
		       unsigned window_size,
		       integral_image<T>& simple,
		       integral_image<T>& squared)
      {
	// No dispatch for this kind of value type.
	mlc_abort(I)::check();

	typedef mln_ch_value(I,bool) output_t;
	return output_t();
      }


    } // end of namespace scribo::binarization::internal



    template <typename I, typename T>
    mln_ch_value(I, value::int_u8)
    sauvola_threshold(const Image<I>& input, unsigned window_size,
		      integral_image<T>& simple,
		      integral_image<T>& squared)
    {
      trace::entering("scribo::binarization::sauvola_threshold");

      mln_precondition(mln_site_(I)::dim == 2);
      mln_precondition(exact(input).is_valid());

      typedef mln_value(I) value_t;
      mln_ch_value(I, value::int_u8)
	output = internal::sauvola_threshold_dispatch(value_t(), exact(input),
						      window_size, simple,
						      squared);

      trace::exiting("scribo::text::ppm2pbm");
      return output;
    }


    template <typename I>
    inline
    mln_ch_value(I, value::int_u8)
    sauvola_threshold(const Image<I>& input, unsigned window_size)
    {
      mln::integral_image<value::int_u8> simple, squared;
      return sauvola_threshold(input, window_size, simple, squared);
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
