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

#ifndef MLN_FUN_X2X_ROTATION_HH
# define MLN_FUN_X2X_ROTATION_HH

/*! \file mln/fun/x2x/rotation.hh
 *
 * \brief Define a rotation function.
 */

# include <mln/core/concept/function.hh>
# include <mln/fun/internal/x2x_linear_impl.hh>
# include <mln/algebra/vec.hh>
# include <mln/algebra/mat.hh>
# include <cmath>

namespace mln
{

  namespace fun
  {

    namespace x2x
    {

      /*! \brief Represent a rotation function.
       *
       */
      template <unsigned n, typename C>
      struct rotation
	: internal::x2x_linear_impl_< algebra::vec<n,C>, rotation<n,C> >
	, public Bijection_x2x< rotation<n,C> >
      {
	typedef fun::internal::x2x_linear_impl_< algebra::vec<n,C>, rotation<n,C> > super_;

	/// Type of the inverse function.
	typedef rotation<n,C> invert;
	/// Return the invere function.
	invert inv() const;

	/// Constructor without argument.
        rotation();
	/// Constructor with grade alpha and a facultative direction (rotation axis).
        rotation(float alpha, unsigned dir = 2);

        using super_::operator();
	/// Perform the rotation of the given vector.
        algebra::vec<n,C> operator()(const algebra::vec<n,C>& v) const;

	/// Set a new grade alpha.
        void set_alpha(float alpha);
	/// Set a new rotation axis.
        void set_dir(unsigned dir);

      protected:
        void update();

        float alpha_;
        unsigned dir_;
      };


# ifndef MLN_INCLUDE_ONLY

      template <unsigned n, typename C>
      inline
      rotation<n,C>::rotation()
      {
      }

      template <unsigned n, typename C>
      inline
      rotation<n,C>::rotation(float alpha, unsigned dir)
	:alpha_(alpha),
	 dir_(dir)
      {
	mln_precondition(dir == 2 || n == 3);
	this->m_ = algebra::h_mat<n,C>::Id;
	update();
      }

      template <unsigned n, typename C>
      inline
      algebra::vec<n,C>
      rotation<n,C>::operator()(const algebra::vec<n,C>& v) const
      {
	algebra::mat<n+1,1,C> hmg;
	algebra::mat<n+1,1,C> tmp;
	algebra::vec<n,C> res;

	for (unsigned i = 0; i < n; ++i)
	  hmg(i,0) = v[i];
	hmg(n,0) = 1;
	tmp = this->m_ * hmg;
	mln_assertion(tmp(n,0) == 1);
	for (unsigned i = 0; i < n; ++i)
	  res[i] = tmp(i,0);
	return res;
      }

      template <unsigned n, typename C>
      inline
      rotation<n,C>
      rotation<n,C>::inv() const
      {
	typename rotation::invert res(-alpha_, dir_);
	return res;
      }

      template <unsigned n, typename C>
      inline
      void
      rotation<n,C>::set_alpha(float alpha)
      {
	alpha_ = alpha;
	update();
      }

      template <unsigned n, typename C>
      inline
      void
      rotation<n,C>::set_dir(unsigned dir)
      {
	mln_precondition(dir == 2 || n == 3);
	dir_ = dir;
	update();
      }

      template <unsigned n, typename C>
      inline
      void
      rotation<n,C>::update()
      {
	const float cos_a = cos(alpha_);
	const float sin_a = sin(alpha_);
	const algebra::vec<4,float> vec = make::vec(cos_a, -sin_a, sin_a, cos_a);

	unsigned k = 0;
	for (unsigned i = 0; i < n; ++i)
	  for (unsigned j = 0; j < n; ++j)
	  {
	    if (j != this->dir_ && i != this->dir_)
	      this->m_(i, j) = vec[k++];
	    else
	      this->m_(i, j) = (i == j);
	  }
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::x2x

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_X2X_ROTATION_HH
