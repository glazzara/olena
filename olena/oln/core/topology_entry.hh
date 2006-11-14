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

#ifndef OLN_CORE_TOPOLOGY_ENTRY_HH
# define OLN_CORE_TOPOLOGY_ENTRY_HH

# include <oln/core/abstract/entry.hh>
# include <oln/core/abstract/topology/all.hh>


namespace oln
{

  /// Fwd decl.
  template <typename E> struct topology_entry;


  /// Virtual types associated to topology_entry<E>.

  template <typename E>
  struct vtypes< topology_entry<E> >
  {
    typedef mlc::undefined point_type;
    typedef mlc::undefined bbox_type;
    typedef mlc::none      neighborhood_type;
    typedef mlc::undefined is_random_accessible_type;
    typedef mlc::none      subset_type;

    typedef mlc::none      morpher_type;
  };



  /// Entry class for point sets: topology_entry<E> is an alias for
  /// entry< abstract::topology, E>.

  template <typename E>
  struct topology_entry : public entry< abstract::topology, E>
  {
  protected:
    topology_entry();
  };


# ifndef OLN_INCLUDE_ONLY

  template <typename E>
  topology_entry<E>::topology_entry()
  {
  }

# endif


} // end of namespace oln


#endif // ! OLN_CORE_TOPOLOGY_ENTRY_HH
