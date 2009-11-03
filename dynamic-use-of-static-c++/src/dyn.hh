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

#ifndef DYN_DYN_HH
# define DYN_DYN_HH

# include "config.hh"

# define DYN_FULL_IMPLEMENTATION

# include "data.hh"

# include "function.hh"

namespace dyn
{
  namespace language
  {
    struct val;

    struct var : public data
    {
      var() : data() { logger << "var()" << std::endl; }

      template <class T>
      var(const T& obj) : data((abstract_data*)new data_proxy_by_cpy<T>(obj), (proxy_tag*)0)
      {
        logger << "var(const T& obj) [ T = " << mlc_name<T>::of() << " ]" << std::endl;
      }

      template <class T>
      var(T& obj) : data(obj) { logger << "var(T& obj) [ T = " << mlc_name<T>::of() << " ]" << std::endl; }

      var(const data& rhs) : data(rhs) { logger << "var(const data& rhs)" << std::endl; }
      var(const var& rhs)  : data(rhs) { logger << "var(const var& rhs)" << std::endl; }
      var(const val& rhs)  : data(rhs) { logger << "var(const val& rhs)" << std::endl; }
      var(data& rhs) : data(rhs) { logger << "var(data& rhs)" << std::endl; }
      var(var& rhs)  : data(rhs) { logger << "var(var& rhs)" << std::endl; }
      var(val& rhs)  : data(rhs) { logger << "var(val& rhs)" << std::endl; }
    };

    struct val : public data
    {
      template <class T>
      val(const T& obj) : data(obj) { logger << "val(const T& obj) [ T = " << mlc_name<T>::of() << " ]" << std::endl; }

      val(const data& rhs) : data(rhs) { logger << "val(const data& rhs)" << std::endl; }
      val(const var& rhs) : data() {
        logger << "val(const var& rhs)" << std::endl;
        const_assign(rhs);
      }
      val(const val& rhs)  : data(rhs) { logger << "val(const val& rhs)" << std::endl; }
    };

    typedef ::dyn::fun  fun;
    typedef ::dyn::ctor ctor;
    typedef ::dyn::meth meth;
    typedef ::dyn::meth method;
  }
}

# include "function_loader.hh"

#endif // ! DYN_DYN_HH
