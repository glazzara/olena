// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_WIN_SPHERE3D_HH
# define MLN_WIN_SPHERE3D_HH

/// \file
///
/// Definition of the mln::win::sphere3d window.

# include <mln/win/ball.hh>
# include <mln/core/alias/dpoint3d.hh>


namespace mln
{

  namespace win
  {

    /// \brief 3D sphere window; precisely, ball-shaped window defined on the
    /// 3D cubic grid.
    ///
    /// \ingroup modwin3d
    typedef ball<grid::cube, def::coord> sphere3d;


  } // end of namespace mln::win

} // end of namespace mln



#endif // ! MLN_WIN_SPHERE3D_HH
