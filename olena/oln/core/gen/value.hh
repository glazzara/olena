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

#ifndef OLN_CORE_GEN_VALUE_HH
# define OLN_CORE_GEN_VALUE_HH

# include <oln/core/concept/value.hh>


namespace oln
{

  // Fwd decl.
  template <typename T> struct value_;


  // Super trait.
  template <typename T>
  struct super_trait_< value_<T> >
  {
    typedef Value< value_<T> > ret;
  };


  // Virtual types.
  template <typename T>
  struct vtypes< value_<T> >
  {
    typedef stc::is<Value> category; // FIXME: Temporary code.
  };


  // Wrapper class for a value.
  template <typename T>
  struct value_ : public Value< value_<T> >
  {
    typedef T encoding;

    value_();

    template <typename U>
    value_(const U& v);

    template <typename U>
    value_(const value_<U>& v);

    template <typename U>
    value_<T>& operator=(const value_<U>& v);

    operator T() const;
    
    template <typename U>
    operator U() const;

    template <typename U>
    operator value_<U>() const;

  private:
    T val_;
  };


  // make_value

  template <typename T>
  value_<T> make_value(const T& val);

  template <typename T>
  value_<T> make_value(const value_<T>& val);


  // traits

  template <typename L, typename Op, typename R>
  struct set_trait_< Value, value_<L>, Op, Value, value_<R> >
  {
    typedef typename get_trait_<L, Op, R>::ret ret;
  };


# ifndef OLN_INCLUDE_ONLY

  template <typename T>
  value_<T>::value_()
  {
  }

  template <typename T>
  template <typename U>
  value_<T>::value_(const U& v)
    : val_(v)
  {
  }

  template <typename T>
  template <typename U>
  value_<T>::value_(const value_<U>& v)
    : val_(U(v))
  {
  }

  template <typename T>
  template <typename U>
  value_<T>&
  value_<T>::operator=(const value_<U>& v)
  {
    this->val_ = U(v);
    return *this;
  }

  template <typename T>
  value_<T>::operator T() const
  {
    return this->val_;
  }
   
  template <typename T>
  template <typename U>
  value_<T>::operator U() const
  {
    return static_cast<U>(this->val_);
  }

  template <typename T>
  template <typename U>
  value_<T>::operator value_<U>() const
  {
    value_<U> tmp = static_cast<U>(this->val_);
    return tmp;
  }

  template <typename T>
  value_<T> make_value(const T& val)
  {
    value_<T> tmp = val;
    return tmp;
  }

  template <typename T>
  value_<T> make_value(const value_<T>& val)
  {
    return val;
  }

# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln


#endif // ! OLN_CORE_GEN_VALUE_HH
