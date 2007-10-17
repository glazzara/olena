// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_MORPHO_ALL_HH
# define MLN_MORPHO_ALL_HH

/*! \file mln/morpho/all.hh
 *
 * \brief File that includes all morpho-related routines.
 */


namespace mln
{

  /// Namespace of morphological image processing routines.
  namespace morpho
  {

    /// Namespace of morphological image processing routines
    /// implementation details.
    namespace impl
    {

      /// Namespace of morphological image processing routines generic
      /// implementations.
      namespace generic
      {
      }

    }
  }

}


# include <mln/morpho/closing.hh>
# include <mln/morpho/complementation.hh>
# include <mln/morpho/contrast.hh>
# include <mln/morpho/dilation_fast.hh>
# include <mln/morpho/dilation.hh>
# include <mln/morpho/erosion_fast.hh>
# include <mln/morpho/erosion.hh>
# include <mln/morpho/gradient.hh>
# include <mln/morpho/hit_or_miss.hh>
# include <mln/morpho/includes.hh>
# include <mln/morpho/laplacian.hh>
# include <mln/morpho/min.hh>
# include <mln/morpho/minus.hh>
# include <mln/morpho/opening_area.hh>
# include <mln/morpho/opening_attribute.hh>
# include <mln/morpho/opening.hh>
# include <mln/morpho/plus.hh>
# include <mln/morpho/Rd.hh>
# include <mln/morpho/thickening.hh>
# include <mln/morpho/thick_miss.hh>
# include <mln/morpho/thin_fit.hh>
# include <mln/morpho/thinning.hh>
# include <mln/morpho/top_hat.hh>



#endif // ! MLN_MORPHO_ALL_HH
