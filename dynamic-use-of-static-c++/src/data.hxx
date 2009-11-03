/* Copyright (C) 2005 EPITA Research and Development Laboratory (LRDE)

   This file is part of Olena.

   Olena is free software: you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation, version 2 of the License.

   Olena is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Olena.  If not, see <http://www.gnu.org/licenses/>.

   As a special exception, you may use this file as part of a free
   software project without restriction.  Specifically, if other files
   instantiate templates or use macros or inline functions from this
   file, or you compile this file and link it with other files to produce
   an executable, this file does not by itself cause the resulting
   executable to be covered by the GNU General Public License.  This
   exception does not however invalidate any other reasons why the
   executable file might be covered by the GNU General Public License.  */

#ifndef DYN_DATA_HXX
# define DYN_DATA_HXX

# include <string>
# include <cassert>
# include <iostream>

# include "function.hh"
# include "name_of.hh"

namespace dyn {

  template <typename T>
  T data::convert_to() const
  {
    static fun dyn_data_cast(std::string("data_cast< ") +
                             type() + ", " + mlc_name<T>::of() + " >");
    return dyn_data_cast(*this);
  }


  template <typename T>
  T& data::get_ref_on()
  {
    assert(proxy_);
    data_proxy_by_ref<T>* reinterpret_cast_returned_pointer = reinterpret_cast<data_proxy_by_ref<T>*>(proxy_);
    assert(reinterpret_cast_returned_pointer);
    return reinterpret_cast_returned_pointer->obj();
  }


  template <typename T>
  const T& data::get_ref_on() const
  {
    assert(proxy_);
    data_proxy<T>* reinterpret_cast_returned_pointer = reinterpret_cast<data_proxy<T>*>(proxy_);
    assert(reinterpret_cast_returned_pointer);
    return reinterpret_cast_returned_pointer->const_ref();
  }


  template <typename T>
  data& data::operator=(const T& rhs)
  {
    assert(proxy_);
    static fun dyn_data_assign(std::string("data_assign<") + proxy()->type() + ", " + mlc_name<T>::of() + ">");
    dyn_data_assign(*this, rhs);
    return *this;
  }

}

std::ostream& operator<<(std::ostream& ostr, const dyn::data& d);
std::istream& operator>>(std::istream& istr, dyn::data& d);
dyn::data& operator++(dyn::data& d);
dyn::data& operator--(dyn::data& d);
bool operator!=(const dyn::data& lhs, const dyn::data& rhs);
bool operator==(const dyn::data& lhs, const dyn::data& rhs);
dyn::data operator+(const dyn::data& lhs, const dyn::data& rhs);

#endif // ! DYN_DATA_HXX
