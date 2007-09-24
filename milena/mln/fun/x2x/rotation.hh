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
 * \brief FIXME.
 */

# include <mln/core/concept/function.hh>
# include <mln/metal/vec.hh>
# include <mln/metal/mat.hh>
# include <cmath>


namespace mln
{

  namespace fun
  {

    namespace x2x
    {

      // FIXME: Doc!

      template <unsigned n, typename C>
      struct rotation : public Function_x2x< rotation<n,C> >
      {

	  enum {dim = n};

	  typedef metal::vec<n,C> result;
	  typedef rotation<n,C> invert;

	  rotation();
	  rotation(const float alpha, const metal::vec<n,C>& t);

	  result operator()(const metal::vec<n,C>& v) const;
	  invert inv() const;

	  void set_t(const metal::vec<n,C>& t);
	  void set_alpha(const float alpha);

	protected:

	  float alpha_;
	  metal::vec<n,C> t_;
	  metal::mat<n + 1,n + 1,C> m_;
      };


# ifndef MLN_INCLUDE_ONLY

      template <unsigned n, typename C>
      rotation<n,C>::rotation()
      {
	t_ = make::vec<n,C>(0);
	m_ = metal::mat<n+1,n+1,C>::Id;
      }

      template <unsigned n, typename C>
      rotation<n,C>::rotation(const float alpha, const metal::vec<n,C>& t)
	:alpha_(alpha),
	 t_(t)
      {
	const float cos_a = cos(alpha_);
	const float sin_a = sin(alpha_);
	const float c_1 = 1 - cos_a;
	const float coord1 = c_1 * t[0] + sin_a * t[1];
	const float coord2 = t[1] * c_1 - sin_a * t[0];

	m_ = metal::mat<n+1,n+1,C>::Id;
	m_(0,2) = coord1;
	m_(1,2) = coord2;
	m_(0,0) = cos_a;
	m_(0,1) = -sin_a;
	m_(1,0) = sin_a;
	m_(1,1) = cos_a;
      }

      template <unsigned n, typename C>
      metal::vec<n,C>
      rotation<n,C>::operator()(const metal::vec<n,C>& v) const
      {
	metal::mat<n+1,1,C> hmg;
	metal::mat<n+1,1,C> tmp;
	metal::vec<n,C> res;

	for (unsigned i = 0; i < n; ++i)
	  hmg(i,0) = v[i];
	hmg(n,0) = 1;
	tmp = m_ * hmg;
	mln_assertion(tmp(n,0) == 1);
	for (unsigned i = 0; i < n; ++i)
	  res[i] = tmp(i,0);
	return res;
      }

      template <unsigned n, typename C>
      rotation<n,C>
      rotation<n,C>::inv() const
      {
	typename rotation::invert res(-alpha_, t_);

	return res;
      }

      template <unsigned n, typename C>
      void
      rotation<n,C>::set_t(const metal::vec<n,C>& t)
      {
	t_ = t;
	for (unsigned i = 0; i < n; ++i)
	  m_(i,n) = t_[i];
      }

      template <unsigned n, typename C>
      void
      rotation<n,C>::set_alpha(const float alpha)
      {
	const float cos_a = cos(alpha);
	const float sin_a = sin(alpha);

	alpha_ = alpha;

	m_(0,0) = cos_a;
	m_(0,1) = -sin_a;
	m_(1,0) = sin_a;
	m_(1,1) = cos_a;
      }



# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::x2x

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_X2X_ROTATION_HH
