// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_ACCU_STAT_ALL_HH
# define MLN_ACCU_STAT_ALL_HH

/// \file mln/accu/stat/all.hh
///
/// File that includes all statistical accumulator types.


namespace mln
{
  namespace accu
  {
    namespace meta
    {
      /// Namespace of statistical meta-accumulators.
      namespace stat {}
    }

    /// Namespace of statistical accumulators.
    namespace stat {}

  }
}


# include <mln/accu/stat/deviation.hh>
# include <mln/accu/stat/var.hh>
# include <mln/accu/stat/variance.hh>
# include <mln/accu/stat/max.hh>
# include <mln/accu/stat/min.hh>
# include <mln/accu/stat/max_h.hh>
# include <mln/accu/stat/min_h.hh>
# include <mln/accu/stat/min_max.hh>
# include <mln/accu/stat/mean.hh>
# include <mln/accu/stat/median_h.hh>

#endif // ! MLN_ACCU_STAT_ALL_HH
