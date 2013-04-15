// Copyright (C) 2007, 2008, 2009, 2012, 2013 EPITA Research and
// Development Laboratory (LRDE)
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

#ifndef MLN_CORE_ALIAS_DPOINT1D_HH
# define MLN_CORE_ALIAS_DPOINT1D_HH

/// \file
///
/// Definition of the mln::dpoint1d alias and of its
/// construction routine.

# include <mln/core/dpoint.hh>
# include <mln/core/grids.hh>
# include <mln/core/def/coord.hh>


namespace mln
{

  /*!
    \class dpoint1d
    \headerfile <>

    \brief Type alias for a delta-point defined on the 1D square grid
    with integer coordinates.

    This class stored relative positions of a site.  It is meant to be
    used with point1d to compute a new point1d.

    \code
    point1d p(2);
    dpoint1d up(-1);

    dpoint1d p_before = p + before;
    // p_before = point1d(1);
    \endcode

    dpoint1d are also used in window1d to store the relative
    neighboors.

    \sa point1d, window1d
  */
  /// \cond ALIAS
  typedef dpoint<mln::grid::tick, def::coord> dpoint1d;
  /// \endcond


} // end of namespace mln


# include <mln/core/alias/point1d.hh>


namespace mln
{

  /// \brief Shortcut for delta point in 1d. dpoint1d(-1)
  extern const dpoint1d before;
  /// \brief Shortcut for delta point in 1d. dpoint1d(+1)
  extern const dpoint1d after;

  /// \brief Shortcut for delta point in 1d. dpoint1d(-1)
  extern const dpoint1d ind_dec;
  /// \brief Shortcut for delta point in 1d. dpoint1d(+1)
  extern const dpoint1d ind_inc;



# ifndef MLN_INCLUDE_ONLY

#  ifndef MLN_WO_GLOBAL_VARS

  const dpoint1d before = dpoint1d( -1 );
  const dpoint1d after  = dpoint1d( +1 );

  const dpoint1d ind_dec = before;
  const dpoint1d ind_inc = after;

#  endif // !MLN_WO_GLOBAL_VARS

# endif // ! MLN_INCLUDE_ONLY


} // end of namespace mln


#endif // ! MLN_CORE_ALIAS_DPOINT1D_HH
