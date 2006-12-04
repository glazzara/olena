// Copyright (C) 2006 EPITA Research and Development Laboratory
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

#ifndef OLN_CORE_POINT_SET_ENTRY_HH
# define OLN_CORE_POINT_SET_ENTRY_HH

# include <oln/core/abstract/entry.hh>
# include <oln/core/abstract/point_set/all.hh>


namespace oln
{

  /// Fwd decl.
  template <typename E> struct point_set_entry;


  template <typename E>
  struct set_super_type< point_set_entry<E> >
  {
    typedef mlc::none ret;
  };


  /// Virtual types associated to point_set_entry<E>.

  template <typename E>
  struct vtypes< point_set_entry<E> >
  {
    typedef stc::abstract point_type;

    typedef stc::abstract fwd_piter_type;
    typedef stc::abstract bkd_piter_type;

    typedef mlc::none      bbox_type;
    typedef stc::abstract is_random_accessible_type;
    typedef stc::abstract has_known_size_type;
    typedef stc::abstract is_connected_type;

    // final.
    typedef oln_deduce_vtype(E, point, coord) coord_type;
    typedef oln_deduce_vtype(E, point, grid)  grid_type;
    typedef oln_deduce_vtype(E, point, dim)   dim_type;
    typedef oln_fwd_piter(E)            piter_type;
  };



  /// Entry class for point sets: point_set_entry<E> is an alias for
  /// entry< abstract::point_set, E>.

  template <typename E>
  struct point_set_entry : public entry< abstract::point_set, E>
  {
  protected:
    point_set_entry();
  };



# ifndef OLN_INCLUDE_ONLY

  template <typename E>
  point_set_entry<E>::point_set_entry()
  {
  }

# endif


} // end of namespace oln


#endif // ! OLN_CORE_POINT_SET_ENTRY_HH
