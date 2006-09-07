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

#ifndef OLENA_CORE_PSET_ENTRY_HH
# define OLENA_CORE_PSET_ENTRY_HH

# include <oln/core/abstract/entry.hh>
# include <oln/core/abstract/pset.hh>



namespace oln
{


  /// Entry class for point sets: pset_entry<E> is an alias for
  /// entry< abstract::pset, E>.

  template <typename E>
  struct pset_entry : public entry< abstract::pset, E>
  {
  protected:
    pset_entry() {}
  };


  /// Virtual types associated to pset_entry<E>.

  template <typename E>
  struct vtypes< pset_entry<E> >
  {
    typedef mlc::undefined point_type;
    typedef mlc::undefined fwd_piter_type;
    typedef mlc::undefined bkd_piter_type;

    typedef mlc::none      bbox_type;
    typedef mlc::undefined ra_type;
    typedef mlc::undefined fixed_type;
  };


  template <typename E>
  struct single_vtype< pset_entry<E>, typedef_::coord_type >
  {
    typedef oln_type_of(E, point) P;
    typedef oln_type_of(P, coord) ret;
  };


  template <typename E>
  struct single_vtype< pset_entry<E>, typedef_::grid_type >
  {
    typedef oln_type_of(E, point) P;
    typedef oln_type_of(P, grid) ret;
  };


  template <typename E>
  struct single_vtype< pset_entry<E>, typedef_::dim_type >
  {
    typedef oln_type_of(E, point) P;
    typedef oln_type_of(P, dim) ret;
  };


} // end of namespace oln


#endif // ! OLENA_CORE_PSET_ENTRY_HH
