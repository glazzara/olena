// Copyright (C) 2007 EPITA Research and Development Laboratory
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
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
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

#ifndef MLN_MATH_COV_HH
# define MLN_MATH_COV_HH

/*! \file mln/math/cov.hh
 *
 *  \brief Define the covariance (cov) routine.
 */

# include "exp_val.hh"

namespace mln
{

  namespace math
  {

    template <typename P>
    algebra::mat<P::dim,P::dim,float>
    cov(const p_array<P>& a1,
        const p_array<P>& a2);

# ifndef MLN_INCLUDE_ONLY
    
    template <typename P>
    algebra::mat<P::dim,P::dim,float>
    cov(const p_array<P>& a1,
        const p_array<P>& a2)
    {
      mln_precondition(a1.nsites() == a2.nsites());

      //centers of mass
      algebra::vec<P::dim,float> mu_a1 = math::exp_value(a1);
      algebra::vec<P::dim,float> mu_a2 = math::exp_value(a2);

      //covariance matrix
      algebra::mat<P::dim,P::dim,float> Mk(literal::zero);
      for (unsigned i = 0; i < a1.nsites(); ++i)
        {
          // FIXME: ugly cast.
          algebra::vec<P::dim,float> a1i  = a1[i];
          algebra::vec<P::dim,float> a2i  = a2[i];
          Mk += make::mat(a1i - mu_a1) * trans(make::mat(a2i - mu_a2));
        }
      
      return Mk / a1.nsites();
    }
    
  # endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::math

} // end of namespace mln


#endif // ! MLN_MATH_COV_HH
