// Copyright (C) 2006 EPITA Research and Development Laboratory
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

/// Test the "two-way read-write" morpher.

# include <cassert>

# include <oln/basics2d.hh>
# include <oln/morpher/two_way_rw.hh>




struct norm_t : public oln::abstract::fun_rw< norm_t >
{
  typedef float result_type;

  template <typename I>
  float read(const I& input, const oln_point(I)& p) const
  {
    return xtd::L2_norm(input(p));
  }

  template <typename I>
  void write(I& input, const oln_point(I)& p, float val) const
  {
    float old_norm = xtd::L2_norm(input(p));
    assert(old_norm != 0);
    input(p) *= val / old_norm;
  }
};



int main()
{
  norm_t norm;

  using namespace oln;
  using xtd::vec;
  using xtd::mk_vec;

  typedef image2d< vec<2,float> > image_t;

  image_t ima(1, 1);
  point2d p(0, 0);

  ima(p) = mk_vec(5,1);
  norm(ima)(p) = 1;
  assert(xtd::L2_norm(ima(p)) == 1);
}
