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

#ifndef STC_DOC_TINY_MORE_SCOOP_2_CONCEPTS_HH

/* \file doc/tiny/more_scoop_2/concepts.hh

   \brief Forward declaration of concepts.  */


namespace abc
{

  template <typename Exact> struct Iterator;

  template <typename Exact> struct Forward_Iterator;
  template <typename Exact> struct Backward_Iterator;
  template <typename Exact> struct Bidirectional_Iterator;

  template <typename Exact> struct Read_Iterator;
  template <typename Exact> struct Read_Only_Iterator;
  template <typename Exact> struct Write_Iterator;
  template <typename Exact> struct Write_Only_Iterator;
  template <typename Exact> struct Read_Write_Iterator;

} // end of namespace abc


#endif // ! STC_DOC_TINY_MORE_SCOOP_2_CONCEPTS_HH
