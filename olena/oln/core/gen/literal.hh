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

#ifndef OLN_CORE_GEN_LITERAL_HH
# define OLN_CORE_GEN_LITERAL_HH

# include <oln/core/concept/function.hh>
# include <oln/core/concept/generator.hh>
# include <oln/core/concept/point.hh>
# include <oln/core/concept/value.hh>




namespace oln
{



  // -----------------------------  literal_<T>


  template <typename T>
  struct literal_ : public Generator< literal_<T> >
  {
    typedef const T& result;

    literal_(const T& val) : val_(val) {}

    result operator()() const
    {
      return this->val_;
    }

    result value() const
    {
      return this->val_;
    }

  private:
    T val_;
  };


  // literal
  template <typename T>
  literal_<T>
  literal(const T& val)
  {
    literal_<T> tmp(val);
    return tmp;
  }


  // -----------------------------  lit_p2v_<P,T>


  // Fwd decl.
  template <typename P, typename T> struct lit_p2v_;

  // Category.
  namespace internal
  {
    template <typename P, typename T>
    struct set_category_of_< lit_p2v_<P,T> >
    {
      typedef stc::is< Function_p2v > ret;
    };
  }

  // Class.
  template <typename P, typename T>
  struct lit_p2v_ : public Function_p2v< lit_p2v_<P,T> >
  {
    typedef P argument;
    typedef T result;

    lit_p2v_(const T& val)
      : val_(val)
    {
    }

    result operator()(argument) const
    {
      return this->val_;
    }

  private:
    T val_;
  };


  // -----------------------------  lit_p2b_<P,B>


  // Fwd decl.
  template <typename P, typename B> struct lit_p2b_;

  // Category.
  namespace internal
  {
    template <typename P, typename B>
    struct set_category_of_< lit_p2b_<P,B> >
    {
      typedef stc::is< Function_p2b > ret;
    };
  }

  // Class.
  template <typename P, typename B>
  struct lit_p2b_ : public Function_p2b< lit_p2b_<P,B> >
  {
    typedef P argument;
    typedef B result;

    lit_p2b_(const B& val)
      : val_(val)
    {
    }

    result operator()(argument) const
    {
      return this->val_;
    }

  private:
    B val_;
  };


} // end of namespace oln


#endif // ! OLN_CORE_GEN_LITERAL_HH
