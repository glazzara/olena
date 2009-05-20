// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

/// \file tests/core/site_set/p_transformed.cc
///
/// Tests on mln::p_transformed.

#include <mln/core/alias/box2d.hh>
#include <mln/core/site_set/p_transformed.hh>
#include <mln/geom/bbox.hh>




struct wrap : mln::Function_v2v< wrap >
{
  typedef mln::point2d result;
  result operator()(const result& p) const
  {
    return result(p.row() + 5, p.col() + 1);
  }
};


int main()
{
  using namespace mln;

  box2d b(2, 2);
  p_transformed<box2d,wrap> s(b, wrap());
  box2d b_ = geom::bbox(s);

  mln_assertion(b_.pmin() == point2d(5,1));
}
