// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_CORE_CONCEPT_SITE_ITERATOR_HH
# define MLN_CORE_CONCEPT_SITE_ITERATOR_HH

/// \file
///
/// Definition of the concept of mln::Site_Iterator.
///
/// \todo Fix mln/core/concept/doc/site_iterator.hh


# include <mln/core/concept/site_proxy.hh>
# include <mln/core/concept/iterator.hh> // To fetch the macros.


namespace mln
{

  /// Base class for implementation of classes of iterator on
  ///  points.
  ///
  /// An iterator on points is an iterator that browse over a set of
  /// points.
  ///
  /// \see mln::doc::Site_Iterator for a complete documentation of this class
  /// contents.
  ///
  template <typename E>
  struct Site_Iterator : public Site_Proxy<E>
  {
    /*
      bool is_valid_() const;
      void invalidate_();
      void start_();
      void next_();
      const ..& target_() const;
    */

    /// Go to the next element.
    ///
    /// \warning This is a final method; iterator classes should not
    /// re-defined this method.  The actual "next" operation has to be
    /// defined through the \em next_ method.
    ///
    /// \pre The iterator is valid.
    ///
    void next(); // final

    // FIXME: Doc!!!
    bool is_valid() const;
    void invalidate();
    void start();

    // Defined in site_iterator_base:
    // void change_target(s);

  protected:
    Site_Iterator();
  };



# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  inline
  void
  Site_Iterator<E>::next()
  {
    mln_precondition(is_valid());
    exact(this)->next_();
  }

  template <typename E>
  inline
  bool
  Site_Iterator<E>::is_valid() const
  {
    E *const this_ = const_cast<E*const>(exact(this)); // Unconst.
    if (this_->target_() == 0)
      return false;
    return exact(this)->is_valid_();
  }

  template <typename E>
  inline
  void
  Site_Iterator<E>::invalidate()
  {
    if (exact(this)->target_() == 0)
      return; // No-op.
    exact(this)->invalidate_();
    mln_postcondition(is_valid() == false);
  }

  template <typename E>
  inline
  void
  Site_Iterator<E>::start()
  {
    mln_precondition(exact(this)->target_() != 0);
    exact(this)->start_();
  }

  template <typename E>
  inline
  Site_Iterator<E>::Site_Iterator()
  {
    bool m0 = (& E::target_) == (& E::target_); // FIXME: Find a better test.
    m0 = 0;
    bool (E::*m1)() const = & E::is_valid_;
    m1 = 0;
    void (E::*m2)() = & E::invalidate_;
    m2 = 0;
    void (E::*m3)() = & E::start_;
    m3 = 0;
    void (E::*m4)() = & E::next_;
    m4 = 0;
    bool m5 = (& E::change_target) == (& E::change_target);
    m5 = 0;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_SITE_ITERATOR_HH
