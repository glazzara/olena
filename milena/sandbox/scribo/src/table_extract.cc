// Copyright (C) 2009 EPITA Research and Development Laboratory
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

#include <iostream>

#include <mln/essential/2d.hh>
#include <mln/io/dump/save.hh>

#include <scribo/table/extract.hh>


int usage(const char *name)
{
  std::cout << "Usage: " << name << " <input.pbm> " << std::endl;
  return 1;
}


int main(int argc, char* argv[])
{
  using namespace scribo;
  using namespace mln;

  if (argc < 1)
    return usage(argv[0]);

  scribo::make::internal::debug_filename_prefix = argv[0];

  image2d<bool> input;
  io::pbm::load(input, argv[1]);
  logical::not_inplace(input);

  trace::quiet = false;

  value::label_16 ncells;
  image2d<value::label_16> tables = scribo::table::extract(input, ncells);

  std::cout << "ncells (including background) = " << ncells << std::endl;
  io::ppm::save(mln::debug::colorize(value::rgb8(), tables, ncells),
		scribo::make::debug_filename("table_cells.ppm"));

  io::dump::save(tables, scribo::make::debug_filename("table_cells.dump"));
}
