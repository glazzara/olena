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

#ifndef MLN_CORE_CONCEPT_WEIGHTED_WINDOW_HH
# define MLN_CORE_CONCEPT_WEIGHTED_WINDOW_HH

/*! \file mln/core/concept/weighted_window.hh
 *
 * \brief Definition of the concept of mln::Weighted_Window.
 */

# include <mln/core/concept/object.hh>
# include <mln/core/concept/iterator.hh>


namespace mln
{

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

      typedef point;
      typedef dpoint;
      typedef weight;
      typedef window;

      E& sym();
    */

    /// Test if the weighted window is empty; final method.
    bool is_empty() const
    {
      return exact(this)->win().is_empty();
    }

    /// Test if the weighted window is centered; final method.
    bool is_centered() const
    {
      return exact(this)->win().is_centered();
    }

    // FIXME: Remove because too ambiguous: bool is_symmetric() const
    
    /// Give the maximum coordinate gap.
    unsigned delta() const
    {
      return exact(this)->win().delta();
    }
    
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
  Weighted_Window<E>::Weighted_Window()
  {
    typedef  mln_point(E)  point;
    typedef mln_dpoint(E) dpoint;
    typedef mln_weight(E) weight;
    typedef mln_window(E) window;

    typedef mln_fwd_qiter(E) fwd_qiter;
    typedef mln_bkd_qiter(E) bkd_qiter;

    E& (E::*m1)() = & E::sym;
    m1 = 0;

    const window& (E::*m2)() const = & E::win;
    m2 = 0;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_WEIGHTED_WINDOW_HH
