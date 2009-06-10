// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_MAKE_ALL_HH
# define MLN_MAKE_ALL_HH

/// \file
///
/// File that includes all make routines.


namespace mln
{

  /// Namespace of routines that help to make Milena's objects.
  namespace make {}

}

# include <mln/make/box1d.hh>
# include <mln/make/box2d.hh>
# include <mln/make/box2d_h.hh>
# include <mln/make/box3d.hh>
# include <mln/make/dpoint2d_h.hh>
# include <mln/make/dual_neighb.hh>
# include <mln/make/image.hh>
# include <mln/make/image2d.hh>
# include <mln/make/image3d.hh>
# include <mln/make/mat.hh>
# include <mln/make/pix.hh>
# include <mln/make/pixel.hh>
# include <mln/make/point2d_h.hh>
# include <mln/make/vec.hh>
# include <mln/make/voronoi.hh>
# include <mln/make/w_window.hh>
# include <mln/make/w_window1d.hh>
# include <mln/make/w_window1d_int.hh>
# include <mln/make/w_window2d.hh>
# include <mln/make/w_window2d_int.hh>
# include <mln/make/w_window3d.hh>
# include <mln/make/w_window3d_int.hh>
# include <mln/make/w_window_directional.hh>
# include <mln/make/win_chamfer.hh>

# include <mln/make/influence_zone_adjacency_graph.hh>
# include <mln/make/region_adjacency_graph.hh>
# include <mln/make/rag_and_labeled_wsl.hh>

#endif // ! MLN_MAKE_ALL_HH
