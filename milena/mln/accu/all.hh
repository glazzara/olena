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

#ifndef MLN_ACCU_ALL_HH
# define MLN_ACCU_ALL_HH

/// \file
///
/// File that includes all accumulator types.
///
/// \todo Update accumulators s.a. count so that they are like min_/min.
///
/// \todo Propagate those updates to mln/estim/.
///
/// \todo Update the include list...


namespace mln
{

  /// Namespace of accumulators.
  namespace accu
  {
    /// Implementation namespace of accumulator namespace.
    namespace impl {}

    /// Internal namespace of accumulators.
    namespace internal {}
  }

}


# include <mln/accu/bbox.hh>
# include <mln/accu/count_labels.hh>
# include <mln/accu/center.hh>
// << # include <mln/accu/count_adjacent_vertices.hh> >>
# include <mln/accu/height.hh>
# include <mln/accu/histo.hh>
# include <mln/accu/label_used.hh>
# include <mln/accu/line.hh>
# include <mln/accu/nil.hh>
# include <mln/accu/pair.hh>
# include <mln/accu/rank.hh>
# include <mln/accu/rms.hh>
// << # include <mln/accu/tuple.hh> >>
# include <mln/accu/volume.hh>

// Routines.

# include <mln/accu/convolve.hh>
# include <mln/accu/transform.hh>
# include <mln/accu/transform_stop.hh>

# include <mln/accu/transform_directional.hh>
# include <mln/accu/transform_diagonal.hh>
# include <mln/accu/transform_line.hh>
# include <mln/accu/transform_snake.hh>

// Sub-directories

# include <mln/accu/image/all.hh>
# include <mln/accu/site_set/all.hh>
# include <mln/accu/stat/all.hh>
# include <mln/accu/logic/all.hh>


#endif // ! MLN_ACCU_ALL_HH
