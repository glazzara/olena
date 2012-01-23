// Copyright (C) 2007, 2008, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_ALGEBRA_H_MAT_HH
# define MLN_ALGEBRA_H_MAT_HH

/*! \file
 *
 * \brief Definition of a matrix with homogeneous coordinates.
 *
 * \todo Add traits.
 *
 * FIXME: there is a big issue with this class. exact() does not
 * return the correct type since the exact type is NOT correctly
 * propagated to Object. mat and h_mat should inherit from a base
 * class and the operator traits should be updated so that
 * interoperability between mat and h_mat is preserved which is not
 * obvious.
 *
 */

# include <mln/algebra/mat.hh>
# include <mln/algebra/quat.hh>

# include <mln/math/pi.hh>
# include <mln/util/couple.hh>

namespace mln
{

  namespace algebra
  {

    /*! \brief N-Dimensional matrix with homogeneous coordinates.
      \ingroup mlnalgebratypes
     */
    template <unsigned d, typename T>
    struct h_mat : public mat<d+1, d+1, T>
    {
      /// Dimension is the 'natural' one (3 for 3D), not the one of the vector (dim + 1)
      enum { N = d,
	     M = d,
	     dim = d * d };

      /// Constructor without argument.
      h_mat();
      /// Constructor with the underlying matrix.
      h_mat(const mat<d+1, d+1, T>& x);

    };


    /// \internal Conversion: h_mat -> quat
    template <typename C>
    void from_to_(const algebra::h_mat<3,C>& from, algebra::quat& to);


# ifndef MLN_INCLUDE_ONLY

    template <unsigned d, typename T>
    inline
    h_mat<d,T>::h_mat()
      : mat<d+1, d+1, T>(mat<d+1, d+1, T>::Id)
    {
    }

    template <unsigned d, typename T>
    inline
    h_mat<d,T>::h_mat(const mat<d+1, d+1, T>& x)
      : mat<d+1, d+1, T>(x)
    {
    }


    // Conversions

    template <typename C>
    void from_to_(const algebra::h_mat<3,C>& from, algebra::quat& to)
    {
      C tr = from(0, 0) + from(1, 1) + from(2, 2) + 1;

      if (tr > 0.005f) // Actually, greater than 0
      {
	C s = 0.5 / sqrt(tr),
	  w = 0.25 / s,
	  x = (from(2, 1) - from(1, 2)) * s,
	  y = (from(0, 2) - from(2, 0)) * s,
	  z = (from(1, 0) - from(0, 1)) * s;

	to = algebra::quat(w, x, y, z);
	return;
      }

      // If the trace of the matrix is less than or equal to zero
      // then identify which major diagonal element has the greatest
      // value.

      C max = 0;
      unsigned c = 0;
      for (unsigned d = 0; d <= 3; ++d)
	if (from(d, d) > max)
	{
	  max = from(d, d);
	  c = d;
	}

      // Depending on this value, calculate the following:
      C s, w, x, y, z;
      switch(c)
      {
	case 0:
	  s  = sqrt(1.0 + from(0, 0) - from(1, 1) - from(2, 2)) * 2;
	  x = 0.5 / s;
	  y = (from(0, 1) + from(1, 0)) / s;
	  z = (from(0, 2) + from(2, 0)) / s;
	  w = (from(1, 2) + from(2, 1)) / s;
	  break;

	case 1:
	  s  = sqrt(1.0 + from(1, 1) - from(0, 0) - from(2, 2)) * 2;
	  x = (from(0, 1) + from(1, 0)) / s;
	  y = 0.5 / s;
	  z = (from(1, 2) + from(2, 1)) / s;
	  w = (from(0, 2) + from(2, 0)) / s;
	  break;

	case 2:
	  s  = sqrt(1.0 + from(2, 2) - from(0, 0) - from(1, 1)) * 2;
	  x = (from(0, 2) + from(2, 0)) / s;
	  y = (from(1, 2) + from(2, 1)) / s;
	  z = 0.5 / s;
	  w = (from(0, 1) + from(1, 0) ) / s;
	  break;

	// Error case
	default:
	  x = 0;
	  y = 0;
	  z = 0;
	  w = 0;
      }

      to = algebra::quat(w, x, y, z);
      return;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::algebra

} // end of namespace mln



#endif // ! MLN_ALGEBRA_H_MAT_HH
