// Copyright (C) 2012 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_BINARIZATION_INTERNAL_WOLF_FORMULA_HH
# define SCRIBO_BINARIZATION_INTERNAL_WOLF_FORMULA_HH


/// \file
///
/// \brief Routines computing a threshold using Wolf's binarization
/// formula.


// Setup default Wolf's formula parameters values.
// These macros may be used in other variant of Wolf's algorithms.
//
// Values are set according to the following reference: <FIXME>

# define SCRIBO_DEFAULT_WOLF_K 0.34


#include <mln/core/alias/point2d.hh>

namespace scribo
{

  namespace binarization
  {

    namespace internal
    {

      using namespace mln;

      template <typename V>
      struct wolf_formula
      {

	/*! \brief Compute a threshold using Wolf's formula.

	  \todo doc!

	  \return A threshold.
	*/
	double operator()(const double m_x_y, const double s_x_y,
			  const double K, const double global_max_stddev,
			  const V& global_min) const;

      };


# ifndef MLN_INCLUDE_ONLY


      template <typename V>
      inline
      double
      wolf_formula<V>::operator()(const double m_x_y, const double s_x_y,
				  const double K, const double global_max_stddev,
				  const V& global_min) const
      {
	return  m_x_y - K * (1 - s_x_y / global_max_stddev) * (m_x_y - global_min);
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::binarization::internal

  } // end of namespace scribo::binarization

} // end of namespace scribo

#endif // ! SCRIBO_BINARIZATION_INTERNAL_WOLF_FORMULA_HH
