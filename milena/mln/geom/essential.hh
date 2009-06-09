// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_GEOM_ESSENTIAL_HH
# define MLN_GEOM_ESSENTIAL_HH

/// \file
///
/// File that includes essential geometry related things.


namespace mln
{

  /// Namespace of essential things related to geometry.
  namespace geom
  {
    /// Implementation namespace of geom namespace.
    namespace impl {}
  }
}


# include <mln/geom/bbox.hh>
# include <mln/geom/chamfer.hh>
# include <mln/geom/size1d.hh>
# include <mln/geom/size2d.hh>
# include <mln/geom/size3d.hh>
# include <mln/geom/nsites.hh>

# include <mln/geom/max_col.hh>
# include <mln/geom/max_ind.hh>
# include <mln/geom/max_row.hh>
# include <mln/geom/max_sli.hh>

# include <mln/geom/min_col.hh>
# include <mln/geom/min_ind.hh>
# include <mln/geom/min_row.hh>
# include <mln/geom/min_sli.hh>


#endif // ! MLN_GEOM_ESSENTIAL_HH
