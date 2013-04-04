// Copyright (C) 2008, 2009, 2013 EPITA Research and Development
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

#ifndef MLN_CORE_ALIAS_P_RUN2D_HH
# define MLN_CORE_ALIAS_P_RUN2D_HH

/*! \file
 *
 * \brief Definition of the mln::p_run2d alias.
 */

# include <mln/core/site_set/p_run.hh>
# include <mln/core/alias/point2d.hh>


namespace mln
{

  /*!
    \class p_run2d
    \headerfile <>

    \brief Type alias for a run of 2d points.

    \ingroup modsitesetbasic
  */
  /// \cond ALIAS
  typedef p_run<point2d> p_run2d;
  /// \endcond


} // end of namespace mln


#endif // ! MLN_CORE_ALIAS_P_RUN2D_HH
