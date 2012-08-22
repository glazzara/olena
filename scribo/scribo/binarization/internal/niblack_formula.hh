// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_BINARIZATION_INTERNAL_NIBLACK_FORMULA_HH
# define SCRIBO_BINARIZATION_INTERNAL_NIBLACK_FORMULA_HH


/// \file
///
/// \brief Routines computing a threshold using Niblack's binarization
/// formula.

// \fixme Having an unused parameter to fulfill the required interface
// may not be the best solution...

// Setup default Niblack's formula parameters values.
# define SCRIBO_DEFAULT_NIBLACK_K -0.2



namespace scribo
{

  namespace binarization
  {

    namespace internal
    {

      struct niblack_formula
      {

	/*! \brief compute a threshold using Niblack's formula.

	  \param[in] m_x_y Mean value.
	  \param[in] s_x_y Standard deviation.
	  \param[in] k Control the threshold value in the local
	  window. The higher, the lower the threshold
	  form the local mean m(x, y).

	  \return A threshold.
	*/
	double operator()(const double m_x_y, const double s_x_y,
			  const double K) const;

	/*!
	  \overload K = -0.2.
	 */
	double operator()(const double m_x_y, const double s_x_y) const;

      };


# ifndef MLN_INCLUDE_ONLY

      inline
      double
      niblack_formula::operator()(const double m_x_y, const double s_x_y,
				  const double K) const
      {
	return m_x_y + K * s_x_y;
      }

      inline
      double
      niblack_formula::operator()(const double m_x_y, const double s_x_y) const
      {
	return  (*this)(m_x_y, s_x_y,
			SCRIBO_DEFAULT_NIBLACK_K);
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::binarization::internal

  } // end of namespace scribo::binarization

} // end of namespace scribo

#endif // ! SCRIBO_BINARIZATION_INTERNAL_NIBLACK_FORMULA_HH
