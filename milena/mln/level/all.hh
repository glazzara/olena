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

#ifndef MLN_LEVEL_ALL_HH
# define MLN_LEVEL_ALL_HH

/*! \file mln/level/all.hh
 *
 * \brief File that includes all level-related routines.
 *
 */


namespace mln
{

  /// Namespace of image processing routines related to pixel levels.
  namespace level
  {

    /// Implementation namespace of level namespace.
    namespace impl {

      /// Generic implementation namespace of level namespace.
      namespace generic {}

    }

  }

}

# include <mln/level/abs.hh>
# include <mln/level/apply.hh>
# include <mln/level/approx/all.hh>
# include <mln/level/assign.hh>
# include <mln/level/compare.hh>
# include <mln/level/compute.hh>
# include <mln/level/fast_median.hh>
# include <mln/level/fill.hh>
# include <mln/level/median.hh>
# include <mln/level/memcpy_.hh>
# include <mln/level/memset_.hh>
# include <mln/level/naive/all.hh>
# include <mln/level/paste.hh>
# include <mln/level/saturate.hh>
# include <mln/level/sort_psites.hh>
# include <mln/level/stretch.hh>
# include <mln/level/to_enc.hh>
# include <mln/level/transform.hh>
# include <mln/level/was.median.hh>



#endif // ! MLN_LEVEL_ALL_HH
