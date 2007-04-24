// Copyright (C) 2007
// EPITA Research and Development Laboratory
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


#ifndef OLN_CORE_CONCEPT_FAST_ITERATOR_HH
# define OLN_CORE_CONCEPT_FAST_ITERATOR_HH

# include <oln/core/concept/iterator.hh>

namespace oln
{

  //  Concept-class "Fast_Iterator".
  template <typename Exact>
  struct Fast_Iterator : public Iterator<Exact>
  {
    stc_typename(value);

    value& operator* ();
    const value& operator*() const;

  protected:
    Fast_Iterator();

  private:
    void check__() const;
  };

# ifndef OLN_INCLUDE_ONLY

  template <typename Exact>
  Fast_Iterator<Exact>::Fast_Iterator()
  {
  }

  template <typename Exact>
  typename Fast_Iterator<Exact>::value&
  Fast_Iterator<Exact>::operator* ()
  {
    return exact(this)->impl_dereference();
  }

  template <typename Exact>
  const typename Fast_Iterator<Exact>::value&
  Fast_Iterator<Exact>::operator* () const
  {
    return exact(this)->impl_dereference();
  }

  template <typename Exact>
  void Fast_Iterator<Exact>::check__() const
  {
    // FIXME
  }

# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln



#endif // ! OLN_CORE_CONCEPT_FAST_ITERATOR_HH
