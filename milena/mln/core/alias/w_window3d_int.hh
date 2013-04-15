// Copyright (C) 2007, 2008, 2009, 2013 EPITA Research and Development
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

#ifndef MLN_CORE_ALIAS_W_WINDOW3D_INT_HH
# define MLN_CORE_ALIAS_W_WINDOW3D_INT_HH

/*! \file
 *
 * \brief Definition of the mln::w_window3d_int alias.
 */

# include <mln/core/w_window.hh>
# include <mln/core/alias/dpoint3d.hh>


namespace mln
{

  /*!
    \class w_window3d_int
    \headerfile <>

    \brief Type alias for a w_window with arbitrary shape, defined
    on the 3D grid (with integer coordinates) and whose
    weights are integers.

    \ingroup modwin3d
  */
  /// \cond ALIAS
  typedef w_window<dpoint3d, int> w_window3d_int;
  /// \endcond


} // end of namespace mln


# include <mln/make/w_window3d.hh>


#endif // ! MLN_CORE_ALIAS_W_WINDOW3D_INT_HH
