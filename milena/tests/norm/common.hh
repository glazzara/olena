// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef TESTS_NORM_COMMON_HH
# define TESTS_NORM_COMMON_HH

#include <cassert>

#include <mln/math/abs.hh>


// FIXME: We should have a almost_equal function somewhere in Milena.
static const float epsilon = 0.0001f;


namespace test
{

  template <typename Norm, typename V>
  void
  check_norm(const Norm& norm, const V& vec1, const V& vec2)
  {
    assert(mln::math::abs(norm(vec1) - norm(vec2)) < epsilon);
  }

  template <typename Distance, typename V, typename S>
  void
  check_distance(const Distance& dist, const V& vec1, const V& vec2,
		 const S& ref_val)
  {
    float delta = mln::math::abs(static_cast<float>(dist(vec1, vec2))
				 - static_cast<float>(ref_val));
    assert(delta < epsilon);
  }

} // end of namespace test.


#endif // ! TESTS_NORM_COMMON_HH
