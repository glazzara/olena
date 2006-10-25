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

/// Test the count_rw morpher.

#include <cassert>
#include <oln/basics2d.hh>
#include <oln/morpher/count_rw.hh>


int main()
{
  using namespace oln;

  typedef image2d<int> image_t;
  image_t ima(3,3);
  morpher::count_rw_<image_t> ima2(ima);
  point2d p(0,0);

  ima2(p);
  ima2(p) = 51;
  int i = ima2(p);
  i = 0; // suppress "unused i" warning

  assert(value::counter_rw::n_readwrite_calls(ima) == 3);
  assert(value::counter_rw::n_readonly_calls(ima)  == 0);
  assert(value::counter_rw::n_reads(ima)           == 1);
  assert(value::counter_rw::n_writes(ima)          == 1);
}


/*

// another piece of nice code:


#include <cassert>

#include <oln/basics2d.hh>
#include <oln/core/gen/pw_value.hh>

#include <oln/morpher/add_isubset.hh>
#include <oln/morpher/add_neighborhood.hh>
#include <oln/morpher/count_rw.hh>

#include <oln/debug/print.hh>
#include <oln/level/fill.hh>


template <typename I>
void test(const oln::abstract::image<I>& input)
{
  oln_value(I) dev_null;
  oln_piter(I) p(input.topo());
  oln_niter(I) n(p, input.topo());
  for_all(p)
  {
    std::cout << p << ": ";
    for_all(n)
    {
      std::cout << n << ' ';
      dev_null = input(n); // read
    }
    std::cout << std::endl;
  }
}

int main()
{
  using namespace oln;
  typedef image2d<int> image_t;

  image_t ima(5, 5);
  int values[] =
    { 3, 7, 1, 1, 7,
      9, 3, 2, 1, 1,
      1, 6, 5, 4, 3,
      7, 9, 8, 1, 3,
      3, 3, 9, 7, 1 };
  ima << values;

# define pred (pw_value(ima) % 2 == 0 or pw_value(ima) == 5)

  debug::print(ima | pred);
  test(count_rw((ima + c4) | pred));
    
  value::counter_rw::print();

// gives:
// - - - - - 
// - - 2 - - 
// - 6 5 4 - 
// - - 8 - - 
// - - - - - 
// (1, 2): (2, 2) 
// (2, 1): (2, 2) 
// (2, 2): (1, 2) (2, 1) (2, 3) (3, 2) 
// (2, 3): (2, 2) 
// (3, 2): (2, 2) 
// n_readwrite_calls = 0
// n_readonly_calls  = 8
// n_reads           = 8
// n_writes          = 0

*/
