// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_TEXT_GROUPING_INTERNAL_ALL_HH
# define SCRIBO_TEXT_GROUPING_INTERNAL_ALL_HH

/// \file scribo/text/grouping/internal/all.hh
///
/// Include all headers located in scribo/text/grouping/internal.

namespace scribo
{

  namespace text
  {

    namespace grouping
    {

      /// Namespace of internal routines grouping text components.
      namespace internal
      {

      } // end of namespace scribo::text::grouping::internal

    } // end of namespace scribo::text::grouping

  } // end of namespace scribo::text

} // end of namespace scribo


# include <scribo/text/grouping/internal/init_link_array.hh>
# include <scribo/text/grouping/internal/is_link_valid.hh>
# include <scribo/text/grouping/internal/find_root.hh>
# include <scribo/text/grouping/internal/find_graph_link.hh>
# include <scribo/text/grouping/internal/find_right_link.hh>
# include <scribo/text/grouping/internal/update_graph_link.hh>
# include <scribo/text/grouping/internal/update_link_array.hh>
# include <scribo/text/grouping/internal/find_left_link.hh>


#endif // ! SCRIBO_TEXT_GROUPING_INTERNAL_ALL_HH
