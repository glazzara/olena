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

#ifndef OLENA_CORE_TOPO_ENTRY_HH
# define OLENA_CORE_TOPO_ENTRY_HH

# include <oln/core/abstract/entry.hh>
# include <oln/core/abstract/topo.hh>



namespace oln
{


  /// Entry class for point sets: topo_entry<E> is an alias for
  /// entry< abstract::topo, E>.

  template <typename E>
  struct topo_entry : public entry< abstract::topo, E>
  {
  protected:
    topo_entry() {}
  };


  /// Virtual types associated to topo_entry<E>.

  template <typename E>
  struct vtypes< topo_entry<E> >
  {
    typedef mlc::undefined bbox_type;
    typedef mlc::undefined ra_type;

    typedef mlc::undefined point_type;
  };


} // end of namespace oln


#endif // ! OLENA_CORE_TOPO_ENTRY_HH
