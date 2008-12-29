// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CORE_ROUTINE_INIT_HXX
# define MLN_CORE_ROUTINE_INIT_HXX

/// \file mln/core/routine/init.hxx
///
// All overloaded "mln::init" routines have to be declared in
// the present file.  It allows for *deferred* use of these routines.

namespace mln
{

  // Forward declarations.
  namespace fun { namespace i2v { template <typename T> class array; } }
  namespace fun { namespace l2l { template <typename T> class relabel; } }
  namespace pw { template <typename F, typename S> class image; }
  template <typename I, typename F> class image_if;



  /// Kind: function_t
  /// Type: F,
  /// From: pw::image<F,S>
  template <typename F, typename S>
  void init_(tag::function_t, F& f, const pw::image<F,S>& model);


  /// Kind: image_t
  /// Type: pw::image<F,S>
  /// From: J
  template <typename F, typename S, typename J>
  void init_(tag::image_t, mln::pw::image<F,S>& target, const J& model);


  /// Kind: function_t
  /// Type: F
  /// From: image_if<I,F>
  template <typename I, typename F>
  void init_(tag::function_t, F& f, const image_if<I,F>& model);

  /// Kind: image_t
  /// Type: F
  /// From: J
  template <typename I, typename F, typename J>
  void init_(tag::image_t, image_if<I,F>& target, const J& model);


  /// Kind: function_t
  /// Type: fun::i2v::array<T1>
  /// From: fun::i2v::array<T2>
  template <typename T1, typename T2>
  void init_(tag::function_t,
	     fun::i2v::array<T1>&	  f,
	     const fun::i2v::array<T2>&	  model);

  /// Kind: function_t
  /// Type: fun::l2l::relabel<T1>
  /// From: fun::l2l::relabel<T2>
  template <typename T1, typename T2>
  void init_(tag::function_t,
	     fun::l2l::relabel<T1>&	  f,
	     const fun::l2l::relabel<T2>& model);

} // end of namespace mln

#endif // ! MLN_CORE_ROUTINE_INIT_HXX
