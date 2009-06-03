// Copyright (C) 2001, 2002, 2003, 2004, 2008, 2009 EPITA Research and
// Laboratory (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_LINEAR_GAUSSIAN_INTERNAL_COEFFICIENTS_HH
# define MLN_LINEAR_GAUSSIAN_INTERNAL_COEFFICIENTS_HH

/// \file mln/linear/gaussian/internal/coefficients.hh
///
/// Compute coefficients for recursive Gaussian filtering.
///
/// \todo Add a clean reference Rachid Deriche
///	 Recursively implementing the gaussian and its derivatives (1993)

# include <vector>
# include <cmath>



namespace mln
{

  namespace linear
  {

    namespace gaussian
    {

      namespace internal
      {


	typedef double norm_fun(double, double,
				double, double,
				double, double,
				double, double,
				double, double,
				int&);

	struct coefficients
	{
	  /// Constructor.
	  //
	  coefficients(double a0, double a1,
		      double b0, double b1,
		      double c0, double c1,
		      double w0, double w1,
		      double s, norm_fun norm);
	  std::vector<double> n, d, nm, dm;
	};


	coefficients coefficients_not_derivative(double sigma);
	coefficients coefficients_1st_derivative(double sigma);
	coefficients coefficients_2nd_derivative(double sigma);



# ifndef MLN_INCLUDE_ONLY


	inline
	coefficients::coefficients(double a0, double a1,
				 double b0, double b1,
				 double c0, double c1,
				 double w0, double w1,
				 double s, norm_fun norm)
	{
	  n.reserve(5);
	  d.reserve(5);
	  nm.reserve(5);
	  dm.reserve(5);

	  b0 /= s;
	  b1 /= s;
	  w0 /= s;
	  w1 /= s;

	  double sin0 = std::sin(w0);
	  double sin1 = std::sin(w1);
	  double cos0 = std::cos(w0);
	  double cos1 = std::cos(w1);

	  int sign = 1;
	  double n_ = norm(a0, a1, b0, b1, c0, c1, cos0, sin0, cos1, sin1, sign);

	  a0 /= n_;
	  a1 /= n_;
	  c0 /= n_;
	  c1 /= n_;

	  n[3] =
	    std::exp(-b1 - 2*b0) * (c1 * sin1 - cos1 * c0) +
	    std::exp(-b0 - 2*b1) * (a1 * sin0 - cos0 * a0);
	  n[2] =
	    2 * std::exp(-b0 - b1) * ((a0 + c0) * cos1 * cos0 -
				      cos1 * a1 * sin0 -
				      cos0 * c1 * sin1) +
	    c0 * std::exp(-2*b0) + a0 * std::exp(-2*b1);
	  n[1] =
	    std::exp(-b1) * (c1 * sin1 - (c0 + 2 * a0) * cos1) +
	    std::exp(-b0) * (a1 * sin0 - (2 * c0 + a0) * cos0);
	  n[0] =
	    a0 + c0;

	  d[4] =
	    std::exp(-2 * b0 - 2 * b1);
	  d[3] =
	    -2 * cos0 * std::exp(-b0 - 2*b1) -
	    2 * cos1 * std::exp(-b1 - 2*b0);
	  d[2] =
	    4 * cos1 * cos0 * std::exp(-b0 - b1) +
	    std::exp(-2*b1) + std::exp(-2*b0);
	  d[1] =
	    -2 * std::exp(-b1) * cos1 - 2 * std::exp(-b0) * cos0;

	  for (unsigned i = 1; i <= 3; ++i)
	    {
	      dm[i] = d[i];
	      nm[i] = double(sign) * (n[i] - d[i] * n[0]);
	    }
	  dm[4] = d[4];
	  nm[4] = double(sign) * (-d[4] * n[0]);
	}



	inline
	double norm_not_derivative(double a0, double a1,
				   double b0, double b1,
				   double c0, double c1,
				   double cos0, double sin0,
				   double cos1, double sin1,
				   int& sign)
	{
	  double expb0 = std::exp(b0);
	  double exp2b0 = std::exp(2.f * b0);

	  double scale0 = 1 + exp2b0 - 2 * cos0 * expb0;
	  double scaleA = 2 * a1 * sin0 * expb0 - a0 * (1 - exp2b0);

	  double expb1 = std::exp(b1);
	  double exp2b1 = std::exp(2.f * b1);

	  double scale1 = 1 + exp2b1 - 2 * cos1 * expb1;
	  double scaleC = 2 * c1 * sin1 * expb1 - c0 * (1 - exp2b1);

	  double sumA = scaleA / scale0;
	  double sumC = scaleC / scale1;

	  sign = 1;

	  return sumA + sumC;
	}


	inline
	double norm_1st_derivative(double a0, double a1,
				   double b0, double b1,
				   double c0, double c1,
				   double cos0, double sin0,
				   double cos1, double sin1,
				   int& sign)
	{
	  double expb0 = std::exp(b0);
	  double exp2b0 = std::exp(2.f * b0);

	  double scale0 = 1 + exp2b0 - 2 * cos0 * expb0;
	  scale0 *= scale0;
	  double scaleA = - 2 * a1 * sin0 * expb0 * (1 - exp2b0) +
	    2 * a0 * expb0 * (2 * expb0 - cos0 * (1 + exp2b0));

	  double expb1 = std::exp(b1);
	  double exp2b1 = std::exp(2.f * b1);

	  double scale1 = 1 + exp2b1 - 2 * cos1 * expb1;
	  scale1 *= scale1;
	  double scaleC = - 2 * c1 * sin1 * expb1 * (1 - exp2b1) +
	    2 * c0 * expb1 * (2 * expb1 - cos1 * (1 + exp2b1));

	  double sumA = scaleA / scale0;
	  double sumC = scaleC / scale1;

	  sign = -1;

	  return sumA + sumC;
	}


	inline
	double norm_2nd_derivative(double a0, double a1,
				   double b0, double b1,
				   double c0, double c1,
				   double cos0, double sin0,
				   double cos1, double sin1,
				   int& sign)
	{
	  double expb0 = std::exp(b0);
	  double exp2b0 = std::exp(2.f * b0);

	  double scale0 = 1 + exp2b0 - 2 * cos0 * expb0;
	  scale0 *= scale0 * scale0;

	  double scaleA = a1 * sin0 * expb0 *
	    (1 + expb0 * (2 * cos0 * (1 + exp2b0) + exp2b0 - 6)) +
	    a0 * expb0 * (2 * expb0 * (2 - cos0 * cos0) *
			  (1 - exp2b0) - cos0 * (1 - exp2b0 * exp2b0));

	  double expb1 = std::exp(b1);
	  double exp2b1 = std::exp(2.f * b1);

	  double scale1 = 1 + exp2b1 - 2 * cos1 * expb1;
	  scale1 *= scale1 * scale1;

	  double scaleC = c1 * sin1 * expb1 *
	    (1 + expb1 * (2 * cos1 * (1 + exp2b1) + exp2b1 - 6)) +
	    c0 * expb1 * (2 * expb1 * (2 - cos1 * cos1) *
			  (1 - exp2b1) - cos1 * (1 - exp2b1 * exp2b1));

	  double sumA = scaleA / scale0;
	  double sumC = scaleC / scale1;
	  sign = 1;

	  return sumA + sumC;
	}


	inline
	coefficients coefficients_not_derivative(double sigma)
	{
	  coefficients tmp(+1.6800, +3.7350,
			  +1.7830, +1.7230,
			  -0.6803, -0.2598,
			  +0.6318, +1.9970,
			  sigma,
			  norm_not_derivative);
	  return tmp;
	}


	inline
	coefficients coefficients_1st_derivative(double sigma)
	{
	  coefficients tmp(-0.6472, -4.5310,
			  +1.5270, +1.5160,
			  +0.6494, +0.9557,
			  +0.6719, +2.0720,
			  sigma,
			  norm_1st_derivative);
	  return tmp;
	}


	inline
	coefficients coefficients_2nd_derivative(double sigma)
	{
	  coefficients tmp(-1.3310, +3.661,
			  +1.2400, +1.314,
			  +0.3225, -1.738,
			  +0.7480, +2.166,
			  sigma,
			  norm_2nd_derivative);
	  return tmp;
	}

# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace mln::linear::gaussian::internal

    } // end of namespace mln::linear::gaussian

  } // end of namespace mln::linear

} // end of namespace mln


#endif // ! MLN_LINEAR_GAUSSIAN_INTERNAL_COEFFICIENTS_HH
