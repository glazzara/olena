// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef APPS_GRAPH_MORPHO_DEBUG_HH
# define APPS_GRAPH_MORPHO_DEBUG_HH

/// \file
/// \brief Pretty-printing between graph/complex image types.

# include <mln/core/alias/complex_image.hh>
# include <mln/core/image/image2d.hh>

# include <mln/debug/println.hh>

# include "apps/graph-morpho/convert.hh"


namespace debug
{

  /// Pretty print an mln::image2d<bool> viewed as a graph.
  void
  println_graph(const std::string& message, const mln::image2d<bool>& input)
  {
    // INPUT must have odd numbers of rows and columns.
    mln_precondition(input.nrows() % 2 == 1);
    mln_precondition(input.ncols() % 2 == 1);

    mln::image2d<char> output;
    mln::initialize(output, input);
    for (int row = input.domain().pmin().row();
	 row <= input.domain().pmax().row(); ++row)
      {
	if (row % 2 == 0)
	  // Even row: line with vertices and horizontal edges.
	  for (int col = input.domain().pmin().col();
	       col <= input.domain().pmax().col(); ++col)
	    {
	      mln::point2d p(row, col);
	      if (col % 2 == 0)
		// Even column: vertex.
		output(p) = input(p) ? 'O' : '.';
	      else
		// Odd column: horizontal edge.
		output(p) = input(p) ? '-' : ' ';
	    }
	else
	  // Odd row: line with vertical edges and inter-edge spaces
	  // (squares).
	  for (int col = input.domain().pmin().col();
	       col <= input.domain().pmax().col(); ++col)
	    {
	      mln::point2d p(row, col);
	      if (col % 2 == 0)
		// Even column: vertical edge.
		output(p) = input(p) ? '|' : ' ';
	      else
		// Odd column: space.
		output(p) = ' ';
	    }
      }
    mln::debug::println(message, output);
  }


  /// Pretty-print an mln::bin_1complex_image2d viewed as a graph.
  void
  println_graph(const std::string& message,
		const mln::bin_1complex_image2d& input)
  {
    println_graph(message, convert::to_image2d(input));
  }

} // end of namespace debug


#endif // ! APPS_GRAPH_MORPHO_DEBUG_HH
