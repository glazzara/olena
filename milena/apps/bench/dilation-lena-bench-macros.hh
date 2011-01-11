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

#ifndef APPS_BENCH_DILATION_LENA_MACROS_HH
# define APPS_BENCH_DILATION_LENA_MACROS_HH

#include <cstdlib>

#include <iostream>
#include <iomanip>
#include <string>

#include "apps/bench/dilation-lena.hh"


#define DILATION_WITH_BUILTIN_WINDOW(Namespace)		\
  do							\
    {							\
      using namespace mln;				\
      using value::int_u8;				\
							\
      if (argc != 2)					\
	usage(argv[0]);					\
							\
      std::string filename = argv[1];			\
							\
      border::thickness = 1;				\
      image2d<int_u8> lena;				\
      io::pgm::load(lena, filename);			\
      util::timer t;					\
							\
      image2d<int_u8> d = lena;				\
      t.start();					\
      for (unsigned i = 0; i < 10; ++i)			\
	d = Namespace::dilation(d);			\
      t.stop();						\
							\
      /* Fix the output format for floats.  */		\
      std::cout << std::fixed << std::setprecision(4);	\
      /* Display the elapsed time.  */			\
      std::cout << t.read() << std::endl;		\
    }							\
  while (0)

#define DILATION(Namespace, Win)			\
  do							\
    {							\
      using namespace mln;				\
      using value::int_u8;				\
							\
      if (argc != 2)					\
	usage(argv[0]);					\
							\
      std::string filename = argv[1];			\
							\
      border::thickness = 1;				\
      image2d<int_u8> lena;				\
      io::pgm::load(lena, filename);			\
      util::timer t;					\
							\
      image2d<int_u8> d = lena;				\
      t.start();					\
      for (unsigned i = 0; i < 10; ++i)			\
        d = Namespace::dilation(d, Win);		\
      t.stop();						\
							\
      /* Fix the output format for floats.  */		\
      std::cout << std::fixed << std::setprecision(4);	\
      /* Display the elapsed time.  */			\
      std::cout << t.read() << std::endl;		\
    }							\
  while (0)

#endif // ! APPS_BENCH_DILATION_LENA_MACROS_HH
