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

#ifndef MLN_MORPHO_ALL_HH
# define MLN_MORPHO_ALL_HH

/// \file
///
/// File that includes all mathematical morphology routines.


namespace mln
{

  /// Namespace of mathematical morphology routines.
  namespace morpho
  {

    /// Namespace of mathematical morphology routines
    /// implementations.
    namespace impl
    {

      /// Namespace of mathematical morphology routines
      /// generic implementations.
      namespace generic
      {}

    }
  }
}


# include <mln/morpho/complementation.hh>
# include <mln/morpho/contrast.hh>
# include <mln/morpho/dilation.hh>
# include <mln/morpho/erosion.hh>
# include <mln/morpho/general.hh>
# include <mln/morpho/gradient.hh>
# include <mln/morpho/hit_or_miss.hh>
# include <mln/morpho/includes.hh>
# include <mln/morpho/laplacian.hh>
# include <mln/morpho/min.hh>
# include <mln/morpho/minus.hh>
# include <mln/morpho/plus.hh>
# include <mln/morpho/Rd.hh>
# include <mln/morpho/rank_filter.hh>
# include <mln/morpho/thickening.hh>
# include <mln/morpho/thick_miss.hh>
# include <mln/morpho/thin_fit.hh>
# include <mln/morpho/thinning.hh>
# include <mln/morpho/top_hat.hh>


// Sub-directories.

# include <mln/morpho/approx/all.hh>
# include <mln/morpho/attribute/all.hh>
# include <mln/morpho/closing/all.hh>
# include <mln/morpho/elementary/all.hh>
# include <mln/morpho/opening/all.hh>
# include <mln/morpho/reconstruction/all.hh>
# include <mln/morpho/tree/all.hh>
# include <mln/morpho/watershed/all.hh>



#endif // ! MLN_MORPHO_ALL_HH
