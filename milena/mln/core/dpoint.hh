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

#ifndef MLN_CORE_DPOINT_HH
# define MLN_CORE_DPOINT_HH

/*! \file mln/core/dpoint.hh
 *
 * \brief Definition of the generic delta-point class mln::dpoint.
 */

# include <mln/core/def/coord.hh>
# include <mln/core/concept/gdpoint.hh>
# include <mln/core/internal/coord_impl.hh>
# include <mln/fun/i2v/all.hh>
# include <mln/algebra/vec.hh>
# include <mln/metal/converts_to.hh>

namespace mln
{

  /// \{ Fwd decls.
  template <typename G, typename C> struct point;
  namespace literal {
    struct zero_t;
    struct one_t;
  }
  /// \}


  /*! \brief Generic delta-point class.
   *
   * Parameters are \c G the dimension of the space and \c C the
   * coordinate type in this space.
   */
  template <typename G, typename C>
  struct dpoint : public Gdpoint< dpoint<G,C> >,
		  public internal::mutable_coord_impl_< G::dim, C, dpoint<G,C> >
  {
    /*! \var dim
     * \brief Dimension of the space.
     * \invariant dim > 0
     */
    enum { dim = G::dim };

    /// Grid associated type.
    typedef G grid;

    /// Psite associated type.
    typedef point<G,C> psite;

    /// Site associated type.
    typedef point<G,C> site;

    /// Coordinate associated type.
    typedef C coord;

    /// Algebra vector (vec) associated type.
    typedef algebra::vec<G::dim, C> vec;

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
    dpoint();

    /// Constructor from an algebra vector.
    template <typename C2>
    dpoint(const algebra::vec<dim,C2>& v);

    /// \{ Constructors with different numbers of arguments
    /// (coordinates) w.r.t. the dimension.
    dpoint(C ind);
    dpoint(C row, C col);
    dpoint(C sli, C row, C col);
    /// \}

    /// \{ Constructors/assignments with literals.
    dpoint(const literal::zero_t&);
    dpoint<G,C>& operator=(const literal::zero_t&);
    // Works only in 1D:
    dpoint(const literal::one_t&);
    dpoint<G,C>& operator=(const literal::one_t&);
    /// \}

    /// Constructor; coordinates are set by function \p f.
    template <typename F>
    dpoint(const Function_i2v<F>& f);

    /// Set all coordinates to the value \p c.
    void set_all(C c);

    /// Conversion towards a algebra::vec.
    template <typename Q>
    operator algebra::vec<G::dim, Q>() const;

    /// Explicit conversion.
    const algebra::vec<G::dim, C>& to_vec() const;

  protected:
    algebra::vec<G::dim, C> coord_;
  };


  namespace util
  {

    /*! \brief Ordering "less than" comparison between a couple of
     *  delta-points.
     *
     * The ordering is based on a lexicographical ordering over
     * coordinates.
     *
     * Both delta-points have to be defined on the same topology.
     */
    template <typename G, typename Cl, typename Cr>
    struct less_than< dpoint<G,Cl>,
		      dpoint<G,Cr> > 
    {
      /*! \brief Comparison between a couple of delta-points \a lhs
       *  and \a rhs.
       *
       * \return True if \p lhs is before \p rhs in the sense of the
       * coordinates lexicographic comparison, otherwise false.
       */
      bool operator()(const dpoint<G,Cl>& lhs,
		      const dpoint<G,Cr>& rhs) const;
    };

  } // end of namespace mln::util


# ifndef MLN_INCLUDE_ONLY

  template <typename G, typename C>
  inline
  C dpoint<G,C>::operator[](unsigned i) const
  {
    assert(i < dim);
    return coord_[i];
  }

  template <typename G, typename C>
  inline
  C& dpoint<G,C>::operator[](unsigned i)
  {
    assert(i < dim);
    return coord_[i];
  }

  template <typename G, typename C>
  inline
  dpoint<G,C>::dpoint()
  {
  }

  template <typename G, typename C>
  template <typename C2>
  inline
  dpoint<G,C>::dpoint(const algebra::vec<dim,C2>& v)
  {
    coord_ = v;
  }

  template <typename G, typename C>
  inline
  dpoint<G,C>::dpoint(C ind)
  {
    metal::bool_<(dim == 1)>::check();
    coord_[0] = ind;
  }

  template <typename G, typename C>
  inline
  dpoint<G,C>::dpoint(C row, C col)
  {
    metal::bool_<(dim == 2)>::check();
    coord_[0] = row;
    coord_[1] = col;
  }

  template <typename G, typename C>
  inline
  dpoint<G,C>::dpoint(C sli, C row, C col)
  {
    metal::bool_<(dim == 3)>::check();
    coord_[0] = sli;
    coord_[1] = row;
    coord_[2] = col;
  }

  template <typename G, typename C>
  inline
  dpoint<G,C>::dpoint(const literal::zero_t&)
  {
    coord_.set_all(0);
  }

  template <typename G, typename C>
  inline
  dpoint<G,C>&
  dpoint<G,C>::operator=(const literal::zero_t&)
  {
    coord_.set_all(0);
    return *this;
  }

  template <typename G, typename C>
  inline
  dpoint<G,C>::dpoint(const literal::one_t&)
  {
    metal::bool_<(dim == 1)>::check();
    coord_[0] = 1;
  }

  template <typename G, typename C>
  inline
  dpoint<G,C>&
  dpoint<G,C>::operator=(const literal::one_t&)
  {
    metal::bool_<(dim == 1)>::check();
    coord_[0] = 1;
    return *this;
  }

  template <typename G, typename C>
  template <typename F>
  inline
  dpoint<G,C>::dpoint(const Function_i2v<F>& f_)
  {
    mlc_converts_to(mln_result(F), C)::check();
    const F& f = exact(f_);
    for (unsigned i = 0; i < dim; ++i)
      coord_[i] = f(i);
  }

  template <typename G, typename C>
  inline
  void dpoint<G,C>::set_all(C c)
  {
    for (unsigned i = 0; i < dim; ++i)
      coord_[i] = c;
  }

  template <typename G, typename C>
  template <typename Q>
  inline
  dpoint<G,C>::operator algebra::vec<G::dim, Q> () const
  {
    return coord_;
  }

  template <typename G, typename C>
  inline
  const algebra::vec<G::dim, C>&
  dpoint<G,C>::to_vec() const
  {
    return coord_;
  }

  namespace util
  {

    template <typename G, typename Cl, typename Cr>
    bool
    less_than< dpoint<G,Cl>,
	       dpoint<G,Cr> >::operator()(const dpoint<G,Cl>& lhs,
					  const dpoint<G,Cr>& rhs) const
    {
      enum { n = G::dim };
      typedef less_than< algebra::vec<n,Cl>, algebra::vec<n,Cr> > less_t;
      static const less_t op = less_t();
      return op(lhs.to_vec(), rhs.to_vec());
    }

  } // end of namespace mln::util

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_DPOINT_HH
