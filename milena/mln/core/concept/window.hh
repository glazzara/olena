// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_CONCEPT_WINDOW_HH
# define MLN_CORE_CONCEPT_WINDOW_HH

/*! \file mln/core/concept/window.hh
 * \brief Definition of the concept of mln::Window.
 *
 * \todo Operator== should test if the cmp is possible.
 */

# include <mln/core/concept/object.hh>
# include <mln/core/concept/iterator.hh>
# include <mln/core/site_set/p_array.hh>


namespace mln
{

  // Fwd decl.
  template <typename E> struct Window;

  // Window category flag type.
  template <>
  struct Window<void>
  {
    typedef Object<void> super;
  };


  /*! \brief Base class for implementation classes that are windows.
   *
   * \see mln::doc::Window for a complete documentation of this class
   * contents.
   */
  template <typename E>
  struct Window : public Object<E>
  {
    typedef Window<void> category;

    /*
      typedef   site;
      typedef  psite;
      typedef dpsite;

      typedef     qiter;
      typedef fwd_qiter;
      typedef bkd_qiter;
    */

  protected:
    Window();
  };


  template <typename Wl, typename Wr>
  bool operator==(const Window<Wl>& lhs, const Window<Wr>& rhs)
  {
    return exact(lhs).std_vector() == exact(rhs).std_vector();
  }


# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  inline
  Window<E>::Window()
  {
    typedef   mln_site(E)   site;
    typedef  mln_psite(E)  psite;
    typedef mln_dpsite(E) dpsite;

    typedef     mln_qiter(E)     qiter;
    typedef mln_fwd_qiter(E) fwd_qiter;
    typedef mln_bkd_qiter(E) bkd_qiter;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_WINDOW_HH
