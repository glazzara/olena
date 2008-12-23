// Copyright (C) 2001, 2002, 2003, 2004, 2008 EPITA Research and
// Laboratory (LRDE)
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_LINEAR_GAUSSIAN_HH
# define MLN_LINEAR_GAUSSIAN_HH

/// \file mln/linear/gaussian.hh
///
/// Gaussian filter.
///
/// \todo Add a clean reference David Deriche
///	 Recursively implementing the gaussian and its derivatives (1993)

# include <mln/core/concept/image.hh>
# include <mln/core/alias/point2d.hh>
# include <mln/data/paste.hh>
# include <mln/geom/ncols.hh>
# include <mln/geom/nrows.hh>

# include <mln/trait/image/props.hh>

# include <mln/extension/adjust_fill.hh>
# include <mln/level/stretch.hh>

# include <mln/algebra/vec.hh>

# include <vector>
# include <cmath>

# include <mln/debug/println.hh>

namespace mln
{

  namespace linear
  {

    /// Gaussian filter of an image \p input
    ///
    /// \pre output.domain = input.domain
    ///
    template <class I>
    mln_concrete(I)
    gaussian(const Image<I>& input, float sigma);

# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      typedef float norm_fun(float, float,
			     float, float,
			     float, float,
			     float, float,
			     float, float,
			     int&);

      struct recursivefilter_coef_
      {

	///
	/// Constructor.
	///
	recursivefilter_coef_(float a0, float a1,
			      float b0, float b1,
			      float c0, float c1,
			      float w0, float w1,
			      float s, norm_fun norm);
	std::vector<float> n, d, nm, dm;
      };

      inline
      recursivefilter_coef_::recursivefilter_coef_(float a0, float a1,
						      float b0, float b1,
						      float c0, float c1,
						      float w0, float w1,
						      float s, norm_fun norm)
      {
	n.reserve(5);
	d.reserve(5);
	nm.reserve(5);
	dm.reserve(5);

	b0 /= s;
	b1 /= s;
	w0 /= s;
	w1 /= s;

	float sin0 = sin(w0);
	float sin1 = sin(w1);
	float cos0 = cos(w0);
	float cos1 = cos(w1);

	int sign = 1;
	float n_ = norm(a0, a1, b0, b1, c0, c1, cos0, sin0, cos1, sin1, sign);

	a0 /= n_;
	a1 /= n_;
	c0 /= n_;
	c1 /= n_;

	n[3] =
	  exp(-b1 - 2*b0) * (c1 * sin1 - cos1 * c0) +
	  exp(-b0 - 2*b1) * (a1 * sin0 - cos0 * a0);
	n[2] =
	  2 * exp(-b0 - b1) * ((a0 + c0) * cos1 * cos0 -
			       cos1 * a1 * sin0 -
			       cos0 * c1 * sin1) +
	  c0 * exp(-2*b0) + a0 * exp(-2*b1);
	n[1] =
	  exp(-b1) * (c1 * sin1 - (c0 + 2 * a0) * cos1) +
	  exp(-b0) * (a1 * sin0 - (2 * c0 + a0) * cos0);
	n[0] =
	  a0 + c0;

	d[4] =
	  exp(-2 * b0 - 2 * b1);
	d[3] =
	  -2 * cos0 * exp(-b0 - 2*b1) -
	  2 * cos1 * exp(-b1 - 2*b0);
	d[2] =
	  4 * cos1 * cos0 * exp(-b0 - b1) +
	  exp(-2*b1) + exp(-2*b0);
	d[1] =
	  -2 * exp(-b1) * cos1 - 2 * exp(-b0) * cos0;

	for (unsigned i = 1; i <= 3; ++i)
	{
	    dm[i] = d[i];
	    nm[i] = sign * (n[i] - d[i] * n[0]);
	}
	dm[4] = d[4];
	nm[4] = sign * (-d[4] * n[0]);
      }



      template <class WorkType, class I>
      inline
      void
      recursivefilter_(I& ima,
		       const recursivefilter_coef_& c,
		       const mln_psite(I)& start,
		       const mln_psite(I)& finish,
		       int len,
		       const mln_deduce(I, psite, delta)& d)
      {
	std::vector<WorkType>	tmp1(len);
	std::vector<WorkType>	tmp2(len);

	// The fourth degree approximation implies to have a special
	// look on the four first points we consider that there is
	// no signal before 0 (to be discussed)

	// --
	// Causal part

	tmp1[0] =
	  c.n[0] * ima(start);

	tmp1[1] =
	  c.n[0] * ima(start + d)
	  + c.n[1] * ima(start)
	  - c.d[1] * tmp1[0];

	tmp1[2] =
	  c.n[0] * ima(start + d + d)
	  + c.n[1] * ima(start + d)
	  + c.n[2] * ima(start)
	  - c.d[1] * tmp1[1]
	  - c.d[2] * tmp1[0];

	tmp1[3] =
	  c.n[0] * ima(start + d + d + d)
	  + c.n[1] * ima(start + d + d)
	  + c.n[2] * ima(start + d)
	  + c.n[3] * ima(start)
	  - c.d[1] * tmp1[2] - c.d[2] * tmp1[1]
	  - c.d[3] * tmp1[0];

	mln_psite(I) current(start + d + d + d + d);
	for (mln_deduce(I, site, coord) i = 4; i < len; ++i)
        {
          tmp1[i] =
            c.n[0] * ima(current)
            + c.n[1] * ima(current - d)
            + c.n[2] * ima(current - d - d)
            + c.n[3] * ima(current - d - d - d)
            - c.d[1] * tmp1[i - 1] - c.d[2] * tmp1[i - 2]
            - c.d[3] * tmp1[i - 3] - c.d[4] * tmp1[i - 4];
          current = current + d;
        }

	// Non causal part

	tmp2[len - 1] = WorkType(); // FIXME : = 0, literal::zero ...?

	tmp2[len - 2] =
	  c.nm[1] * ima(finish);

	tmp2[len - 3] =
	  c.nm[1] * ima(finish - d)
	  + c.nm[2] * ima(finish)
	  - c.dm[1] * tmp2[len - 2];

	tmp2[len - 4] =
	  c.nm[1] * ima(finish - d - d)
	  + c.nm[2] * ima(finish - d)
	  + c.nm[3] * ima(finish)
	  - c.dm[1] * tmp2[len - 3]
	  - c.dm[2] * tmp2[len - 2];

	current = finish - d - d - d ;

	for (int i = len - 5; i >= 0; --i)
	{
          tmp2[i] =
            c.nm[1] * ima(current)
            + c.nm[2] * ima(current + d)
            + c.nm[3] * ima(current + d + d)
            + c.nm[4] * ima(current + d + d + d)
            - c.dm[1] * tmp2[i + 1] - c.dm[2] * tmp2[i + 2]
            - c.dm[3] * tmp2[i + 3] - c.dm[4] * tmp2[i + 4];
          current = current - d;
        }

	// Combine results from causal and non-causal parts.
	current = start;
	for (int i = 0; i < len; ++i)
	{
          ima(current) = tmp1[i] + tmp2[i];
          current = current + d;
	}
      }


      inline
      float gaussian_norm_coef_(float a0, float a1,
				float b0, float b1,
				float c0, float c1,
				float cos0, float sin0,
				float cos1, float sin1,
				int& sign)
      {
	float expb0 = exp(b0);
	float exp2b0 = exp(2.0 * b0);

	float scale0 = 1 + exp2b0 - 2 * cos0 * expb0;
	float scaleA = 2 * a1 * sin0 * expb0 - a0 * (1 - exp2b0);

	float expb1 = exp(b1);
	float exp2b1 = exp(2.0 * b1);

	float scale1 = 1 + exp2b1 - 2 * cos1 * expb1;
	float scaleC = 2 * c1 * sin1 * expb1 - c0 * (1 - exp2b1);

	float sumA = scaleA / scale0;
	float sumC = scaleC / scale1;

	sign = 1;

	return (sumA + sumC);
      }

      inline
      float gaussian_1st_deriv_coef_norm_(float a0, float a1,
					  float b0, float b1,
					  float c0, float c1,
					  float cos0, float sin0,
					  float cos1, float sin1,
					  int& sign)
      {
	float expb0 = exp(b0);
	float exp2b0 = exp(2.0 * b0);

	float scale0 = 1 + exp2b0 - 2 * cos0 * expb0;
	scale0 *= scale0;
	float scaleA = - 2 * a1 * sin0 * expb0 * (1 - exp2b0) +
			 2 * a0 * expb0 * (2 * expb0 - cos0 * (1 + exp2b0));

	float expb1 = exp(b1);
	float exp2b1 = exp(2.0 * b1);

	float scale1 = 1 + exp2b1 - 2 * cos1 * expb1;
	scale1 *= scale1;
	float scaleC = - 2 * c1 * sin1 * expb1 * (1 - exp2b1) +
			 2 * c0 * expb1 * (2 * expb1 - cos1 * (1 + exp2b1));

	float sumA = scaleA / scale0;
	float sumC = scaleC / scale1;

	sign = -1;

	return (sumA + sumC);
      }

      inline
      float gaussian_2nd_deriv_coef_norm_(float a0, float a1,
					  float b0, float b1,
					  float c0, float c1,
					  float cos0, float sin0,
					  float cos1, float sin1,
					  int& sign)
      {
	float expb0 = exp(b0);
	float exp2b0 = exp(2.0 * b0);

	float scale0 = 1 + exp2b0 - 2 * cos0 * expb0;
	scale0 *= scale0 * scale0;

	float scaleA = a1 * sin0 * expb0 *
          (1 + expb0 * (2 * cos0 * (1 + exp2b0) + exp2b0 - 6)) +
          a0 * expb0 * (2 * expb0 * (2 - cos0 * cos0) *
                        (1 - exp2b0) - cos0 * (1 - exp2b0 * exp2b0));

	float expb1 = exp(b1);
	float exp2b1 = exp(2.0 * b1);

	float scale1 = 1 + exp2b1 - 2 * cos1 * expb1;
	scale1 *= scale1 * scale1;

	float scaleC = c1 * sin1 * expb1 *
          (1 + expb1 * (2 * cos1 * (1 + exp2b1) + exp2b1 - 6)) +
          c0 * expb1 * (2 * expb1 * (2 - cos1 * cos1) *
                        (1 - exp2b1) - cos1 * (1 - exp2b1 * exp2b1));

	float sumA = scaleA / scale0;
	float sumC = scaleC / scale1;
	sign = 1;

	return (sumA + sumC);
      }


      template <class I, class F>
      inline
      void
      generic_filter_(trait::image::dimension::one_d,
                      Image<I>& img_, const F& coef, int dir)
      {
	I& img = exact(img_);
        mln_precondition(dir < I::site::dim);


        recursivefilter_<mln_value(I)>(img, coef,
                                       point1d(-img.border()),
                                       point1d(geom::ninds(img) - 1 +
                                               img.border()),
                                       geom::ninds(img) + 2 * img.border(),
                                       dpoint1d(1));
      }

      template <class I, class F>
      inline
      void
      generic_filter_(trait::image::dimension::two_d,
                      Image<I>& img_, const F& coef, int dir)
      {
	I& img = exact(img_);

        mln_precondition(dir < I::site::dim);


        if (dir == 0)
        {
          // Apply on rows.
          for (unsigned j = 0; j < geom::ncols(img); ++j)
            recursivefilter_<mln_value(I)>(img, coef,
                                           point2d(-img.border(), j),
                                           point2d(geom::nrows(img) - 1 +
                                                   img.border(), j),
                                           geom::nrows(img) + 2 * img.border(),
                                           dpoint2d(1, 0));
        }

        if (dir == 1)
        {
          // Apply on columns.
          for (unsigned i = 0; i < geom::nrows(img); ++i)
            recursivefilter_<mln_value(I)>(img, coef,
                                           point2d(i, -img.border()),
                                           point2d(i, geom::ncols(img) - 1 +
                                                   img.border()),
                                           geom::ncols(img) + 2 * img.border(),
                                           dpoint2d(0, 1));
        }
      }

      template <class I, class F>
      inline
      void
      generic_filter_(trait::image::dimension::three_d,
                      Image<I>& img_, const F& coef, int dir)
      {
	I& img = exact(img_);
        mln_precondition(dir < I::site::dim);

        if (dir == 0)
        {
          // Apply on slices.
          for (unsigned j = 0; j < geom::nrows(img); ++j)
            for (unsigned k = 0; k < geom::ncols(img); ++k)
              recursivefilter_<mln_value(I)>(img, coef,
                                             point3d(-img.border(), j , k),
                                             point3d(geom::nslis(img) - 1 +
                                                     img.border(), j, k),
                                             geom::nslis(img) + 2 *
                                             img.border(),
                                             dpoint3d(1, 0, 0));
        }


        if (dir == 1)
        {
          // Apply on rows.
          for (unsigned i = 0; i < geom::nslis(img); ++i)
            for (unsigned k = 0; k < geom::ncols(img); ++k)
              recursivefilter_<mln_value(I)>(img, coef,
                                             point3d(i, -img.border(), k),
                                             point3d(i, geom::nrows(img) - 1 +
                                                     img.border(), k),
                                             geom::nrows(img) + 2 *
                                             img.border(),
                                             dpoint3d(0, 1, 0));
        }

        if (dir == 2)
        {
          // Apply on columns.
          for (unsigned i = 0; i < geom::nslis(img); ++i)
            for (unsigned j = 0; j < geom::nrows(img); ++i)
              recursivefilter_<mln_value(I)>(img, coef,
                                             point3d(i, j, -img.border()),
                                             point3d(i, j, geom::ncols(img) -
                                                     1 + img.border()),
                                             geom::ncols(img) + 2 *
                                             img.border(),
                                             dpoint3d(0, 0, 1));
        }
      }



      template <class I, class F, class O>
      inline
      void
      generic_filter_common_(trait::value::nature::floating,
                             const Image<I>& in,
                             const F& coef,
                             float sigma,
                             Image<O>& out)
      {
	mln_ch_value(O, float) work_img(exact(in).domain());
	data::paste(in, work_img);
	extension::adjust_fill(work_img, 4, 0);

	// On tiny sigma, Derich algorithm doesn't work.
	// It is the same thing that to convolve with a Dirac.
	if (sigma > 0.006)
          for (int i = 0; i < I::site::dim; ++i)
            generic_filter_(mln_trait_image_dimension(I)(),
                            work_img, coef, i);

        // We don't need to convert work_img
	data::paste(work_img, out);
      }

      template <class I, class F, class O>
      inline
      void
      generic_filter_common_(trait::value::nature::floating,
                             const Image<I>& in,
                             const F& coef,
                             float sigma,
                             Image<O>& out,
                             int dir)
      {
	mln_ch_value(O, float) work_img(exact(in).domain());
	data::paste(in, work_img);
	extension::adjust_fill(work_img, 4, 0);

	// On tiny sigma, Derich algorithm doesn't work.
	// It is the same thing that to convolve with a Dirac.
	if (sigma > 0.006)
	  generic_filter_(mln_trait_image_dimension(I)(),
                          work_img, coef, dir);

        // We don't need to convert work_img
	data::paste(work_img, out);
      }


      template <class I, class F, class O>
      inline
      void
      generic_filter_common_(trait::value::nature::scalar,
                             const Image<I>& in,
                             const F& coef,
                             float sigma,
                             Image<O>& out)
      {
	mln_ch_value(O, float) work_img(exact(in).domain());
	data::paste(in, work_img);
	extension::adjust_fill(work_img, 4, 0);

	// On tiny sigma, Derich algorithm doesn't work.
	// It is the same thing that to convolve with a Dirac.
	if (sigma > 0.006)
          for (int i = 0; i < I::site::dim; ++i)
            generic_filter_(mln_trait_image_dimension(I)(),
                            work_img, coef, i);

        // Convert work_img into result type
	data::paste(level::stretch(mln_value(I)(), work_img), out);
      }

      template <class I, class F, class O>
      inline
      void
      generic_filter_common_(trait::value::nature::scalar,
                             const Image<I>& in,
                             const F& coef,
                             float sigma,
                             Image<O>& out,
                             int dir)
      {
	mln_ch_value(O, float) work_img(exact(in).domain());
	data::paste(in, work_img);
	extension::adjust_fill(work_img, 4, 0);

	// On tiny sigma, Derich algorithm doesn't work.
	// It is the same thing that to convolve with a Dirac.
	if (sigma > 0.006)
	  generic_filter_(mln_trait_image_dimension(I)(),
                          work_img, coef, dir);

        // Convert work_img into result type
	data::paste(level::stretch(mln_value(I)(), work_img), out);
      }



      template <class I, class F, class O>
      inline
      void
      generic_filter_common_(trait::value::nature::vectorial,
                             const Image<I>& in,
                             const F& coef,
                             float sigma,
                             Image<O>& out)
      {
        // typedef algebra::vec<3, float> vec3f;
        // mln_ch_value(O, vec3f) work_img(exact(in).domain());
        // FIXME : paste does not work (rgb8 -> vec3f).
        data::paste(in, out);

	// On tiny sigma, Derich algorithm doesn't work.
	// It is the same thing that to convolve with a Dirac.
	if (sigma > 0.006)
          for (int i = 0; i < I::site::dim; ++i)
            generic_filter_(mln_trait_image_dimension(I)(),
                            out, coef, i);
      }

      template <class I, class F, class O>
      inline
      void
      generic_filter_common_(trait::value::nature::vectorial,
                             const Image<I>& in,
                             const F& coef,
                             float sigma,
                             Image<O>& out,
                             int dir)
      {
        // typedef algebra::vec<3, float> vec3f;
        // mln_ch_value(O, vec3f) work_img(exact(in).domain());
        // FIXME : paste does not work (rgb8 -> vec3f).
        data::paste(in, out);

	// On tiny sigma, Derich algorithm doesn't work.
	// It is the same thing that to convolve with a Dirac.
	if (sigma > 0.006)
	  generic_filter_(mln_trait_image_dimension(I)(),
                          out, coef, dir);
      }

    } // end of namespace mln::linear::impl

    // Facade.

    /*! Apply an approximated gaussian filter of \p sigma on \p input.
     * on a specific direction \p dir
     * if \p dir = 0, the filter is applied on the first image dimension.
     * if \p dir = 1, the filter is applied on the second image dimension.
     * And so on...
     *
     * \pre input.has_data
     * \pre dir < dimension(input)
     */
    template <class I>
    inline
    mln_concrete(I)
    gaussian(const Image<I>& input, float sigma, int dir)
    {
      mln_precondition(exact(input).has_data());
      mln_precondition(dir < I::site::dim);

      mln_concrete(I) output;
      initialize(output, input);
      impl::recursivefilter_coef_ coef(1.68f, 3.735f,
                                       1.783f, 1.723f,
                                       -0.6803f, -0.2598f,
                                       0.6318f, 1.997f,
                                       sigma, impl::gaussian_norm_coef_);

      impl::generic_filter_common_(mln_trait_value_nature(mln_value(I))(),
				   input, coef, sigma, output, dir);
      return output;
    }


    /*! Apply an approximated first derivative gaussian filter of \p sigma on
     * \p input.
     * on a specific direction \p dir
     * if \p dir = 0, the filter is applied on the first image dimension.
     * if \p dir = 1, the filter is applied on the second image dimension.
     * And so on...
     *
     * \pre input.has_data
     * \pre dir < dimension(input)
     */
    template <class I>
    inline
    mln_concrete(I)
    gaussian_1st_derivative(const Image<I>& input, float sigma, int dir)
    {
      mln_precondition(exact(input).has_data());
      mln_precondition(dir < I::site::dim);

      mln_concrete(I) output;
      initialize(output, input);

      impl::recursivefilter_coef_
        coef(-0.6472f, -4.531f,
             1.527f, 1.516f,
             0.6494f, 0.9557f,
             0.6719f, 2.072f,
             sigma, impl::gaussian_1st_deriv_coef_norm_);

      impl::generic_filter_common_(mln_trait_value_nature(mln_value(I))(),
				   input, coef, sigma, output, dir);
      return output;
    }

    /*! Apply an approximated second derivative gaussian filter of \p sigma on
     * \p input.
     * on a specific direction \p dir
     * if \p dir = 0, the filter is applied on the first image dimension.
     * if \p dir = 1, the filter is applied on the second image dimension.
     * And so on...
     *
     * \pre input.has_data
     * \pre dir < dimension(input)
     */
    template <class I>
    inline
    mln_concrete(I)
    gaussian_2nd_derivative(const Image<I>& input, float sigma, int dir)
    {
      mln_precondition(exact(input).has_data());
      mln_precondition(dir < I::site::dim);

      mln_concrete(I) output;
      initialize(output, input);

      impl::recursivefilter_coef_
        coef(-1.331f, 3.661f,
             1.24f, 1.314f,
             0.3225f, -1.738f,
             0.748f, 2.166f,
             sigma, impl::gaussian_2nd_deriv_coef_norm_);

      impl::generic_filter_common_(mln_trait_value_nature(mln_value(I))(),
				   input, coef, sigma, output, dir);
      return output;
    }





    /*! Apply an approximated gaussian filter of \p sigma on \p input.
     * This filter is applied in all the input image direction.
     *
     * \pre input.has_data
     */
    template <class I>
    inline
    mln_concrete(I)
    gaussian(const Image<I>& input, float sigma)
    {
      mln_precondition(exact(input).has_data());

      mln_concrete(I) output;
      initialize(output, input);

      impl::recursivefilter_coef_
	coef(1.68f, 3.735f,
	     1.783f, 1.723f,
	     -0.6803f, -0.2598f,
	     0.6318f, 1.997f,
	     sigma, impl::gaussian_norm_coef_);
      impl::generic_filter_common_(mln_trait_value_nature(mln_value(I))(),
				   input, coef, sigma, output);

      return output;
    }


    /*! Apply an approximated first derivative gaussian filter of \p sigma on
     * \p input
     * This filter is applied in all the input image direction.
     *
     * \pre input.has_data
     */
    template <class I>
    inline
    mln_concrete(I)
    gaussian_1st_derivative(const Image<I>& input, float sigma)
    {
      mln_precondition(exact(input).has_data());

      mln_concrete(I) output;
      initialize(output, input);

      impl::recursivefilter_coef_
        coef(-0.6472f, -4.531f,
             1.527f, 1.516f,
             0.6494f, 0.9557f,
             0.6719f, 2.072f,
             sigma, impl::gaussian_1st_deriv_coef_norm_);
      impl::generic_filter_common_(mln_trait_value_nature(mln_value(I))(),
				   input, coef, sigma, output);
      return output;
    }


    /*! Apply an approximated second derivative gaussian filter of \p sigma on
     * \p input
     * This filter is applied in all the input image direction.
     *
     * \pre input.has_data
     */
    template <class I>
    inline
    mln_concrete(I)
    gaussian_2nd_derivative(const Image<I>& input, float sigma)
    {
      mln_precondition(exact(input).has_data());

      mln_concrete(I) output;
      initialize(output, input);

      impl::recursivefilter_coef_
        coef(-1.331f, 3.661f,
             1.24f, 1.314f,
             0.3225f, -1.738f,
             0.748f, 2.166f,
             sigma, impl::gaussian_2nd_deriv_coef_norm_);
      impl::generic_filter_common_(mln_trait_value_nature(mln_value(I))(),
				   input, coef, sigma, output);
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::linear

} // end of namespace mln


#endif // ! MLN_LINEAR_GAUSSIAN_HH
