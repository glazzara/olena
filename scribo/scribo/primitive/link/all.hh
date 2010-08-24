// Copyright (C) 2009, 2010 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef SCRIBO_PRIMITIVE_LINK_ALL_HH
# define SCRIBO_PRIMITIVE_LINK_ALL_HH

/// \file
///
/// Include all headers located in scribo/primitive/group.

namespace scribo
{

  namespace primitive
  {

    /// Namespace of primitive linking related routines.
    namespace link
    {

    } // end of namespace scribo::primitive::group

  } // end of namespace scribo::primitive

} // end of namespace scribo




//# include <scribo/primitive/link/with_several_graphes.hh>
# include <scribo/primitive/link/with_several_right_links.hh>
//# include <scribo/primitive/link/with_graph.hh>
# include <scribo/primitive/link/with_single_left_link.hh>
# include <scribo/primitive/link/with_several_left_links.hh>
# include <scribo/primitive/link/with_single_right_link.hh>


#endif // ! SCRIBO_PRIMITIVE_LINK_ALL_HH
