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

#include <algorithm>
#include <iterator>
#include <iostream>

#include <mln/io/off/load.hh>

#include "tests/data.hh"

// FIXME: write a test!

int main()
{
  using namespace mln;

  typedef float_2complex_image3df ima_t;
  ima_t ima;
  io::off::load(ima, MLN_MESH_DIR "/cone-max-curv.off");

  std::cout << ima.domain().cplx() << std::endl;

  // Iterate on 2-faces.
  mln::p_n_faces_fwd_piter<2, mln_geom_(ima_t)> p(ima.domain(), 2);
  for_all(p)
  {
    // Print psite and value.
    std::cout << "ima(" << p << ") = " << ima(p) /**/ << std::endl
	      << "  located at ";
    // Print site.
    typedef mln_site_(ima_t) site_t;
    site_t s(p);
    std::copy (s.sites.begin(), s.sites.end(),
	       std::ostream_iterator<site_t::location>(std::cout, "  "));
    std::cout << std::endl;
  }
}
