// Copyright (C) 2010, 2011 EPITA Research and Development Laboratory (LRDE)
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

#include <iostream>

#include "apps/bench/dilation-lena.hh"
#include "apps/data.hh"


// Shortcut macros for run.

#define DILATION_WITH_BUILTIN_WINDOW(Namespace, Suffix, Headline)	\
  do									\
    {									\
      d = lena;								\
      t.start();							\
      for (unsigned i = 0; i < niters; ++i)				\
	d = Namespace::dilation(d);					\
      t.stop();								\
      std::cout << Headline << t.read() << " s" << std::endl;		\
      io::pgm::save(d, prefix + '-' + length + '-' + Suffix + ".pgm");	\
    }									\
  while (0)

#define DILATION(Namespace, Win, Suffix, Headline)			\
  do									\
    {									\
      d = lena;								\
      t.start();							\
      for (unsigned i = 0; i < niters; ++i)				\
	d = Namespace::dilation(d, Win);				\
      t.stop();								\
      std::cout << Headline << t.read() << " s" << std::endl;		\
      io::pgm::save(d, prefix + '-' + length + '-' + Suffix + ".pgm");	\
    }									\
  while (0)


void
run(const std::string& filename, const std::string& length, unsigned niters)
{
  using namespace mln;
  using value::int_u8;

  border::thickness = 1;
  image2d<int_u8> lena;
  io::pgm::load(lena, filename);

  image2d<int_u8> d;
  util::timer t;

  std::string prefix = "dilation-lena-out";
  std::cout << "== " << filename << std::endl;

  DILATION_WITH_BUILTIN_WINDOW(nongen,      "nongen",      "nongen\t\t");
  DILATION_WITH_BUILTIN_WINDOW(nongen_2ptr, "nongen_2ptr", "nongen_2ptr\t");
  DILATION_WITH_BUILTIN_WINDOW(nongen_1ptr, "nongen_1ptr", "nongen_1ptr\t");

  DILATION(gen,           win_c4p(), "gen",           "gen\t\t");
  // FIXME: Introduce a new test case, gen_static, using a static window
  // and static_qiters.
  DILATION(fast,          win_c4p(), "fast",          "fast\t\t");
  DILATION(fast_noaccu,   win_c4p(), "fast_noaccu",   "fast_noaccu\t");
  DILATION(faster,        win_c4p(), "faster",        "faster\t\t");
  DILATION(faster_noaccu, win_c4p(), "faster_noaccu", "faster_noaccu\t");

  // Static windows and qixters.
  const unsigned n = 5;
  mln::dpoint2d dps[n] = { mln::dpoint2d( 0, -1),
			   mln::dpoint2d(-1,  0),
			   mln::dpoint2d( 0,  0),
			   mln::dpoint2d(+1,  0),
			   mln::dpoint2d( 0, +1) };
  mln::util::static_array<mln::dpoint2d, n> sa(dps, dps + n);
  mln::static_window<mln::dpoint2d, n> static_win_c4p (sa);

  DILATION(fast_static,   static_win_c4p, "fast_static",   "fast_static\t");
  DILATION(faster_static, static_win_c4p, "faster_static", "faster_static\t");

  std::cout << std::endl;
}

int
main ()
{
  unsigned niters = 10;
  run(MLN_IMG_DIR "/lena.pgm",            "512",  niters);
  run(MLN_APPS_DIR "/bench/lena1024.pgm", "1024", niters);
  run(MLN_APPS_DIR "/bench/lena2048.pgm", "2048", niters);
}
