// Copyright (C) 2007 EPITA Research and Development Laboratory.
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

#ifndef STC_DOC_TINY_MORE_SCOOP_2_REVERSE_ITERATOR_HH

/* \file doc/tiny/more_scoop_2/reverse_iterator.hh

   \brief Specific definitions for reverse applying on iterators.  */


namespace abc
{

    template <typename T>
    struct single_vtype < reverse<T>, typedef_::forward >
    {
      typedef stc_prop(T, backward) ret;
    };


    template <typename T>
    struct single_vtype < reverse<T>, typedef_::backward >
    {
      typedef stc_prop(T, forward) ret;
    };



  namespace automatic
  {

    // default:  behavior::reverse  is  behavior::identity

    template <template <class> class abstraction, typename Exact>
    struct set_impl< abstraction, behavior::reverse, Exact > : public impl< abstraction, behavior::identity, Exact >
    {
    };


    // Forward_Iterator

    template <typename Exact>
    struct set_impl< Forward_Iterator, behavior::reverse, Exact > : public virtual any<Exact>
    {
      void impl_next()  { this->exact().delegatee_.prev();  }
    };


    // Backward_Iterator

    template <typename Exact>
    struct set_impl< Backward_Iterator, behavior::reverse, Exact > : public virtual any<Exact>
    {
      void impl_prev()  { this->exact().delegatee_.next();  }
    };


  } // end of namespace abc::automatic



  template <typename I>
  reverse<I> make_reverse(Iterator<I>& iter)
  {
    reverse<I> tmp(iter.exact());
    return tmp;
  }

  
} // end of namespace abc


#endif // ! STC_DOC_TINY_MORE_SCOOP_2_REVERSE_ITERATOR_HH
