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

#ifndef OLN_CORE_INIT_HH
# define OLN_CORE_INIT_HH

# include <oln/core/internal/utils.hh>
# include <oln/core/internal/instant_value.hh>
# include <oln/core/internal/initializer.hh>



namespace oln
{


  template <typename D>
  internal::initializer_<D>
  init(const D& d);


  template <template<class> class M, typename V>
  internal::initializer_< internal::singleton< M<V> > >
  init(const internal::instant_value_<M,V>& v);


  template <template<class> class M1, typename V1,
	    template<class> class M2, typename V2>
  internal::initializer_< internal::pair< M1<V1>, M2<V2> > >
  init(const internal::instant_value_<M1,V1>& v1,
       const internal::instant_value_<M2,V2>& v2);


  template <template<class> class M1, typename V1,
	    template<class> class M2, typename V2,
	    template<class> class M3, typename V3>
  internal::initializer_< internal::triplet< M1<V1>, M2<V2>, M3<V3> > >
  init(const internal::instant_value_<M1,V1>& v1,
       const internal::instant_value_<M2,V2>& v2,
       const internal::instant_value_<M3,V3>& v3);

  // ...


  const struct with_t {} with = with_t();


  /*
   *
   * // To be specialized.
   * template <typename This_, typename Data>
   * bool init_(This_* this_, const Data& data); // FIXME: give code? err?
   *
   */


  // The init versions below calls the above init_ routine.
  // FIXME: Add oln::Any<*> to signature?


  // Regular version.
  template <typename Target, typename Data>
  bool init(Any<Target>& target, with_t, const Any<Data>& data);

  // Assignment.
  template <typename T>
  bool init(Any<T>& target, with_t, const Any<T>& data);

  // Unconst->const data version.
  template <typename Target, typename Data>
  bool init(Any<Target>& target, with_t, Any<Data>& data);

  // Guard: we cannot have "const Target".
  template <typename Target, typename Data>
  bool init(const Any<Target>& target, with_t, const Any<Data>& data);



# ifndef OLN_INCLUDE_ONLY

  template <typename D>
  internal::initializer_<D>
  init(const D& d)
  {
    return d;
  }

  template <template<class> class M, typename V>
  internal::initializer_< internal::singleton< M<V> > >
  init(const internal::instant_value_<M,V>& v)
  {
    internal::singleton< M<V> > tmp(v.value);
    return tmp;
  }

  template <template<class> class M1, typename V1,
	    template<class> class M2, typename V2>
  internal::initializer_< internal::pair< M1<V1>, M2<V2> > >
  init(const internal::instant_value_<M1,V1>& v1,
       const internal::instant_value_<M2,V2>& v2)
  {
    internal::pair< M1<V1>, M2<V2> > tmp(v1.value, v2.value);
    return tmp;
  }

  template <template<class> class M1, typename V1,
	    template<class> class M2, typename V2,
	    template<class> class M3, typename V3>
  internal::initializer_< internal::triplet< M1<V1>, M2<V2>, M3<V3> > >
  init(const internal::instant_value_<M1,V1>& v1,
       const internal::instant_value_<M2,V2>& v2,
       const internal::instant_value_<M3,V3>& v3)
  {
    internal::triplet< M1<V1>, M2<V2>, M3<V3> > tmp(v1.value, v2.value, v3.value);
    return tmp;
  }

  // ...



  // Regular version.
  template <typename Target, typename Data>
  bool init(Any<Target>& target, with_t, const Any<Data>& data)
  {
    return init_(exact(&target), exact(data));
  }

  // Assignment.
  template <typename T>
  bool init(Any<T>& target, with_t, const Any<T>& data)
  {
    exact(target) = exact(data);
    return true;
  }

  // Unconst->const data version.
  template <typename Target, typename Data>
  bool init(Any<Target>& target, with_t, Any<Data>& data)
  {
    return init(target, const_cast<const Data&>(data));
  }


  namespace ERROR
  {
    template <typename T>
    struct initialization_of_temporary_or_const_object_;
  }

  // Guard: we cannot have "const Target".
  template <typename Target, typename Data>
  bool init(const Any<Target>&, with_t, const Any<Data>&)
  {
    mlc::abort_< Target, ERROR::initialization_of_temporary_or_const_object_<Target> >::check();
    return false;
  }


# endif // OLN_INCLUDE_ONLY


} // end of namespace oln


#endif // ! OLN_CORE_INIT_HH
