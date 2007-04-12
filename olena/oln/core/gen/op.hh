// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef OLN_CORE_GEN_OP_HH
# define OLN_CORE_GEN_OP_HH

# include <oln/core/internal/special_op.hh>
# include <oln/core/internal/category_of.hh>


namespace oln
{

  // Fwd decl.
  template <typename L, typename OpName, typename R> class op_;


  // Virtual types.
  template <typename L, typename OpName, typename R>
  struct vtypes< op_<L, OpName, R> >
  {
  };


  // Super type.

# define  super \
  internal::special_op_< oln_category_of_(L), L, OpName, oln_category_of_(R), R >

  template <typename L, typename OpName, typename R>
  struct super_trait_< op_<L, OpName, R> >
  {
    typedef super ret;
  };


  /// Class for result of "L op R".

  template <typename L, typename OpName, typename R>
  class op_ : public super
  {
  public:
    op_();
    op_(L& l, R& r);
    template <typename D>
    op_(const internal::initializer_<D>& data);

  }; // end of op_<L, OpName, R>



# ifndef OLN_INCLUDE_ONLY

  template <typename L, typename OpName, typename R>
  op_<L, OpName, R>::op_()
  {
  }

  template <typename L, typename OpName, typename R>
  op_<L, OpName, R>::op_(L& l, R& r)
    : super (l, r)
  {
  }

  template <typename L, typename OpName, typename R>
  template <typename D>
  op_<L, OpName, R>::op_(const internal::initializer_<D>& data)
  {
    bool op_ok = this->super::init__(data);
    postcondition(op_ok);
  }

# endif // ! OLN_INCLUDE_ONLY

# undef super

} // end of namespace oln


#endif // ! OLN_CORE_GEN_OP_HH
