// Copyright (C) 2012, 2013 EPITA Research and Development Laboratory
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

#ifndef SCRIBO_BINARIZATION_INTERNAL_SINGH_FORMULA_HH
# define SCRIBO_BINARIZATION_INTERNAL_SINGH_FORMULA_HH


/// \file
///
/// \brief Routines computing a threshold using Singh's binarization
/// formula.


// Setup default Singh's formula parameters values.
// These macros may be used in other variant of Singh's algorithms.
# define SCRIBO_DEFAULT_SINGH_K 0.06


#include <mln/core/alias/point2d.hh>

// extern mln::image2d<bool> skewness_pbm;

namespace scribo
{

  namespace binarization
  {

    namespace internal
    {

      using namespace mln;

      template <typename V>
      struct singh_formula
      {

	/*! \brief Compute a threshold using Singh's formula.

	  \param[in] v The current image value.
	  \param[in] m_x_y Mean value.
	  \param[in] K Control the threshold value in the local
	  window. The higher, the lower the threshold
	  form the local mean m(x, y).

	  \return A threshold.
	*/
	double operator()(const V& v, const double m_x_y, const double K) const;

	/*! \overload
	  K = 0.34.
	 */
	double operator()(const V& v, const double m_x_y) const;

      };


# ifndef MLN_INCLUDE_ONLY

      // bool b;
      // double skewness_;

      template <typename V>
      inline
      double
      singh_formula<V>::operator()(const V& v, const double m_x_y, const double K) const
      {
	double mean_dev = v - m_x_y;
	return  m_x_y * (1.0 + K * (mean_dev / (1 - mean_dev) - 1));
      }

      template <typename V>
      inline
      double
      singh_formula<V>::operator()(const V& v, const double m_x_y) const
      {
	return  (*this)(v, m_x_y, SCRIBO_DEFAULT_SINGH_K);
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::binarization::internal

  } // end of namespace scribo::binarization

} // end of namespace scribo

#endif // ! SCRIBO_BINARIZATION_INTERNAL_SINGH_FORMULA_HH
