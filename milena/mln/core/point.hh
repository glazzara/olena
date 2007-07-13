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

#ifndef MLN_CORE_POINT_HH
# define MLN_CORE_POINT_HH

/*! \file mln/core/point.hh
 *
 * \brief Definition of the generic point class mln::point_.
 */

# include <mln/core/concept/point.hh>
# include <mln/core/internal/coord_impl.hh>
# include <mln/fun/all.hh>


namespace mln
{

  // fwd decl
  template <unsigned n, typename C> struct dpoint_;


  /*! \brief Generic point class.
   *
   * Parameters are \c n the dimension of the space and \c C the
   * coordinate type in this space.
   */
  template <unsigned n, typename C>
  struct point_ : public Point< point_<n,C> >,
		  public internal::mutable_coord_impl_< n, C, point_<n,C> >
  {
    /*! \var dim
     * \brief Dimension of the space.
     * \invariant dim > 0 
     */
    enum { dim = n };

    /*! \brief Dpoint associated type.
     */
    typedef dpoint_<n,C> dpoint;

    /*! \brief Coordinate associated type.
     */
    typedef C coord;

    /*! \brief Read-only access to the \p i-th coordinate value.
     * \param[in] i The coordinate index.
     * \pre \p i < \c dim
     */
    C  operator[](unsigned i) const;

    /*! \brief Read-write access to the \p i-th coordinate value.
     * \param[in] i The coordinate index.
     * \pre \p i < \c dim
     */
    C& operator[](unsigned i);

    /*! \brief Constructor without argument.
     */
    point_();

    /*! \brief Constructor; coordinates are set by function \p f.
     */
    template <typename F>
    point_(const Function<F>& f);

    /*! \brief Set all coordinates to the value \p c.
     */
    void set_all(C c);

    /// Origin point (all coordinates are 0).
    static const point_<n,C> zero;

  protected:
    C coord_[n];
  };


# ifndef MLN_INCLUDE_ONLY

  template <unsigned n, typename C>
  C point_<n,C>::operator[](unsigned i) const
  {
    assert(i < n);
    return this->coord_[i];
  }

  template <unsigned n, typename C>
  C& point_<n,C>::operator[](unsigned i)
  {
    assert(i < n);
    return this->coord_[i];
  }

  template <unsigned n, typename C>
  point_<n,C>::point_()
  {
  }

  template <unsigned n, typename C>
  template <typename F>
  point_<n,C>::point_(const Function<F>& f_)
  {
    const F& f = exact(f_);
    for (unsigned i = 0; i < n; ++i)
      coord_[i] = f(i);
  }

  template <unsigned n, typename C>
  void point_<n,C>::set_all(C c)
  {
    for (unsigned i = 0; i < n; ++i)
      coord_[i] = c;
  }

  template <unsigned n, typename C>
  const point_<n,C> point_<n,C>::zero = all(0);

# endif // ! MLN_INCLUDE_ONLY
  
} // end of namespace mln


#endif // ! MLN_CORE_POINT_HH
