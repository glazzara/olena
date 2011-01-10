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
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#include <cstdlib>

#include <iostream>
#include <iomanip>
#include <string>

#include "apps/bench/dilation-lena.hh"


namespace tag
{
  enum dilation
    {
      nongen = 1,
      nongen_2ptr = 2,
      gen = 3,
      fast = 4,
      fast_static = 5
    };
}

void
usage(const std::string& program)
{
  std::cerr << program << " implementation input.pgm\n\n" <<
    "  were implementation is\n"
    "    1 : nongen\n"
    "    2 : nongen_2ptr \n"
    "    3 : gen\n"
    "    4 : fast\n"
    "    5 : fast_static\n";
  std::exit(1);
}


#define DILATION_WITH_BUILTIN_WINDOW(Namespace)	\
  do						\
    {						\
      image2d<int_u8> d = lena;			\
      t.start();				\
      for (unsigned i = 0; i < 10; ++i)		\
	d = Namespace::dilation(d);		\
      t.stop();					\
    }						\
  while (0)

#define DILATION(Namespace, Win)		\
  do						\
    {						\
      image2d<int_u8> d = lena;			\
      t.start();				\
      for (unsigned i = 0; i < 10; ++i)		\
        d = Namespace::dilation(d, Win);	\
      t.stop();					\
    }						\
  while (0)


int
main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;
  
  if (argc != 3)
    usage(argv[0]);

  int method = atoi(argv[1]);
  std::string filename = argv[2];

  border::thickness = 1;
  image2d<int_u8> lena;
  io::pgm::load(lena, filename);
  util::timer t;

  // Static windows and qixters.
  const unsigned n = 5;
  mln::dpoint2d dps[n] = { mln::dpoint2d( 0, -1),
			   mln::dpoint2d(-1,  0),
			   mln::dpoint2d( 0,  0),
			   mln::dpoint2d(+1,  0),
			   mln::dpoint2d( 0, +1) };
  mln::util::static_array<mln::dpoint2d, n> sa(dps, dps + n);
  mln::static_window<mln::dpoint2d, n> static_win_c4p(sa);

  switch (method)
    {
    case ::tag::nongen:      DILATION_WITH_BUILTIN_WINDOW(nongen);      break;
    case ::tag::nongen_2ptr: DILATION_WITH_BUILTIN_WINDOW(nongen_2ptr); break;
    case ::tag::gen:         DILATION(gen,         win_c4p());          break;
    case ::tag::fast:        DILATION(fast,        win_c4p());          break;
    case ::tag::fast_static: DILATION(fast_static, static_win_c4p);     break;

    default:
      usage(argv[0]);
    }

  // Fix the output format for floats.
  std::cout << std::fixed << std::setprecision(4);
  // Display the elapsed time.
  std::cout << t.read() << std::endl;
}
