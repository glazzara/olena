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

#ifndef MLN_CORE_ALIAS_DPOINT2D_HH
# define MLN_CORE_ALIAS_DPOINT2D_HH

/// \file
///
/// Definition of the mln::dpoint2d alias and of its
/// construction routine.

# include <mln/core/dpoint.hh>
# include <mln/core/def/coord.hh>


namespace mln
{

  /*!
    \class dpoint2d
    \headerfile <>

    \brief Type alias for a delta-point defined on the 2D square grid
    with integer coordinates.

    This class stored relative positions of a site.  It is meant to be
    used with point2d to compute a new point2d.

    \code
    point2d p(2,2);
    dpoint2d up(-1,0);

    dpoint2d p_up = p + up;
    // p_up = point2d(1,2);
    \endcode

    dpoint2d are also used in window2d to store the relative
    neighboors.

    \sa point2d, window2d
  */
  /// \cond ALIAS
  typedef dpoint<mln::grid::square, mln::def::coord> dpoint2d;
  /// \endcond


} // end of namespace mln


# include <mln/core/alias/point2d.hh>

namespace mln
{

  /// \brief Shortcut for delta point in 2d. dpoint2d(+1,0)
  extern const dpoint2d up;
  /// \brief Shortcut for delta point in 2d. dpoint2d(-1,0)
  extern const dpoint2d down;
  /// \brief Shortcut for delta point in 2d. dpoint2d(0,-1)
  extern const dpoint2d left;
  /// \brief Shortcut for delta point in 2d. dpoint2d(0,+1)
  extern const dpoint2d right;

  /// \brief Shortcut for delta point in 2d. dpoint2d(-1,-1)
  extern const dpoint2d up_left;
  /// \brief Shortcut for delta point in 2d. dpoint2d(-1,+1)
  extern const dpoint2d up_right;
  /// \brief Shortcut for delta point in 2d. dpoint2d(+1,-1)
  extern const dpoint2d down_left;
  /// \brief Shortcut for delta point in 2d. dpoint2d(+1,+1)
  extern const dpoint2d down_right;



# ifndef MLN_INCLUDE_ONLY

#  ifndef MLN_WO_GLOBAL_VARS

  const dpoint2d up    = dpoint2d( -1,  0 );
  const dpoint2d down  = dpoint2d( +1,  0 );
  const dpoint2d left  = dpoint2d(  0, -1 );
  const dpoint2d right = dpoint2d(  0, +1 );

  const dpoint2d up_left     = dpoint2d( -1,  -1 );
  const dpoint2d up_right    = dpoint2d( -1,  +1 );
  const dpoint2d down_left   = dpoint2d( +1,  -1 );
  const dpoint2d down_right  = dpoint2d( +1,  +1 );

#  endif // !MLN_WO_GLOBAL_VARS

# endif // ! MLN_INCLUDE_ONLY


} // end of namespace mln


#endif // ! MLN_CORE_ALIAS_DPOINT2D_HH
