// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CORE_DPOINT_HH
# define MLN_CORE_DPOINT_HH

/*! \file mln/core/dpoint.hh
 *
 * \brief Definition of the generic delta-point class mln::dpoint_.
 */

# include <mln/core/concept/dpoint.hh>
# include <mln/core/internal/coord_impl.hh>
# include <mln/fun/i2v/all.hh>
# include <mln/algebra/vec.hh>
# include <mln/metal/converts_to.hh>

namespace mln
{

  /// \{ Fwd decls.
  template <typename M, typename C> struct point_;
  namespace literal {
    struct zero_t;
    struct one_t;
  }
  /// \}


  /*! \brief Generic delta-point class.
   *
   * Parameters are \c M the dimension of the space and \c C the
   * coordinate type in this space.
   */
  template <typename M, typename C>
  struct dpoint_ : public Dpoint< dpoint_<M,C> >,
		   public internal::mutable_coord_impl_< M::dim, C, dpoint_<M,C> >
  {
    /*! \var dim
     * \brief Dimension of the space.
     * \invariant dim > 0
     */
    enum { dim = M::dim };

    /// Mesh associated type.
    typedef M mesh;

    /// Point associated type.
    typedef point_<M,C> point;

    /// Coordinate associated type.
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

    /// Constructor without argument.
    dpoint_();

    /// \{ Constructors with different numbers of arguments
    /// (coordinates) w.r.t. the dimension.
    dpoint_(C ind);
    dpoint_(C row, C col);
    dpoint_(C sli, C row, C col);
    /// \}

    /// \{ Constructors/assignments with literals.
    dpoint_(const literal::zero_t&);
    dpoint_<M,C>& operator=(const literal::zero_t&);
    // Works only in 1D:
    dpoint_(const literal::one_t&);
    dpoint_<M,C>& operator=(const literal::one_t&);
    /// \}

    /// Constructor; coordinates are set by function \p f.
    template <typename F>
    dpoint_(const Function_i2v<F>& f);

    /// Set all coordinates to the value \p c.
    void set_all(C c);

    /// Conversion towards a algebra::vec.
    template <typename Q>
    operator mln::algebra::vec<M::dim, Q>() const;

    /// Explicit conversion.
    mln::algebra::vec<M::dim, C> to_vec() const;

  protected:
    mln::algebra::vec<M::dim, C> coord_;
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename M, typename C>
  inline
  C dpoint_<M,C>::operator[](unsigned i) const
  {
    assert(i < dim);
    return coord_[i];
  }

  template <typename M, typename C>
  inline
  C& dpoint_<M,C>::operator[](unsigned i)
  {
    assert(i < dim);
    return coord_[i];
  }

  template <typename M, typename C>
  inline
  dpoint_<M,C>::dpoint_()
  {
  }

  template <typename M, typename C>
  inline
  dpoint_<M,C>::dpoint_(C ind)
  {
    metal::bool_<(dim == 1)>::check();
    coord_[0] = ind;
  }

  template <typename M, typename C>
  inline
  dpoint_<M,C>::dpoint_(C row, C col)
  {
    metal::bool_<(dim == 2)>::check();
    coord_[0] = row;
    coord_[1] = col;
  }

  template <typename M, typename C>
  inline
  dpoint_<M,C>::dpoint_(C sli, C row, C col)
  {
    metal::bool_<(dim == 3)>::check();
    coord_[0] = sli;
    coord_[1] = row;
    coord_[2] = col;
  }

  template <typename M, typename C>
  inline
  dpoint_<M,C>::dpoint_(const literal::zero_t&)
  {
    coord_.set_all(0);
  }

  template <typename M, typename C>
  inline
  dpoint_<M,C>&
  dpoint_<M,C>::operator=(const literal::zero_t&)
  {
    coord_.set_all(0);
    return *this;
  }

  template <typename M, typename C>
  inline
  dpoint_<M,C>::dpoint_(const literal::one_t&)
  {
    metal::bool_<(dim == 1)>::check();
    coord_[0] = 1;
  }

  template <typename M, typename C>
  inline
  dpoint_<M,C>&
  dpoint_<M,C>::operator=(const literal::one_t&)
  {
    metal::bool_<(dim == 1)>::check();
    coord_[0] = 1;
    return *this;
  }

  template <typename M, typename C>
  template <typename F>
  inline
  dpoint_<M,C>::dpoint_(const Function_i2v<F>& f_)
  {
    mlc_converts_to(mln_result(F), C)::check();
    const F& f = exact(f_);
    for (unsigned i = 0; i < dim; ++i)
      coord_[i] = f(i);
  }

  template <typename M, typename C>
  inline
  void dpoint_<M,C>::set_all(C c)
  {
    for (unsigned i = 0; i < dim; ++i)
      coord_[i] = c;
  }

  template <typename M, typename C>
  template <typename Q>
  inline
  dpoint_<M,C>::operator mln::algebra::vec<M::dim, Q> () const
  {
    return coord_;
  }

  template <typename M, typename C>
  inline
  mln::algebra::vec<M::dim, C>
  dpoint_<M,C>::to_vec() const
  {
    return coord_;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_DPOINT_HH
