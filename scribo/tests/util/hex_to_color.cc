// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

// \file

#include <scribo/util/hex_to_color.hh>

int main()
{
  using namespace mln;

  value::rgb8 v1 = scribo::util::hex_to_color("#ADFF2F");
  value::rgb8 v2 = scribo::util::hex_to_color("ADFF2F");
  value::rgb8 v3 = scribo::util::hex_to_color("aDfF2F");

  mln_assertion(v1 == v2);
  mln_assertion(v2 == v3);
  mln_assertion(v1 == value::rgb8(173, 255, 47));
}
