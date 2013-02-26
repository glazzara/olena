// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_DATA_ALL_HH
# define MLN_DATA_ALL_HH

/// \file
///
/// File that includes all data-related routines.


namespace mln
{

  /// Namespace of image processing routines related to pixel data.
  namespace data
  {

    /// Implementation namespace of data namespace.
    namespace impl {

      /// Generic implementation namespace of data namespace.
      namespace generic {}

    }
  }
}


# include <mln/data/abs.hh>
# include <mln/data/apply.hh>
# include <mln/data/approx/all.hh>
# include <mln/data/compare.hh>
# include <mln/data/compute.hh>
# include <mln/data/convert.hh>
# include <mln/data/fast_median.hh>
# include <mln/data/fill.hh>
# include <mln/data/median.hh>
# include <mln/data/naive/all.hh>
# include <mln/data/paste.hh>
# include <mln/data/replace.hh>
# include <mln/data/saturate.hh>
# include <mln/data/sort_offsets.hh>
# include <mln/data/sort_psites.hh>
# include <mln/data/stretch.hh>
# include <mln/data/to_enc.hh>
# include <mln/data/transform.hh>
# include <mln/data/update.hh>

# include <mln/data/wrap.hh>



#endif // ! MLN_DATA_ALL_HH
