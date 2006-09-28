// Copyright (C) 2002, 2005, 2006  EPITA Research and Development Laboratory
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

#ifndef XTD_ABSTRACT_FUN_HH
# define XTD_ABSTRACT_FUN_HH

# include <xtd/internal/mlc.hh>


namespace xtd
{

  namespace abstract
  {

    /*! \class xtd::abstract::fun_<E>
    **
    ** Abstract base class for xtd functions.  These functions are
    ** pure (non imperative); they can be plain, meta, or expr.
    ** Parameter E is the exact type of the function.
    ** 
    ** \note 1) This base class is useful to check if a type is the
    ** one of an xtd function.  2) As an xtd function is pure, all
    ** imperative operators are disabled.
    **
    ** \see xtd::abstract::plain_fun_<E>, xtd::abstract::meta_fun_<E>,
    ** xtd::abstract::fun_expr_<E>
    */

    template <typename E>
    class fun_
    {
    public:

      /// Static dispatch of this towards its exact type.
      const E& exact() const
      {
	return *(const E*)(const void*)(this);
      }

      /// \{
      /// Imperative operators are disabled.
      template <typename T> void operator %= (const T&) const;
      template <typename T> void operator &= (const T&) const;
      template <typename T> void operator *= (const T&) const;
      template <typename T> void operator += (const T&) const;
      template <typename T> void operator -= (const T&) const;
      template <typename T> void operator /= (const T&) const;
      template <typename T> void operator ^= (const T&) const;
      template <typename T> void operator |= (const T&) const;
      template <typename T> void operator = (const T&);
      void operator ++ () const;
      void operator -- () const;
      void operator ++ (int) const;
      void operator -- (int) const;
      /// \}

      /// \{
      /// Operators "*(x)" and "(x)->" are disabled.
      void operator *  () const;
      void operator -> () const;
      /// \}

      /// Conversion operator is disabled.
      template <typename T> operator T () const;
      
    protected:
      /// Ctor is protected so that this class seems abstract.
      fun_() {}
    };


  } // end of namespace xtd::abstract


} // end of namespace xtd



#endif // ! XTD_ABSTRACT_FUN_HH
