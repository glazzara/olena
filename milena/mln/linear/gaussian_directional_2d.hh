// Copyright (C) 2009, 2012 EPITA Research and Development Laboratory
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

#ifndef MLN_LINEAR_GAUSSIAN_DIRECTIONAL_2D_HH
# define MLN_LINEAR_GAUSSIAN_DIRECTIONAL_2D_HH

/// \file
///
/// Directional Gaussian filter for 2D images.
///
/// \todo This is experimental code to be merged with regular code...

#include <mln/core/image/image2d.hh>
#include <mln/extension/adjust_fill.hh>
#include <mln/extension/adjust_duplicate.hh>



namespace mln
{

  namespace linear
  {

    /*! \brief Directional Gaussian filter for 2D images.
      \ingroup mlnlinear
    */
    template <typename I>
    mln_concrete(I)
    gaussian_directional_2d(const Image<I>& input,
			    unsigned dir, double sigma,
			    const mln_value(I)& bdr);



# ifndef MLN_INCLUDE_ONLY

    namespace my
    {

      struct recursivefilter_coef_
      {
	enum FilterType { DericheGaussian,
			  DericheGaussianFirstDerivative,
			  DericheGaussianSecondDerivative };

	std::vector<double> n, d, nm, dm;
	double sumA, sumC;

	recursivefilter_coef_(double a0, double a1,
			      double b0, double b1,
			      double c0, double c1,
			      double w0, double w1,
			      double s, FilterType filter_type)
	{
	  n.reserve(5);
	  d.reserve(5);
	  nm.reserve(5);
	  dm.reserve(5);

	  b0 /= s;
	  b1 /= s;
	  w0 /= s;
	  w1 /= s;

	  double sin0	= sin(w0);
	  double sin1	= sin(w1);
	  double cos0	= cos(w0);
	  double cos1	= cos(w1);

	  switch (filter_type) {

	  case DericheGaussian :
	    {
	      sumA  =
		(2.0 * a1 * exp( b0 ) * cos0 * cos0 - a0 * sin0 * exp( 2.0 * b0 )
		 + a0 * sin0 - 2.0 * a1 * exp( b0 )) /
		(( 2.0 * cos0 * exp( b0 ) - exp( 2.0 * b0 ) - 1 ) * sin0);

	      sumC  =
		(2.0 * c1 * exp( b1 ) * cos1 * cos1 - c0 * sin1 * exp( 2.0 * b1 )
		 + c0 * sin1 - 2.0 * c1 * exp( b1 ))
		/ (( 2.0 * cos1 * exp( b1 ) - exp( 2.0 * b1 ) - 1 ) * sin1);
	      break;
	    }

	  case DericheGaussianFirstDerivative :
	    {
	      sumA  = -2.f *
		(a0 * cos0 - a1 * sin0 + a1 * sin0 * exp( 2.0 * b0 )
		 + a0 * cos0 * exp( 2.0 * b0 ) - 2.0 * a0 * exp( b0 ))
		* exp( b0 )
		/ (exp( 4.0 * b0 ) - 4.0 * cos0 * exp( 3.0 * b0 )
		   + 2.0 * exp( 2.0 * b0 ) + 4.0 * cos0 * cos0 * exp( 2.0 * b0 )
		   + 1.0 - 4.0 * cos0 * exp( b0 ));
	      sumC  = - 2.f *
		(c0 * cos1 - c1 * sin1 + c1 * sin1 * exp( 2.0 * b1 )
		 + c0 * cos1 * exp( 2.0 * b1 ) - 2.0 * c0 * exp( b1 ))
		* exp( b1 ) /
		(exp( 4.0 * b1 ) - 4.0 * cos1 * exp( 3.0 * b1 )
		 + 2.0 * exp( 2.0 * b1 ) + 4.0 * cos1 * cos1 * exp( 2.0 * b1 )
		 + 1.0 - 4.0 * cos1 * exp( b1 ));
	      break;
	    }

	  case DericheGaussianSecondDerivative :
	    {
	      double aux;
	      aux   =
		12.0 * cos0 * exp( 3.0 * b0 ) - 3.0 * exp( 2.0 * b0 )
		+ 8.0 * cos0 * cos0 * cos0 * exp( 3.0 * b0 ) - 12.0 * cos0 * cos0 *
		exp( 4.0 * b0 )
		- (3.0 * exp( 4.0 * b0 ))
		+ 6.0 * cos0 * exp( 5.0 * b0 ) -  exp( 6.0 * b0 ) + 6.0 * cos0 * exp
		( b0 )
		- ( 1.0 + 12.0 * cos0 * cos0 * exp( 2.0 * b0 ) );
	      sumA  =
		4.0 * a0 * sin0 * exp( 3.0 * b0 ) + a1 * cos0 * cos0 * exp( 4.0 * b0 )
		- ( 4.0 * a0 * sin0 * exp( b0 ) + 6.0 * a1 * cos0 * cos0 * exp( 2.0 * b0 ) )
		+ 2.0 * a1 * cos0 * cos0 * cos0 * exp( b0 ) - 2.0 * a1 * cos0 * exp(b0)
		+ 2.0 * a1 * cos0 * cos0 * cos0 * exp( 3.0 * b0 ) - 2.0 * a1 * cos0
		* exp( 3.0 * b0 )
		+ a1 * cos0 * cos0 - a1 * exp( 4.0 * b0 )
		+ 2.0 * a0 * sin0 * cos0 * cos0 * exp( b0 ) - 2.0 * a0 * sin0 * cos0
		* cos0 * exp( 3.0 * b0 )
		- ( a0 * sin0 * cos0 * exp( 4.0 * b0 ) + a1 )
		+ 6.0 * a1 * exp( 2.0 * b0 ) + a0 * cos0 * sin0
		* 2.0 * exp( b0 ) / ( aux * sin0 );
	      aux   =
		12.0 * cos1 * exp( 3.0 * b1 ) - 3.0 * exp( 2.0 * b1 )
		+ 8.0 * cos1 * cos1 * cos1 * exp( 3.0 * b1 ) - 12.0 * cos1 * cos1 *
		exp( 4.0 * b1 )
		- 3.0 * exp( 4.0 * b1 )
		+ 6.0 * cos1 * exp( 5.0 * b1 ) -  exp( 6.0 * b1 ) + 6.0 * cos1 * exp
		( b1 )
		- ( 1.0 + 12.0 * cos1 * cos1 * exp( 2.0 * b1 ) );
	      sumC  = 4.0 * c0 * sin1 * exp( 3.0 * b1 ) + c1 * cos1 * cos1 * exp( 4.0 * b1 )
		- ( 4.0 * c0 * sin1 * exp( b1 ) + 6.0 * c1 * cos1 * cos1 * exp( 2.0 * b1 ) )
		+ 2.0 * c1 * cos1 * cos1 * cos1 * exp( b1 ) - 2.0 * c1 * cos1 * exp( b1 )
		+ 2.0 * c1 * cos1 * cos1 * cos1 * exp( 3.0 * b1 ) - 2.0 * c1 * cos1
		* exp( 3.0 * b1 )
		+ c1 * cos1 * cos1 - c1 * exp( 4.0 * b1 )
		+ 2.0 * c0 * sin1 * cos1 * cos1 * exp( b1 ) - 2.0 * c0 * sin1 * cos1
		* cos1 * exp( 3.0 * b1 )
		- ( c0 * sin1 * cos1 * exp( 4.0 * b1 ) + c1 )
		+ 6.0 * c1 * exp( 2.0 * b1 ) + c0 * cos1 * sin1
		* 2.0 * exp( b1 ) / ( aux * sin1 );
	      sumA /= 2;
	      sumC /= 2;
	      break;
	    }
	  }

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

	  switch (filter_type) {
	  case DericheGaussian :
	  case DericheGaussianSecondDerivative :
	    {
	      for (unsigned i = 1; i <= 3; ++i)
		{
		  dm[i] = d[i];
		  nm[i] = n[i] - d[i] * n[0];
		}
	      dm[4] = d[4];
	      nm[4] = -d[4] * n[0];
	      break;
	    }
	  case DericheGaussianFirstDerivative :
	    {
	      for (unsigned i = 1; i <= 3; ++i)
		{
		  dm[i] = d[i];
		  nm[i] = - (n[i] - d[i] * n[0]);
		}
	      dm[4] = d[4];
	      nm[4] = d[4] * n[0];
	      break;
	    }
	  }
	}
      };


    } // end of namespace mln::linear::my




    // FIXME: in the "generic" code below there is no test that we
    // actually stay in the image domain...


    template <typename I, typename C>
    inline
    void
    recursivefilter_directional_generic(I& ima,
					const C& c,
					const mln_psite(I)& start,
					const mln_psite(I)& finish,
					int len,
					const mln_deduce(I, psite, delta)& d)
    {
      std::vector<double> tmp1(len);
      std::vector<double> tmp2(len);

      tmp1[0] =
	c.n[0]*ima(start);

      tmp1[1] =
	c.n[0]*ima(start + d)
	+ c.n[1]*ima(start)
	- c.d[1]*tmp1[0];

      tmp1[2] =
	c.n[0]*ima(start + d + d)
	+ c.n[1]*ima(start + d)
	+ c.n[2]*ima(start)
	- c.d[1]*tmp1[1]
	- c.d[2]*tmp1[0];

      tmp1[3] =
	c.n[0]*ima(start + d + d + d)
	+ c.n[1]*ima(start + d + d)
	+ c.n[2]*ima(start + d)
	+ c.n[3]*ima(start)
	- c.d[1]*tmp1[2] - c.d[2]*tmp1[1]
	- c.d[3]*tmp1[0];

      mln_site(I) current = start + d + d + d + d;
      for (int i = 4; i < len; ++i)
	{
	  tmp1[i] =
	    c.n[0]*ima(current)
	    + c.n[1]*ima(current - d)
	    + c.n[2]*ima(current - d - d)
	    + c.n[3]*ima(current - d - d - d)
	    - c.d[1]*tmp1[i - 1] - c.d[2]*tmp1[i - 2]
	    - c.d[3]*tmp1[i - 3] - c.d[4]*tmp1[i - 4];
	  current += d;
	}

      // Non causal part

      tmp2[len - 1] = 0;

      tmp2[len - 2] =
	c.nm[1]*ima(finish);

      tmp2[len - 3] =
	c.nm[1]*ima(finish - d)
	+ c.nm[2]*ima(finish)
	- c.dm[1]*tmp2[len-2];

      tmp2[len - 4] =
	c.nm[1]*ima(finish - d - d)
	+ c.nm[2]*ima(finish - d)
	+ c.nm[3]*ima(finish)
	- c.dm[1]*tmp2[len-3]
	- c.dm[2]*tmp2[len-2];

      current = finish - d - d - d ;

      for (int i = len - 5; i >= 0; --i)
	{
	  tmp2[i] =
	    c.nm[1]*ima(current)
	    + c.nm[2]*ima(current + d)
	    + c.nm[3]*ima(current + d + d)
	    + c.nm[4]*ima(current + d + d + d)
	    - c.dm[1]*tmp2[i+1] - c.dm[2]*tmp2[i+2]
	    - c.dm[3]*tmp2[i+3] - c.dm[4]*tmp2[i+4];
	  current -= d;
	}

      // Combine results from causal and non-causal parts.

      current = start;
      for (int i = 0; i < len; ++i)
	{
	  ima(current) = (tmp1[i] + tmp2[i]);
	  current += d;
	}
    }




    template <typename I, typename C>
    inline
    void
    recursivefilter_directional_fastest(I& ima,
					const C& c,
					const mln_psite(I)& start,
					const mln_psite(I)& finish,
					int len,
					const mln_deduce(I, psite, delta)& d,
					const mln_value(I)& bdr)
    {
      //       extension::adjust_fill(ima, 5 * int(151 + .50001) + 1, bdr);
      // extension::fill(ima, bdr);

      std::vector<double> tmp1(len);
      std::vector<double> tmp2(len);

      unsigned delta_offset = ima.delta_offset(d);
      unsigned
	o_start = ima.offset_of_point(start),
	o_start_d   = o_start +     delta_offset,
	o_start_dd  = o_start + 2 * delta_offset,
	o_start_ddd = o_start + 3 * delta_offset;

      tmp1[0] =
	c.n[0] * ima.element(o_start);

      tmp1[1] = 0
	+ c.n[0] * ima.element(o_start_d)
	+ c.n[1] * ima.element(o_start)
	- c.d[1] * tmp1[0];

      tmp1[2] = 0
	+ c.n[0] * ima.element(o_start_dd)
	+ c.n[1] * ima.element(o_start_d)
	+ c.n[2] * ima.element(o_start)
	- c.d[1] * tmp1[1]
	- c.d[2] * tmp1[0];

      tmp1[3] = 0
	+ c.n[0] * ima.element(o_start_ddd)
	+ c.n[1] * ima.element(o_start_dd)
	+ c.n[2] * ima.element(o_start_d)
	+ c.n[3] * ima.element(o_start)
	- c.d[1] * tmp1[2] - c.d[2] * tmp1[1]
	- c.d[3] * tmp1[0];

      unsigned
	o_current = o_start + 4 * delta_offset,
	o_current_d   = o_current -     delta_offset,
	o_current_dd  = o_current - 2 * delta_offset,
	o_current_ddd = o_current - 3 * delta_offset;

      for (int i = 4; i < len; ++i)
	{
	  tmp1[i] = 0
	    + c.n[0] * ima.element(o_current)
	    + c.n[1] * ima.element(o_current_d)
	    + c.n[2] * ima.element(o_current_dd)
	    + c.n[3] * ima.element(o_current_ddd)
	    - c.d[1] * tmp1[i - 1] - c.d[2] * tmp1[i - 2]
	    - c.d[3] * tmp1[i - 3] - c.d[4] * tmp1[i - 4];
	  o_current     += delta_offset;
	  o_current_d   += delta_offset;
	  o_current_dd  += delta_offset;
	  o_current_ddd += delta_offset;
	}

      // Non causal part

      (void) bdr;
      // extension::fill(ima, bdr);

      unsigned
	o_finish   = ima.offset_of_point(finish),
	o_finish_d  = o_finish -     delta_offset,
	o_finish_dd = o_finish - 2 * delta_offset;

      tmp2[len - 1] = 0;

      tmp2[len - 2] =
	c.nm[1] * ima.element(o_finish);

      tmp2[len - 3] = 0
	+ c.nm[1] * ima.element(o_finish_d)
	+ c.nm[2] * ima.element(o_finish)
	- c.dm[1] * tmp2[len-2];

      tmp2[len - 4] = 0
	+ c.nm[1] * ima.element(o_finish_dd)
	+ c.nm[2] * ima.element(o_finish_d)
	+ c.nm[3] * ima.element(o_finish)
	- c.dm[1] * tmp2[len-3]
	- c.dm[2] * tmp2[len-2];

      o_current = o_finish - 3 * delta_offset;
      o_current_d   = o_current +     delta_offset;
      o_current_dd  = o_current + 2 * delta_offset;
      o_current_ddd = o_current + 3 * delta_offset;

      for (int i = len - 5; i >= 0; --i)
	{
	  tmp2[i] = 0
	    + c.nm[1] * ima.element(o_current)
	    + c.nm[2] * ima.element(o_current_d)
	    + c.nm[3] * ima.element(o_current_dd)
	    + c.nm[4] * ima.element(o_current_ddd)
	    - c.dm[1] * tmp2[i+1] - c.dm[2] * tmp2[i+2]
	    - c.dm[3] * tmp2[i+3] - c.dm[4] * tmp2[i+4];
	  o_current     -= delta_offset;
	  o_current_d   -= delta_offset;
	  o_current_dd  -= delta_offset;
	  o_current_ddd -= delta_offset;
	}

      // Combine results from causal and non-causal parts.

      o_current = o_start;
      for (int i = 0; i < len; ++i)
	{
	  ima.element(o_current) = static_cast<mln_value(I)>(tmp1[i] + tmp2[i]);
	  o_current += delta_offset;
	}
    }


    template <typename I>
    inline
    mln_concrete(I)
    gaussian_directional_2d(const Image<I>& input_,
			    unsigned dir, double sigma,
			    const mln_value(I)& bdr)
    {
      trace::entering("linear::gaussian_directional_2d");

      typedef mln_site(I) P;
      mlc_bool(P::dim == 2)::check();

      const I& input = exact(input_);

      mln_precondition(dir == 0 || dir == 1);
      mln_precondition(input.is_valid());

      my::recursivefilter_coef_ coef(1.68f, 3.735f,
				     1.783f, 1.723f,
				     -0.6803f, -0.2598f,
				     0.6318f, 1.997f,
				     sigma,
				     my::recursivefilter_coef_::DericheGaussian);

      extension::adjust_fill(input, 5 * int(sigma + .50001) + 1, bdr);
      mln_concrete(I) output = duplicate(input);

      if (sigma < 0.006)
	return output;

      int
	nrows = geom::nrows(input),
	ncols = geom::ncols(input),
	b     = input.border();

      if (dir == 0)
	{
	  for (int j = 0; j < ncols; ++j)
	    recursivefilter_directional_fastest(output, coef,
						point2d(- b, j),
						point2d(nrows - 1 + b, j),
						nrows + 2 * b,
						dpoint2d(1, 0),
						bdr);
	}

      if (dir == 1)
	{
	  for (int i = 0; i < nrows; ++i)
	    recursivefilter_directional_fastest(output, coef,
						point2d(i, - b),
						point2d(i, ncols - 1 + b),
						ncols + 2 * b,
						dpoint2d(0, 1),
						bdr);
	}

      trace::exiting("linear::gaussian_directional_2d");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::linear

} // end of namespace mln


#endif // ! MLN_LINEAR_GAUSSIAN_DIRECTIONAL_2D_HH
