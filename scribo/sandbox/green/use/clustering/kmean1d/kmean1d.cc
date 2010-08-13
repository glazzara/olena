// Copyright (C) 2007, 2008, 2009, 2010 EPITA LRDE
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

/// \file
///
/// \brief Minimal code to use the second implementation of the kmean algorithm.
///

#include <iostream>
#include <mln/clustering/kmean1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/img_path.hh>
#include <mln/io/pgm/load.hh>
#include <mln/value/int_u8.hh>

int main()
{
  typedef mln::value::int_u8     t_int_u8;
  typedef mln::image2d<t_int_u8> t_image2d_int_u8;
  t_image2d_int_u8               img_int_u8;

  mln::io::pgm::load(img_int_u8, OLENA_IMG_PATH"/house.pgm");
  mln::clustering::kmean1d<double, 8> kmean(img_int_u8, 3);

  kmean.launch_n_times();

  return 0;
}