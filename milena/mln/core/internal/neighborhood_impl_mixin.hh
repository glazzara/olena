// Copyright (C) 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_INTERNAL_NEIGHBORHOOD_IMPL_MIXIN_HH
# define MLN_CORE_INTERNAL_NEIGHBORHOOD_IMPL_MIXIN_HH

/*! \file mln/core/internal/neighborhood_impl_mixin.hh
 *
 * \brief Definition of a mixin to turn a window implementation class
 * into a neighborhood impl class.
 */


namespace mln
{

  namespace internal
  {

    template <typename W_impl, typename E>
    struct neighborhood_impl_mixin : W_impl
    {
      /// Site_Iterator type to browse the neighborhood sites.
      typedef typename W_impl::qiter niter;

      /// Site_Iterator type to browse the neighborhood sites in the
      /// forward way.
      typedef typename W_impl::fwd_qiter fwd_niter;
      
      /// Site_Iterator type to browse the neighborhood sites in the
      /// backward way.
      typedef typename W_impl::bkd_qiter bkd_niter;
    };

  } // end of namespace internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_NEIGHBORHOOD_IMPL_MIXIN_HH
