// Copyright (C) 2007, 2009, 2013 EPITA Research and Development
// Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_CORE_CONCEPT_BROWSING_HH
# define MLN_CORE_CONCEPT_BROWSING_HH

/*! \file
 * \brief Definition of the concept of mln::Browsing.
 */

# include <mln/core/concept/object.hh>


namespace mln
{

  // Fwd decl.
  template <typename E> struct Browsing;

  /// \cond INTERNAL_API
  /// Browsing category flag type.
  template <>
  struct Browsing<void>
  {
    typedef Object<void> super;
  };
  /// \endcond


  /*!
    \brief Base class for implementation classes that are browsings.
    \ingroup modconcepts
   */
  template <typename E>
  struct Browsing : public Object<E>
  {
    typedef Browsing<void> category;

    void init();
    void final();

  protected:
    Browsing();
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  inline
  Browsing<E>::Browsing()
  {
  }

  template <typename E>
  inline
  void Browsing<E>::init()
  {
  }

  template <typename E>
  inline
  void Browsing<E>::final()
  {
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_BROWSING_HH
