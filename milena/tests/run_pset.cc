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

/*! \file tests/run_pset.cc
 *
 * \brief Test on mln::internal::run_pset_ and related tools.
 */

#include <mln/core/image2d.hh>
#include <mln/core/internal/run_pset.hh>


template <typename Pset>
void parc(const Pset& pset)
{
  mln_fwd_piter(Pset) it_(pset);
  for_all(it_)
    std::cout << it_ << std::endl;

  mln_bkd_piter(Pset) rit_(pset);
  for_all(rit_)
    std::cout << rit_ << std::endl;
}


int main()
{
  using namespace mln;

  point2d p, q, r;
  p = make::point2d(2, 4);
  q = make::point2d(18, 42);
  r = make::point2d(50, 76);

  // Psite declaration
  internal::run_psite<point2d>  site(p, 5, 0);
  internal::run_psite<point2d>  site2(r, 40, 0);

  // Pset test
  internal::run_pset_<point2d> ps;

  ps.insert(p, 7);
  mln_assertion(ps.npoints() == 7);

  ps.insert(q, 42);
  mln_assertion(ps.npoints() == 49);

  mln_assertion(ps.has(site));
  mln_assertion(!ps.has(site2));

  ps.insert(r, 14);
  mln_assertion(!ps.has(site2));

  // parc(ps);
}
