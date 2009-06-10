// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#include <mln/core/image/image2d.hh>
#include <mln/core/site_set/p_array.hh>
#include <mln/core/site_set/p_vaccess.hh>
#include <mln/debug/println.hh>
#include <mln/convert/to.hh>


int main()
{
  using namespace mln;

  unsigned char vals[] = { 1, 2, 1,
			   2, 2, 3,
			   3, 4, 1 };
  image2d<unsigned char> ima = make::image2d(vals);

  debug::println(ima);

  typedef p_array<point2d> Arr;
  typedef p_vaccess<unsigned char, Arr> S;

  S s = convert::to<S>(ima);
  
  mln_viter_(S::vset) v(s.values());
  for_all(v)
  {
    if (s(v).nsites() == 0)
      continue;
    std::cout << int(v) << ": ";

    mln_piter_(Arr) p(s(v));
    for_all(p)
      std::cout << p << ' ';
    std::cout << std::endl;
  }

}
