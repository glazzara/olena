// Copyright (C) 2001, 2003, 2004, 2005, 2006, 2007 EPITA Research and
// Development Laboratory
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

#ifndef OLN_CORE_CONCEPT_ITERATOR_HH
# define OLN_CORE_CONCEPT_ITERATOR_HH

# include <oln/core/equipment.hh>



/// Macro for_all.
# define for_all(i)  for (i.start(); i.is_valid(); i.next())



namespace oln
{

  /// Concept-class "Iterator".

  template <typename Exact>
  struct Iterator : virtual public Any<Exact>
  {
    void start();
    void next();
    void invalidate();
    bool is_valid() const;

  protected:
    Iterator();

  private:
    void check__() const;

  }; // end of class oln::Iterator<Exact>



# ifndef OLN_INCLUDE_ONLY

  template <typename Exact>
  void Iterator<Exact>::start()
  {
    exact(this)->impl_start();
  }

  template <typename Exact>
  void Iterator<Exact>::next()
  {
    precondition(this->is_valid());
    exact(this)->impl_next();
  }

  template <typename Exact>
  void Iterator<Exact>::invalidate()
  {
    exact(this)->impl_invalidate();
  }

  template <typename Exact>
  bool Iterator<Exact>::is_valid() const
  {
    return exact(this)->impl_is_valid();
  }

  template <typename Exact>
  Iterator<Exact>::Iterator()
  {
    this->check__();
  }

  template <typename Exact>
  void Iterator<Exact>::check__() const
  {
    void (Exact::*impl_start_adr)() = & Exact::impl_start;
    impl_start_adr = 0;
    void (Exact::*impl_next_adr)() = & Exact::impl_next;
    impl_next_adr = 0;
    void (Exact::*impl_invalidate_adr)() = & Exact::impl_invalidate;
    impl_invalidate_adr = 0;
    bool (Exact::*impl_is_valid_adr)() const = & Exact::impl_is_valid;
    impl_is_valid_adr = 0;
  }

# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln



#endif // ! OLN_CORE_CONCEPT_ITERATOR_HH
