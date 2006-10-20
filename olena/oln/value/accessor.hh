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

#ifndef OLN_VALUE_ACCESSOR_HH
# define OLN_VALUE_ACCESSOR_HH

#include <oln/morpher/thru_fun.hh>
#include <xtd/accessor.hh>


xtd_decl_accessor(value);
xtd_decl_accessor(vec);

xtd_decl_accessor(red);
xtd_decl_accessor(green);
xtd_decl_accessor(blue);


namespace oln
{

  static const xtd::accessor::value_type a_value;
  static const xtd::accessor::vec_type   a_vec;

  static const xtd::accessor::red_type   a_red;
  static const xtd::accessor::green_type a_green;
  static const xtd::accessor::blue_type  a_blue;

} // end of namespace oln


#endif // ! OLN_VALUE_ACCESSOR_HH
