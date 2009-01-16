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

# include <mln/core/alias/neighb2d.hh>
# include <mln/core/image/image2d.hh>
# include <mln/value/int_u8.hh>
# include <mln/io/pgm/load.hh>
# include <mln/io/pgm/save.hh>
# include <mln/linear/gaussian.hh>
# include <tests/timer.hh>

# include "self_dual_reconstruction.hh"

void usage(char** argv)
{
  std::cerr << "Usage: " << argv[0] << " input.pgm blur" << std::endl;
  exit(1);
}

int main(int argc, char** argv)
{
  using namespace mln;

  if (argc < 2)
    usage(argv);

  typedef image2d<value::int_u8> I;
  typedef mln_value_(I) V;

  timer t;

  t.start();
  I input = io::pgm::load<V>(argv[1]);
  std::cout << "load: " << t << std::endl;

  I output;

  t.start();
  output = linear::gaussian(input, atoi(argv[2]));
  io::pgm::save(output, "gaussian.pgm");
  std::cout << "gaussian: " << t << std::endl;


  trace::quiet = false;

  t.start();
  output = self_dual_reconstruction(output, input, c8());
  std::cout << "reconstruction: " << t << std::endl;

  trace::quiet = true;

  t.start();
  io::pgm::save(output, "reconstruction.pgm");
  std::cout << "save: " << t << std::endl;
}
