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

#ifndef OLN_CORE_GEN_CONSTANT_HH
# define OLN_CORE_GEN_CONSTANT_HH

# include <oln/core/concept/generator.hh>
# include <oln/core/concept/point.hh>
# include <oln/core/gen/value.hh>


namespace oln
{

  // constant_p2v_<P,T>

  template <typename P, typename V>
  struct constant_p2v_ : public Function_p2v< constant_p2v_<P,V> >
  {
    typedef P argument;
    typedef const V& result;

    constant_p2v_(const V& val);

    const V& operator()(const P&) const;

  private:
    V val_;
  };


  // constant_p2b_<P,B>

  template <typename P, typename B>
  struct constant_p2b_ : public Function_p2b< constant_p2b_<P,B> >
  {
    typedef P argument;
    typedef const B& result;

    constant_p2b_(const B& val);

    const B& operator()(const P&) const;

  private:
    B val_;
  };




# ifndef OLN_INCLUDE_ONLY

  // constant_p2v_<P,V>

  template <typename P, typename V>
  constant_p2v_<P,V>::constant_p2v_(const V& val)
    : val_(val)
  {
  }

  template <typename P, typename V>
  const V&
  constant_p2v_<P,V>::operator()(const P&) const
  {
    return this->val_;
  }


  //constant_p2b_<P,B>

  template <typename P, typename B>
  constant_p2b_<P,B>::constant_p2b_(const B& val)
    : val_(val)
  {
  }

  template <typename P, typename B>
  const B&
  constant_p2b_<P,B>::operator()(const P&) const
  {
    return this->val_;
  }

# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln


#endif // ! OLN_CORE_GEN_CONSTANT_HH
