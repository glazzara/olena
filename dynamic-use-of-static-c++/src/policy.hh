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

#ifndef DYN_POLICY_HH
# define DYN_POLICY_HH

# include "data.hh"

namespace dyn
{
  namespace policy
  {
    
#   define select_dyn_policy(e)                                               \
      (dyn::policy::type)dyn::policy::simplify<                               \
        sizeof((dyn::policy::id_for_pod_2(), e, dyn::policy::id_for_pod_2())) \
      + sizeof((dyn::policy::id_for_ptr_and_ref_2(), e,                       \
                dyn::policy::id_for_ptr_and_ref_2()))>::val

    template <unsigned n>
    struct id
    {
      char d[n];
    };

    template <class T> struct check_is_const { enum { val = none }; };
    template <class T> struct check_is_const <const T> { enum { val = is_const }; };

    template <unsigned N>
    struct id_for_pod : id<N>
    {
      id_for_pod<N>() {}
      template <unsigned M> id_for_pod<N>(id_for_pod<M>&) {}
      id_for_pod<N>& operator() () { return *this; }

      template <typename T>
      id_for_pod<N + is_pod> operator, (T) { assert(0); }
    };
    extern id_for_pod<2> id_for_pod_2;

    template <unsigned N>
    struct id_for_ptr_and_ref : id<N>
    {
      id_for_ptr_and_ref<N>() {}
      template <unsigned M> id_for_ptr_and_ref<N>(id_for_ptr_and_ref<M>&) {}
      id_for_ptr_and_ref<N>& operator() () { return *this; }

      template <typename T>
      id_for_ptr_and_ref<N + is_ref + check_is_const<T>::val> operator, (T&) { assert(0); }

      template <typename T>
      id_for_ptr_and_ref<N + is_ptr + check_is_const<T>::val> operator, (T*) { assert(0); }
    };
    extern id_for_ptr_and_ref<2> id_for_ptr_and_ref_2;

    template <unsigned n>
    struct simplify;

#   define set_simplify(x, y) \
    template <> struct simplify<x + 20> { enum { val = y }; }

    set_simplify(-16, is_void);
    set_simplify(0,   is_pod);
    set_simplify(4,   is_ref);
    set_simplify(5,   is_ref + is_const);
    set_simplify(6,   is_ptr);
    set_simplify(7,   is_ptr + is_const);

    template <type policy>
    struct receiver
    {
      receiver() : proxy_(0) { logger << "receiver() [ policy = " << policy << " ]" << std::endl; }

      receiver& operator() ()
      {
        return *this;
      }

      template <typename T>
      receiver& operator,(T& obj)
      {
        logger << "receiver::operator,(T&) [ T = " << mlc_name<T>::of() << " ]" << std::endl;
        proxy_ = new typename dyn_choose_data_proxy<T, policy>::ret(obj);
        return *this;
      }

      template <typename T>
      receiver& operator,(const T& obj)
      {
        logger << "receiver::operator,(const T&) [ T = " << mlc_name<const T>::of() << " ]" << std::endl;
        proxy_ = new typename dyn_choose_data_proxy<T, policy>::ret(obj);
        return *this;
      }

      abstract_data* proxy()
      {
        if (proxy_)
          return proxy_;
        else
          return new data_nil;
      }

      protected:
      abstract_data* proxy_;
    };

  }
}

#endif // ! DYN_POLICY_HH
