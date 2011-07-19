// Copyright (C) 2010, 2011 EPITA Research and Development Laboratory (LRDE)
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

/// \file
/// \brief A program computing an unconstrained skeleton of a 2D image.

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/topo/skeleton/breadth_first_thinning.hh>

#include <mln/topo/is_simple_point2d.hh>
#include <mln/topo/detach_point.hh>

#include <mln/io/pbm/all.hh>


int
main(int argc, char* argv[])
{
  using namespace mln;

  if (argc != 3)
    {
      std::cerr << "usage: " << argv[0] << " input.pbm output.pbm"
		<< std::endl;
      std::exit(1);
    }

  std::string input_filename = argv[1];
  std::string output_filename = argv[2];

  typedef image2d<bool> I;
  typedef neighb2d N;

  // Add a border of (at least) 1 pixel, to a guarantee a meaningful
  // result of the computation of connectivity numbers (called within
  // is_simple_2d); indeed, this computation always expects each pixel
  // to have 8 neighboring sites.
  border::thickness = 1;

  I input = io::pbm::load(input_filename);

  // FIXME: Use a dual neighborhood instead?

  // Foreground neighborhood.
  neighb2d nbh_fg = c4();
  // Background neighborhood.
  neighb2d nbh_bg = c8();

  // Simplicity criterion functor.
  topo::is_simple_point2d<I, N> is_simple(nbh_fg, nbh_bg);
  // Simple point detach procedure.
  topo::detach_point<I> detach;

  I output = topo::skeleton::breadth_first_thinning(input, nbh_fg,
						    is_simple,
						    detach);
  io::pbm::save(output, output_filename);
}
