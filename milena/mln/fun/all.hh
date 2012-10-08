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

#ifndef MLN_FUN_ALL_HH
# define MLN_FUN_ALL_HH

/// \file
///
/// File that includes all functions.
///
/// \todo Incomplete list of files.


namespace mln
{

  /// Namespace of functions.
  namespace fun
  {

    /// Internal namespace of functions.
    namespace internal
    {}

  }
}


// Files.

# include <mln/fun/c.hh>
# include <mln/fun/ops.hh>


// Sub-directories.

# include <mln/fun/n2v/all.hh>

# include <mln/fun/i2v/all.hh>

# include <mln/fun/p2b/all.hh>
# include <mln/fun/p2v/all.hh>

# include <mln/fun/stat/all.hh>
# include <mln/fun/v2b/all.hh>
# include <mln/fun/v2i/all.hh>
# include <mln/fun/v2v/all.hh>
# include <mln/fun/vv2v/all.hh>
# include <mln/fun/x2p/all.hh>
# include <mln/fun/x2v/all.hh>
# include <mln/fun/x2x/all.hh>


#endif // ! MLN_FUN_ALL_HH
