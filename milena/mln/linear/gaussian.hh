// Copyright (C) 2001, 2002, 2003, 2004 EPITA Research and Development
// Laboratory
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

/*! \file mln/linear/gaussian.hh
 *
 * \brief Gaussian filter.
 */

# include <mln/core/concept/image.hh>
# include <mln/core/point2d.hh>
# include <mln/level/paste.hh>
# include <mln/geom/ncols.hh>
# include <mln/geom/nrows.hh>

# include <mln/algebra/vec.hh>

# include <vector>
# include <cmath>

namespace mln
{

  namespace linear
  {

    /*! Gaussian filter of an image \p input
     *
     * \pre output.domain = input.domain
     */
    template <class I, class O>
    void
    gaussian(const Image<I>& input, float sigma,
	     Image<O>& output);

# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      struct recursivefilter_coef_
      {

	/*!
	** \brief Constructor.
	*/
	recursivefilter_coef_(float a0, float a1,
			      float b0, float b1,
			      float c0, float c1,
			      float w0, float w1,
			      float s);
	std::vector<float> n, d, nm, dm;
	float sumA, sumC;
      };

      inline
      recursivefilter_coef_::recursivefilter_coef_(float a0, float a1,
						   float b0, float b1,
						   float c0, float c1,
						   float w0, float w1,
						   float s)
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

	sumA  =
	  (2.0 * a1 * exp( b0 ) * cos0 * cos0 - a0 * sin0 * exp( 2.0 * b0 )
	   + a0 * sin0 - 2.0 * a1 * exp( b0 )) /
	  (( 2.0 * cos0 * exp( b0 ) - exp( 2.0 * b0 ) - 1 ) * sin0);

	sumC  =
	  (2.0 * c1 * exp( b1 ) * cos1 * cos1 - c0 * sin1 * exp( 2.0 * b1 )
	   + c0 * sin1 - 2.0 * c1 * exp( b1 ))
	  / (( 2.0 * cos1 * exp( b1 ) - exp( 2.0 * b1 ) - 1 ) * sin1);

	a0 /= (sumA + sumC);
	a1 /= (sumA + sumC);
	c0 /= (sumA + sumC);
	c1 /= (sumA + sumC);

	n[3] =
	  exp( -b1 - 2*b0 ) * (c1 * sin1 - cos1 * c0) +
	  exp( -b0 - 2*b1 ) * (a1 * sin0 - cos0 * a0);
	n[2] =
	  2 * exp(-b0 - b1) * ((a0 + c0) * cos1 * cos0 -
			       cos1 * a1 * sin0 -
			       cos0 * c1 * sin1) +
	  c0 * exp(-2 * b0) + a0 * exp(-2 * b1);
	n[1] =
	  exp(-b1) * (c1 * sin1 - (c0 + 2*a0) * cos1) +
	  exp(-b0) * (a1 * sin0 - (2*c0 + a0) * cos0);
	n[0] =
	  a0 + c0;

	d[4] = exp(-2 * b0 - 2 * b1);
	d[3] =
	  -2 * cos0 * exp(-b0 - 2*b1) -
	  2 * cos1 * exp(-b1 - 2*b0);
	d[2] =
	  4 * cos1 * cos0 * exp(-b0 - b1) +
	  exp(-2*b1) + exp(-2*b0);
	d[1] =
	  -2*exp(-b1) * cos1 - 2 * exp(-b0) * cos0;

	for (unsigned i = 1; i <= 3; ++i)
	  {
	    dm[i] = d[i];
	    nm[i] = n[i] - d[i] * n[0];
	  }
	dm[4] = d[4];
	nm[4] = -d[4] * n[0];
      }



      template <class WorkType, class I>
      inline
      void
      recursivefilter_(I& ima,
		       const recursivefilter_coef_& c,
		       const mln_psite(I)& start,
		       const mln_psite(I)& finish,
		       int len,
		       const mln_dpsite(I)& d)
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
	  - c.d[1] * tmp1[2] - c.d[2]*tmp1[1]
	  - c.d[3] * tmp1[0];

	mln_psite(I) current(start + d + d + d + d);
	for (mln_coord(I) i = 4; i < len; ++i)
	  {
	    tmp1[i] =
	      c.n[0] * ima(current)
	      + c.n[1] * ima(current - d)
	      + c.n[2] * ima(current - d - d)
	      + c.n[3] * ima(current - d - d - d)
	      - c.d[1] * tmp1[i - 1] - c.d[2]*tmp1[i - 2]
	      - c.d[3] * tmp1[i - 3] - c.d[4]*tmp1[i - 4];
	    current = current + d;
	  }

	// Non causal part

	tmp2[len - 1] = WorkType(); // FIXME : = 0, literal::zero ...?

	tmp2[len - 2] =
	  c.nm[1] * ima(finish);

	tmp2[len - 3] =
	  c.nm[1] * ima(finish - d)
	  + c.nm[2] * ima(finish)
	  - c.dm[1] * tmp2[len-2];

	tmp2[len - 4] =
	  c.nm[1] * ima(finish - d - d)
	  + c.nm[2] * ima(finish - d)
	  + c.nm[3] * ima(finish)
	  - c.dm[1] * tmp2[len-3]
	  - c.dm[2] * tmp2[len-2];

	current = finish - d - d - d ;

	for (int i = len - 5; i >= 0; --i)
	  {
	    tmp2[i] =
	      c.nm[1] * ima(current)
	      + c.nm[2] * ima(current + d)
	      + c.nm[3] * ima(current + d + d)
	      + c.nm[4] * ima(current + d + d + d)
	      - c.dm[1] * tmp2[i+1] - c.dm[2]*tmp2[i+2]
	      - c.dm[3] * tmp2[i+3] - c.dm[4]*tmp2[i+4];
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


      template <class I, class F>
      inline
      void
      gaussian_(Image<I>& img_, const F& coef)
      {
	I& img = exact(img_);

	// Apply on rows.
	for (unsigned j = 0; j < geom::ncols(img); ++j)
	  recursivefilter_< mln_value(I) >(img, coef,
				  make::point2d(-img.border(), j),
				  make::point2d(geom::nrows(img) - 1 + img.border(), j),
				  geom::nrows(img) + 2 * img.border(),
				  make::dpoint2d(1, 0));

	// Apply on columns.
	for (unsigned i = 0; i < geom::nrows(img); ++i)
	  recursivefilter_< mln_value(I) >(img, coef,
				  make::point2d(i, -img.border()),
				  make::point2d(i, geom::ncols(img) - 1 + img.border()),
				  geom::ncols(img) + 2 * img.border(),
				  make::dpoint2d(0, 1));

      }


      template <class I, class F, class O>
      inline
      void
      gaussian_common_(trait::value::nature::scalar,
                       const Image<I>& in,
		       const F& coef,
		       float sigma,
                       Image<O>& out)
      {
      	mln_ch_value(O, float) work_img(exact(in).domain());
      	level::paste(in, work_img);
      
      	// On tiny sigma, Derich algorithm doesn't work.
      	// It is the same thing that to convolve with a Dirac.
      	if (sigma > 0.006)
      	  gaussian_(work_img, coef);
      	/* Convert the result image to the user-requested datatype.
      	   FIXME: We are making an unnecessary copy in case the
      	   user expects a ntg::float_s image.  */
      	level::paste(work_img, out);
      }


      template <class I, class F, class O>
      inline
      void
      gaussian_common_(trait::value::nature::vectorial,
                       const Image<I>& in,
		       const F& coef,
		       float sigma,
                       Image<O>& out)
      {
        // typedef algebra::vec<3, float> vec3f;
        // mln_ch_value(O, vec3f) work_img(exact(in).domain());
        // FIXME : paste does not work (rgb8 -> vec3f).
        level::paste(in, out);

	if (sigma > 0.006)
	  gaussian_(out, coef);
      }
      
    } // end of namespace mln::linear::impl
    
    // Facade.

    template <class I, class O>
    inline
    void
    gaussian(const Image<I>& input, float sigma,
	     Image<O>& output)
    {
      mln_precondition(exact(input).has_data());
      mln_precondition(exact(output).has_data());
      
      impl::recursivefilter_coef_
	coef(1.68f, 3.735f,
	     1.783f, 1.723f,
	     -0.6803f, -0.2598f,
	     0.6318f, 1.997f,
	     sigma);
      impl::gaussian_common_(mln_trait_value_nature(mln_value(I))(),
                             input, coef, sigma, output);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::linear

} // end of namespace mln


#endif // ! MLN_LINEAR_GAUSSIAN_HH
