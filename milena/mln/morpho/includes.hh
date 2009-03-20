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

#ifndef MLN_MORPHO_INCLUDES_HH
# define MLN_MORPHO_INCLUDES_HH

/// \file mln/morpho/includes.hh
///
/// Basic list of includes for all files in mln/morpho/.


# include <mln/core/concept/image.hh>
# include <mln/core/concept/window.hh>
# include <mln/core/concept/neighborhood.hh>

# include <mln/core/routine/duplicate.hh>

# include <mln/value/ops.hh>

# include <mln/accu/rank.hh>

# include <mln/accu/transform.hh>
# include <mln/accu/transform_stop.hh>

# include <mln/accu/transform_directional.hh>
# include <mln/accu/transform_diagonal.hh>
# include <mln/accu/transform_line.hh>
# include <mln/accu/transform_snake.hh>

# include <mln/fun/v2v/saturate.hh>

# include <mln/level/compare.hh>
# include <mln/data/fill.hh>

# include <mln/test/positive.hh>

# include <mln/extension/all.hh>

# include <mln/win/sym.hh>
# include <mln/win/shift.hh>
# include <mln/win/diff.hh>
# include <mln/win/inter.hh>

# include <mln/morpho/erosion.hh>
# include <mln/morpho/complementation.hh>
# include <mln/morpho/dilation.hh>

# include <mln/morpho/min.hh>
# include <mln/morpho/minus.hh>
# include <mln/morpho/plus.hh>


#endif // ! MLN_MORPHO_INCLUDES_HH
