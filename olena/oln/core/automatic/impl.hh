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

#ifndef OLN_CORE_AUTOMATIC_IMPL_HH
# define OLN_CORE_AUTOMATIC_IMPL_HH

# include <oln/core/typedefs.hh>


namespace oln
{
  namespace automatic
  {
    /* Specialize to set an automatic implementation.  */
    template <template <class> class abstraction, typename tag, typename E>
    class set_impl
    {
    };

    /// Specialization of oln::automatic::get_impl saving the user
    /// the need to specify the morpher type as tag.
    template < template <class> class abstraction, typename E >
    class get_impl : public set_impl< abstraction, oln_vtype(E, morpher), E >
    {
    };

  } // end of namespace oln::automatic
  
} // end of namespace oln

#endif // ! OLN_CORE_AUTOMATIC_IMPL_HH
