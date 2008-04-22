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
# include <mln/fun/i2v/all_to.hh>

# include <mln/metal/bool.hh>
# include <mln/algebra/vec.hh>
# include <mln/metal/converts_to.hh>
# include <mln/core/h_vec.hh>


namespace mln
{

  /// \{ Fwd decls.
  template <typename M, typename C> struct dpoint_;
  namespace literal {
    struct zero_t;
    struct one_t;
    struct origin_t;
  }
  /// \}


  namespace internal
  {

    template <typename M, typename C>
    struct point_to_
    {
      typedef algebra::vec<M::dim, C> metal_vec;
      typedef mln::h_vec<M::dim, C> h_vec;
    };

  } // end of namespace mln::internal



  /*! \brief Generic point class.
   *
   * Parameters are \c n the dimension of the space and \c C the
   * coordinate type in this space.
   */
  template <typename M, typename C>
  struct point_ : public Point< point_<M,C> >,
		  public internal::mutable_coord_impl_< M::dim, C, point_<M,C> >
  {
    /*! \var dim
     * \brief Dimension of the space.
     * \invariant dim > 0 
     */
    enum { dim = M::dim };

    /// Mesh associated type.
    typedef M mesh;

    /// Dpoint associated type.
    typedef dpoint_<M,C> dpoint;

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
    point_();

    /// \{ Constructors with different numbers of arguments
    /// (coordinates) w.r.t. the dimension.
    point_(C ind);
    point_(C row, C col);
    point_(C sli, C row, C col);
    /// \}

    /// \{ Constructors/assignments with literals.
    point_(const literal::origin_t&);
    point_<M,C>& operator=(const literal::origin_t&);
    // Works only in 1D:
    point_(const literal::zero_t&);
    point_<M,C>& operator=(const literal::zero_t&);
    point_(const literal::one_t&);
    point_<M,C>& operator=(const literal::one_t&);
    /// \}

    /// Constructor; coordinates are set by function \p f.
    template <typename F>
    point_(const Function_i2v<F>& f);

    /// Set all coordinates to the value \p c.
    void set_all(C c);

    /// Origin point (all coordinates are 0).
    static const point_<M,C> origin;

    /// Shifting by \p dp.
    point_<M,C>& operator+=(const dpoint& dp);

    /// Shifting by \p the inverse of dp.
    point_<M,C>& operator-=(const dpoint& dp);

    /// Type of the array of coordinates.
    typedef algebra::vec<M::dim, C> vec_t;

    /// Hook to coordinates.
    operator typename internal::point_to_<M, C>::metal_vec () const;
    operator algebra::vec<M::dim, float> () const;

    /// Transform to point in homogene coordinate system.
    h_vec<M::dim, C> to_h_vec() const;

  protected:
    algebra::vec<M::dim, C> coord_;
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename M, typename C>
  inline
  C point_<M,C>::operator[](unsigned i) const
  {
    assert(i < dim);
    return this->coord_[i];
  }

  template <typename M, typename C>
  inline
  C& point_<M,C>::operator[](unsigned i)
  {
    assert(i < dim);
    return this->coord_[i];
  }

  // Constructors.

  template <typename M, typename C>
  inline
  point_<M,C>::point_()
  {
  }

  template <typename M, typename C>
  inline
  point_<M,C>::point_(C ind)
  {
    metal::bool_<(dim == 1)>::check();
    coord_[0] = ind;
  }

  template <typename M, typename C>
  inline
  point_<M,C>::point_(C row, C col)
  {
    metal::bool_<(dim == 2)>::check();
    coord_[0] = row;
    coord_[1] = col;
  }

  template <typename M, typename C>
  inline
  point_<M,C>::point_(C sli, C row, C col)
  {
    metal::bool_<(dim == 3)>::check();
    coord_[0] = sli;
    coord_[1] = row;
    coord_[2] = col;
  }

  template <typename M, typename C>
  template <typename F>
  inline
  point_<M,C>::point_(const Function_i2v<F>& f_)
  {
    mlc_converts_to(mln_result(F), C)::check();
    const F& f = exact(f_);
    for (unsigned i = 0; i < dim; ++i)
      coord_[i] = f(i);
  }

  template <typename M, typename C>
  inline
  point_<M,C>::point_(const literal::origin_t&)
  {
    coord_.set_all(0);
  }

  template <typename M, typename C>
  inline
  point_<M,C>&
  point_<M,C>::operator=(const literal::origin_t&)
  {
    coord_.set_all(0);
    return *this;
  }

  template <typename M, typename C>
  inline
  point_<M,C>::point_(const literal::zero_t&)
  {
    metal::bool_<(dim == 1)>::check();
    coord_[0] = 1;
  }

  template <typename M, typename C>
  inline
  point_<M,C>&
  point_<M,C>::operator=(const literal::zero_t&)
  {
    metal::bool_<(dim == 1)>::check();
    coord_[0] = 1;
    return *this;
  }

  template <typename M, typename C>
  inline
  point_<M,C>::point_(const literal::one_t&)
  {
    metal::bool_<(dim == 1)>::check();
    coord_[0] = 1;
  }

  template <typename M, typename C>
  inline
  point_<M,C>&
  point_<M,C>::operator=(const literal::one_t&)
  {
    metal::bool_<(dim == 1)>::check();
    coord_[0] = 1;
    return *this;
  }

  template <typename M, typename C>
  inline
  void point_<M,C>::set_all(C c)
  {
    coord_.set_all(c);
  }

  template <typename M, typename C>
  const point_<M,C> point_<M,C>::origin = all_to(0);

  template <typename M, typename C>
  inline
  point_<M,C>&
  point_<M,C>::operator+=(const dpoint& dp)
  {
    for (unsigned i = 0; i < dim; ++i)
      coord_[i] += dp[i];
    return *this;
  }

  template <typename M, typename C>
  inline
  point_<M,C>&
  point_<M,C>::operator-=(const dpoint& dp)
  {
    for (unsigned i = 0; i < dim; ++i)
      coord_[i] -= dp[i];
    return *this;
  }

  template <typename M, typename C>
  inline
  point_<M,C>::operator typename internal::point_to_<M, C>::metal_vec () const
  {
    return coord_; // FIXME: Is-it OK?
  }
  
  template <typename M, typename C>
  inline
  point_<M,C>::operator algebra::vec<M::dim, float> () const
  {
    algebra::vec<dim, float> tmp;
    for (unsigned i = 0; i < dim; ++i)
      tmp[i] = coord_[i];
    return tmp;
  }
  
  template <typename M, typename C>
  inline
  h_vec<M::dim, C> point_<M,C>::to_h_vec() const
  {
    h_vec<M::dim, C> tmp;
    for (unsigned i = 0; i < dim; ++i)
      tmp[i] = coord_[i];
    tmp[M::dim] = 1;
    return tmp;
  }

# endif // ! MLN_INCLUDE_ONLY
  
} // end of namespace mln


#endif // ! MLN_CORE_POINT_HH
