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

#ifndef SCRIBO_BINARIZATION_SAUVOLA_HH
# define SCRIBO_BINARIZATION_SAUVOLA_HH

/// \file
///
/// Convert an image into a binary image.

# include <algorithm>
# include <cmath>

# include <mln/core/image/image2d.hh>
# include <mln/value/rgb8.hh>
# include <mln/value/int_u.hh>
# include <mln/value/int_u8.hh>

# include <mln/data/transform.hh>
# include <mln/pw/all.hh>
# include <mln/core/routine/duplicate.hh>




namespace scribo
{

  namespace binarization
  {

    using namespace mln;

    /*! \brief Convert an image into a binary image.

      \input[in] An image.

      \return A binary image.

     */
    template <typename I>
    mln_ch_value(I,bool)
    sauvola(const Image<I>& input);



# ifndef MLN_INCLUDE_ONLY


    namespace internal
    {

      template <typename T>
      class integral_image
      {
      public:

	template<class F>
	integral_image(const image2d<T>& i, F func)
	  : img_(0),
	    nrows_(i.nrows()),
	    ncols_(i.ncols())
	{
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

	unsigned long long operator()(int row, int col) const
	{
	  return img_[row][col];
	}

      private:
	unsigned long long** img_;
	int nrows_;
	int ncols_;
      };


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

    } // end of namespace scribo::binarization::internal




    // Implementation


    namespace impl
    {

      namespace generic
      {

	template <typename I>
	inline
	mln_ch_value(I, bool)
	sauvola(const I& input)
	{
	  trace::entering("scribo::binarization::impl::generic::sauvola");

	  typedef mln_value(I) V;

	  // Value of the window size
	  const unsigned int w = 11;

	  // Control the threshold value in the local window
	  // The higher, the lower the threshold form the local
	  // mean m(x, y). Badekas et al. said 0.34 was best.
	  const double k = 0.34;

	  // Maximum value of the standard deviation (128 for
	  // grayscale documents).
	  const double R = 128;

	  // Compute the sum of all intensities of input
	  internal::integral_image<V> simple(input, internal::identity_);

	  // Compute the sum of all squared intensities of input
	  internal::integral_image<V> squared(input, internal::square_);

	  int w_2 = w >> 1;

	  // Savaula Algorithm with I.I.

	  image2d<bool> output;
	  initialize(output, input);

	  const def::coord nrows = static_cast<def::coord>(input.nrows());
	  const def::coord ncols = static_cast<def::coord>(input.ncols());

	  for(def::coord row = 0; row < nrows; ++row)
	    for(def::coord col = 0; col < ncols; ++col)
	    {
	      int row_min = std::max(0, row - w_2);
	      int col_min = std::max(0, col - w_2);
	      int row_max = std::min(nrows - 1, row + w_2);
	      int col_max = std::min(ncols - 1, col + w_2);

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

	      output.at_(row, col) = (input.at_(row, col) < t_x_y);
	    }

	  trace::exiting("scribo::binarization::impl::generic::sauvola");
	  return output;
	}

      } // end of namespace scribo::binarization::impl::generic


	template <typename I>
	inline
	mln_ch_value(I, bool)
	sauvola_gl(const I& input)
	{
	  return impl::generic::sauvola(input);
	}


	template <typename I>
	inline
	mln_ch_value(I, bool)
	sauvola_rgb8(const I& input)
	{
	  trace::entering("scribo::binarization::impl::sauvola_rgb8");

	  mln_ch_value(I, value::int_u8) gima;
	  gima = data::transform(input,
				 internal::rgb8_to_int_u8());

	  mln_ch_value(I, bool) output = impl::generic::sauvola(gima);

	  trace::exiting("scribo::binarization::impl::sauvola_rgb8");
	  return output;
	}


      } // end of namespace scribo::binarization::impl




      // Dispatch

      namespace internal
      {

	template <typename I, unsigned n>
	inline
	mln_ch_value(I, bool)
	sauvola_dispatch(const value::int_u<n>&, const I& input)
	{
	  return impl::sauvola_gl(input);
	}

	template <typename I>
	inline
	mln_ch_value(I, bool)
	sauvola_dispatch(const value::rgb8&, const I& input)
	{
	  return impl::sauvola_rgb8(input);
	}

	template <typename I>
	inline
	mln_ch_value(I, bool)
	sauvola_dispatch(const mln_value(I)&, const I& input)
	{
	  // No dispatch for this kind of value type.
	  mlc_abort(I)::check();

	  typedef mln_ch_value(I,bool) output_t;
	  return output_t();
	}


      } // end of namespace scribo::binarization::internal


      template <typename I>
      inline
      mln_ch_value(I, bool)
      sauvola(const Image<I>& input)
      {
	trace::entering("scribo::binarization::sauvola");

	mln_precondition(I::dim == 2);
	mln_precondition(exact(input).is_valid());

	typedef mln_value(I) value_t;
	mln_ch_value(I, bool)
	  output = internal::sauvola_dispatch(value_t(), exact(input));

	trace::exiting("scribo::text::ppm2pbm");
	return output;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::binarization

} // end of namespace scribo


#endif // ! SCRIBO_TEXT_PPM2PBM_HH
