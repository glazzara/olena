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

#ifndef MLN_CORE_ALIAS_DPOINT3D_HH
# define MLN_CORE_ALIAS_DPOINT3D_HH

/// \file
///
/// Definition of the mln::dpoint3d alias and of its
/// construction routine.

# include <mln/core/dpoint.hh>
# include <mln/core/grids.hh>
# include <mln/core/def/coord.hh>

namespace mln
{

  /*!
    \class dpoint3d
    \headerfile <>

    \brief Type alias for a delta-point defined on the 3D square grid
    with integer coordinates.

    This class stored relative positions of a site.  It is meant to be
    used with point1d to compute a new point3d.

    \code
    point3d p(2,2,2);
    dpoint3d up(0,-1,0);

    dpoint3d p_up = p + up;
    // p_up = point3d(2,1,2);
    \endcode

    dpoint3d are also used in window3d to store the relative
    neighboors.

    \sa point3d, window3d
  */
  /// \cond ALIAS
  typedef dpoint<mln::grid::cube, def::coord> dpoint3d;
  /// \endcond


} // end of namespace mln


# include <mln/core/alias/point3d.hh>


namespace mln
{
  /// Definition of a shortcut for delta point in 3d. dpoint3d(0,0,-1)
  extern const dpoint3d sagittal_dec;
  /// Definition of a shortcut for delta point in 3d. dpoint3d(0,0,+1)
  extern const dpoint3d sagittal_inc;
  /// Definition of a shortcut for delta point in 3d. dpoint3d(0,-1,0)
  extern const dpoint3d axial_dec;
  /// Definition of a shortcut for delta point in 3d. dpoint3d(0,+1,0)
  extern const dpoint3d axial_inc;
  /// Definition of a shortcut for delta point in 3d. dpoint3d(-1,0,0)
  extern const dpoint3d coronal_dec;
  /// Definition of a shortcut for delta point in 3d. dpoint3d(+1,0,0)
  extern const dpoint3d coronal_inc;


# ifndef MLN_INCLUDE_ONLY

#  ifndef MLN_WO_GLOBAL_VARS

  const dpoint3d sagittal_dec = dpoint3d( 0,  0, -1);
  const dpoint3d sagittal_inc = dpoint3d( 0,  0, +1);
  const dpoint3d axial_dec    = dpoint3d( 0, -1,  0);
  const dpoint3d axial_inc    = dpoint3d( 0, +1,  0);
  const dpoint3d coronal_dec  = dpoint3d(-1,  0,  0);
  const dpoint3d coronal_inc  = dpoint3d(+1,  0,  0);

#  endif // !MLN_WO_GLOBAL_VARS

# endif // ! MLN_INCLUDE_ONLY


} // end of namespace mln


#endif // ! MLN_CORE_ALIAS_DPOINT3D_HH
