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

#ifndef OLN_CORE_CONCEPT_LITERAL_HH
# define OLN_CORE_CONCEPT_LITERAL_HH

# include <oln/core/concept/operators.hh>


namespace oln
{

  /// Concept-class "Literal".

  template <typename Exact>
  struct Literal : public Any<Exact>
  {
    typedef Literal<void> category;

    template <typename T>
    operator T() const; // NEW

  protected:
    Literal();
  };


  // Default.

  template <typename L, typename R>
  oln_minus_trait(R, R)
  operator - (const Literal<L>& lhs, const Any<R>& rhs);

  template <typename L, typename R>
  oln_plus_trait(R, R)
  operator + (const Literal<L>& lhs, const Any<R>& rhs);



# ifndef OLN_INCLUDE_ONLY

  template <typename Exact>
  Literal<Exact>::Literal()
  {
  }

  template <typename Exact>
  template <typename T>
  Literal<Exact>::operator T() const
  {
    T tmp;
    assignment_(tmp, exact(*this));
    return tmp;
  }

  template <typename L, typename R>
  oln_minus_trait(R, R)
  operator - (const Literal<L>& lhs, const Any<R>& rhs)
  {
    oln_minus_trait(R, R) tmp = exact(lhs).operator R() - exact(rhs);
    return tmp;
  }

  template <typename L, typename R>
  oln_plus_trait(R, R)
  operator + (const Literal<L>& lhs, const Any<R>& rhs)
  {
    oln_plus_trait(R, R) tmp = exact(lhs).operator R() + exact(rhs);
    return tmp;
  }

# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln


#endif // ! OLN_CORE_CONCEPT_LITERAL_HH
