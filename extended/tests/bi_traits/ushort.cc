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

#include <iostream>
#include <typeinfo>

#include <mlc/assert.hh>
#include <mlc/cmp.hh>

#include <xtd/optraits.hh>


# define check_op_trait(OpName, OpSymb, T1, T2) \
mlc::assert_< mlc::eq_< typeof(T1() OpSymb T2()), xtd_op_##OpName##_trait_(T1, T2) > >::check()

# define check_trait(T1, T2)			\
check_op_trait(plus, +, T1, T2);		\
check_op_trait(minus, -, T1, T2);		\
check_op_trait(mult, *, T1, T2)

// check_op_trait(mod, %, T1, T2);



int main()
{
  typedef long double ldouble;
  typedef unsigned char uchar;
  typedef signed char schar;
  typedef unsigned short ushort;
  typedef signed short sshort;
  typedef unsigned int uint;
  typedef signed int sint;
  typedef unsigned long ulong;
  typedef signed long slong;

  check_trait(ushort, bool);
  check_trait(ushort, char);
  check_trait(ushort, float);
  check_trait(ushort, double);
  check_trait(ushort, ldouble);
  check_trait(ushort, uchar);
  check_trait(ushort, schar);
  check_trait(ushort, ushort);
  check_trait(ushort, sshort);
  check_trait(ushort, uint);
  check_trait(ushort, sint);
  check_trait(ushort, ulong);
  check_trait(ushort, slong);

}
