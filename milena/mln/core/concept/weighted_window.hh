// Copyright (C) 2007, 2008, 2009, 2011 EPITA Research and Development
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

#ifndef MLN_CORE_CONCEPT_WEIGHTED_WINDOW_HH
# define MLN_CORE_CONCEPT_WEIGHTED_WINDOW_HH

/// \file
///
/// Definition of the concept of mln::Weighted_Window.
///
/// \todo Make ::sym() be optional.

# include <mln/core/concept/object.hh>
# include <mln/core/concept/iterator.hh>
# include <mln/trait/windows.hh>



# define mln_is_simple_weighted_window(W)  \
  mln_is_simple_window(typename W::window)



namespace mln
{

  // Forward declaration.
  template <typename E> struct Weighted_Window;


  // Weighted_Window category flag type.
  template <>
  struct Weighted_Window<void>
  {
    typedef Object<void> super;
  };


  /*! \brief Base class for implementation classes that are
   *  weighted_windows.
   *
   * \see mln::doc::Weighted_Window for a complete documentation of
   * this class contents.
   */
  template <typename E>
  struct Weighted_Window : public Object<E>
  {
    typedef Weighted_Window<void> category;

    /*
      typedef fwd_qiter;
      typedef bkd_piter;

      typedef dpsite;
      typedef psite;
      typedef site;
      typedef weight;
      typedef window;

      void sym();
    */
    
  protected:
    Weighted_Window();
  };


  /*! \brief Compute the symmetrical weighted window of \p rhs.
   *
   * \relates mln::Weighted_Window
   */
  template <typename W>
  W operator-(const Weighted_Window<W>& rhs);



# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  inline
  Weighted_Window<E>::Weighted_Window()
  {
    typedef   mln_site(E)   site;
    typedef  mln_psite(E)  psite;
    typedef mln_dpsite(E) dpsite;

    typedef mln_weight(E) weight;
    typedef mln_window(E) window;

    typedef mln_fwd_qiter(E) fwd_qiter;
    typedef mln_bkd_qiter(E) bkd_qiter;

    void (E::*m1)() = & E::sym;
    (void) m1;

    const window& (E::*m2)() const = & E::win;
    (void) m2;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_WEIGHTED_WINDOW_HH
