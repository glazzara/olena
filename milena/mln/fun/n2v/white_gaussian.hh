// Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_FUN_N2V_WHITE_GAUSSIAN_HH
# define MLN_FUN_N2V_WHITE_GAUSSIAN_HH

/// \file
///
/// White Gaussian noise.

# include <cstdlib>
# include <ctime>

# include <mln/core/concept/function.hh>



namespace mln
{

  namespace fun
  {

    namespace n2v
    {

      /*! \brief Generate a White Gaussian Noise.


	Reference:
	http://www.dspguru.com/dsp/howtos/how-to-generate-white-gaussian-noise
       */
      template <typename V>
      struct white_gaussian : Function_n2v< white_gaussian<V> >
      {
	typedef V result;

	white_gaussian(double sigma_);
	V operator()() const;

	double sigma;
      };


# ifndef MLN_INCLUDE_ONLY


      template <typename V>
      white_gaussian<V>::white_gaussian(double sigma_)
	: sigma(sigma_)
      {
	srand(time(0));
      }


      template <typename V>
      V white_gaussian<V>::operator()() const
      {
	double X = 0;
	for (int i = 1; i < 50; ++i)
	{
	  double U = rand() / (float)RAND_MAX;
	  X = X + U;
	}

	X = X - 25.f;                /* set mean to 0 */
	X = X * sqrt(12 / 50.f);     /* adjust variance to 1 */

	return sigma * X;
      };


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::n2v

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_N2V_WHITE_GAUSSIAN_HH
