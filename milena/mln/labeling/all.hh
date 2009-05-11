// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_LABELING_ALL_HH
# define MLN_LABELING_ALL_HH

/// \file mln/labeling/all.hh
///
/// File that includes all labeling routines.
///
/// \todo Many files in this directory have to be updated with the
/// test and dispatch mechanisms.


namespace mln
{

  /// Namespace of labeling routines.
  namespace labeling
  {
    /// Implementation namespace of labeling namespace.
    namespace impl {

      /// Generic implementation namespace of labeling namespace.
      namespace generic {}

    }
  }
}

# include <mln/labeling/background.hh>
# include <mln/labeling/blobs.hh>
# include <mln/labeling/colorize.hh>
# include <mln/labeling/compute.hh>
# include <mln/labeling/fill_holes.hh>
# include <mln/labeling/flat_zones.hh>
# include <mln/labeling/foreground.hh>
# include <mln/labeling/level.hh>
# include <mln/labeling/n_max.hh>
# include <mln/labeling/pack.hh>
# include <mln/labeling/regional_maxima.hh>
# include <mln/labeling/regional_minima.hh>


#endif // ! MLN_LABELING_ALL_HH
