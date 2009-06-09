// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_FUN_STAT_MAHALANOBIS_HH
# define MLN_FUN_STAT_MAHALANOBIS_HH

/// \file
///
/// \brief Define the FIXME

# include <cmath>
# include <mln/core/concept/function.hh>
# include <mln/algebra/vec.hh>
# include <mln/algebra/mat.hh>


namespace mln
{

  namespace fun
  {

    namespace stat
    {

      template <typename V>
      struct mahalanobis
	: public Function_v2v< mahalanobis<V> >,
	  private metal::equal< V, algebra::vec<V::dim,float> >::check_t
      {
	enum { n = V::dim };
	typedef float result;

	mahalanobis(const algebra::mat<n,n,float>& var,
		    const algebra::vec<n,float>&   mean);
	// Tech. note: using n (instead of V::dim) above helps g++-2.95.

	float operator()(const V& v) const;

	typedef algebra::vec<n,float> mean_t;

	mean_t mean() const;

      protected:
	algebra::mat<n,n,float> var_1_;
	algebra::vec<n,float>   mean_;
      };


# ifndef MLN_INCLUDE_ONLY

      template <typename V>
      inline
      mahalanobis<V>::mahalanobis(const algebra::mat<n,n,float>& var,
				  const algebra::vec<n,float>&   mean)
      {
	var_1_ = var._1();
	mean_  = mean;
      }

      template <typename V>
      inline
      float
      mahalanobis<V>::operator()(const V& v) const
      {
	return std::sqrt((v - mean_).t() * var_1_ * (v - mean_));
      }

      template <typename V>
      inline
      typename mahalanobis<V>::mean_t
      mahalanobis<V>::mean() const
      {
	return mean_;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::stat

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_X2V_NORM_MAHALANOBIS_HH
