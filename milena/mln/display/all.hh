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

#ifndef MLN_DISPLAY_ALL_HH
# define MLN_DISPLAY_ALL_HH

/*! \file mln/display/all.hh
 *
 * \brief File that includes all display routines.
 */


namespace mln
{

  /// Namespace of routines that help to display images.
  namespace display
  {
    /// \internal Implementation namespace of display namespace.
    namespace impl {

      /// \internal Generic implementation namespace of display namespace.
      namespace generic {}

    }

  }

}

# include <mln/display/color_pretty.hh>
# include <mln/display/remove.hh>
# include <mln/display/save_and_show.hh>
# include <mln/display/save.hh>
# include <mln/display/show.hh>

#endif // ! MLN_DISPLAY_ALL_HH
