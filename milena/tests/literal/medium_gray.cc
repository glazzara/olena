// Copyright (C) 2007 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/literal/grays.hh>
#include <mln/value/graylevel.hh>
#include <mln/value/gl8.hh>
#include <mln/value/gl16.hh>


int main()
{
  using namespace mln;

  using literal::medium_gray;
  using value::gl8;
  using value::gl16;

  gl8 a;
  gl16 b;

  a = medium_gray;

  std::cout << int(a.value()) << std::endl;

  b = a;
  std::cout << int(b.value()) << std::endl;

  b = medium_gray;
  std::cout << int(b.value()) << std::endl;
}
