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

#ifndef SCRIBO_BINARIZATION_INTERNAL_SAUVOLA_FORMULA_HH
# define SCRIBO_BINARIZATION_INTERNAL_SAUVOLA_FORMULA_HH


/// \file
///
/// \brief Routines computing a threshold using Sauvola's binarization
/// formula.


// Setup default Sauvola's formula parameters values.
// These macros may be used in other variant of Sauvola's algorithms.
//
// Values are set according to the following reference: "Automatic
// Evaluation of Document Binarization Results", Badekas and al, 2005
//
// Badekas et al. said 0.34 was best for Sauvola.
# define SCRIBO_DEFAULT_SAUVOLA_K 0.34
//
// 128 is best for grayscale documents.
# define SCRIBO_DEFAULT_SAUVOLA_R 128


#include <mln/core/alias/point2d.hh>

extern mln::image2d<bool> skewness_pbm;

namespace scribo
{

  namespace binarization
  {

    namespace internal
    {

      using namespace mln;

      struct sauvola_formula
      {

	/*! \brief Compute a threshold using Sauvola's formula.

	  \param[in] m_x_y Mean value.
	  \param[in] s_x_y Standard deviation.
	  \param[in] k Control the threshold value in the local
	  window. The higher, the lower the threshold
	  form the local mean m(x, y).
	  \param[in] R Maximum value of the standard deviation (128
	  for grayscale documents).

	  \return A threshold.
	*/
	double operator()(const point2d& p, const double m_x_y, const double s_x_y,
			  const double K, const double R) const;

	/*!
	  \overload K = 0.34 and R = 128.
	 */
	double operator()(const point2d& p, const double m_x_y, const double s_x_y) const;

      };


# ifndef MLN_INCLUDE_ONLY

      // bool b;
      // double skewness_;

      inline
      double
      sauvola_formula::operator()(const point2d& p, const double m_x_y, const double s_x_y,
				  const double K, const double R) const
      {
	// if (b)
	//   std::cout << skewness_ << " - " << (K * -1 * skewness_) << std::endl;
	// volatile double new_t = ((skewness_ < 0) ? -skewness_ : 1 * m_x_y * (1.0 + K * ((s_x_y / R) - 1.0)));
	// volatile double old_t = (m_x_y * (1.0 + K * ((s_x_y / R) - 1.0)));
	// if (skewness_ > 0)
	//   if (new_t != old_t)
	//     std::cout << skewness_ << " - " << new_t << " vs " << old_t << std::endl;
	if (skewness_pbm(p))
	  return  m_x_y * (1.0 + K * ((s_x_y / R) - 1.0));
	else
	  return  (255 - m_x_y) * (1.0 + K * ((s_x_y / R) - 1.0));
      }

      inline
      double
      sauvola_formula::operator()(const point2d& p, const double m_x_y, const double s_x_y) const
      {
	return  (*this)(p, m_x_y, s_x_y,
			SCRIBO_DEFAULT_SAUVOLA_K, SCRIBO_DEFAULT_SAUVOLA_R);
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::binarization::internal

  } // end of namespace scribo::binarization

} // end of namespace scribo

#endif // ! SCRIBO_BINARIZATION_INTERNAL_SAUVOLA_FORMULA_HH
