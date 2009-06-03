// Copyright (C) 2008 EPITA Research and Development Laboratory
//
// This file is part of the Milena Library.  This library is free
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


#include "demat.hh"

int main(int argc, char*argv[])
{
  using namespace mln;
  using value::int_u8;

  if (argc < 2)
  {
    std::cout << argv[0] << " <in.pbm> <out.pgm> <l> <bbox_larger> <bbox_distance> <min_comp_nsites>" << std::endl
	      << std::endl << std::endl
	      << std::endl
	      << "=========="
	      << std::endl << std::endl
	      << "<in.pbm>	    B/W inverted input image."
	      << std::endl << std::endl
/*	      << "<out.ppm>	    RGB8 output image."
	      << std::endl << std::endl
	      << "<bbox_distance>     Maximum distance between character bounding boxes. Used for bbox grouping."
	      << std::endl << std::endl
	      << "<min_comp_nsites>   Minimum site count of a character/text component."
	      << std::endl
	      << "		    If a component have a site count lesser than this value, it is erased."
	      << std::endl << std::endl
	      << std::endl*/
	      << "=========="
	      << std::endl << std::endl
	      << "HINT: compile with -DNOUT to avoid debug images."
	      << std::endl << std::endl;
    return 1;
  }

  scribo::demat(argv, false);

  return 0;
}
