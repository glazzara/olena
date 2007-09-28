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
	  rotation(float alpha, unsigned dir = 2);

	  result operator()(const metal::vec<n,C>& v) const;
	  invert inv() const;

	  void set_alpha(float alpha);
	  void set_dir(unsigned dir);

	  void update();

	protected:

	  float alpha_;
	  unsigned dir_;
	  metal::mat<n + 1,n + 1,C> m_;
      };


# ifndef MLN_INCLUDE_ONLY

      template <unsigned n, typename C>
      rotation<n,C>::rotation()
      {
	m_ = metal::mat<n + 1,n + 1,C>::Id;
      }

      template <unsigned n, typename C>
      rotation<n,C>::rotation(float alpha, unsigned dir)
	:alpha_(alpha),
	 dir_(dir)
      {
	mln_precondition(dir == 2 || n == 3);
	update();
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
	typename rotation::invert res(-alpha_, dir_);
	return res;
      }

      template <unsigned n, typename C>
      void
      rotation<n,C>::set_alpha(float alpha)
      {
	alpha_ = alpha;
	update();
      }

      template <unsigned n, typename C>
      void
      rotation<n,C>::set_dir(unsigned dir)
      {
	mln_precondition(dir == 2 || n == 3);
	dir_ = dir;
	update();
      }

      template <unsigned n, typename C>
      void
      rotation<n,C>::update()
      {
	const float cos_a = cos(alpha_);
	const float sin_a = sin(alpha_);
	const metal::vec<4,float> vec = make::vec(cos_a, -sin_a, sin_a, cos_a);

	m_ = metal::mat<n + 1,n + 1,C>::Id;
	unsigned k = 0;
	for (unsigned i = 0; i < n; ++i)
	{
	  if (i == dir_)
	    continue;
	  for (unsigned j = 0; j < n; ++j)
	  {
	    if (j == dir_)
	      continue;
	    m_(i, j) = vec[k++];
	  }
	}
      }

//       template <typename C> FIXME : template parameter should be swapped
//       void
//       rotation<2,C>::update()
//       {
// 	const float cos_a = cos(alpha_);
// 	const float sin_a = sin(alpha_);

// 	m_ = metal::mat<3,3,C>::Id;
// 	m_(0, 0) = cos_a;
// 	m_(0, 1) = -sin_a;
// 	m_(1, 0) = sin_a;
// 	m_(1, 1) = cos_a;
//       }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::x2x

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_X2X_ROTATION_HH
