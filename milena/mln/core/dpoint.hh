// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CORE_DPOINT_HH
# define MLN_CORE_DPOINT_HH

/// \file
///
/// \brief Definition of the generic delta-point class mln::dpoint.

# include <mln/core/def/coord.hh>
# include <mln/core/concept/gdpoint.hh>
# include <mln/core/internal/coord_impl.hh>
# include <mln/algebra/vec.hh>
# include <mln/metal/converts_to.hh>


namespace mln
{

  /// \{ Forward declarations.
  template <typename G, typename C> struct point;
  namespace literal {
    struct zero_t;
    struct one_t;
  }
  /// \}


  /// \brief Generic delta-point class.
  ///
  /// Parameters are \c G the dimension of the space and \c C the
  /// coordinate type in this space.
  template <typename G, typename C>
  struct dpoint : public Gdpoint< dpoint<G,C> >,
		  public internal::mutable_coord_impl_< G::dim, C, dpoint<G,C> >
  {
    /// \var dim
    /// Dimension of the space.
    /// \invariant dim > 0
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

    /// Read-only access to the \p i-th coordinate value.
    /// \param[in] i The coordinate index.
    /// \pre \p i < \c dim
    C  operator[](unsigned i) const;

    /// Read-write access to the \p i-th coordinate value.
    /// \param[in] i The coordinate index.
    /// \pre \p i < \c dim
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
    dpoint(const Function_v2v<F>& f);

    /// Set all coordinates to the value \p c.
    void set_all(C c);

    /// Conversion towards a algebra::vec.
    template <typename Q>
    operator mln::algebra::vec<dpoint<G,C>::dim, Q>() const;

    /// Explicit conversion.
    vec to_vec() const;

  protected:
    mln::algebra::vec<G::dim, C> coord_;
  };


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
    // FIXME: to be improved.
    if (dim < 3)
      coord_ = v;
    else
    {
      unsigned j = 0;
      for (unsigned i = dim - 2; i < dim; ++i)
	coord_[i] = static_cast<C>(v[j++]);
      for (unsigned i = 2; i < dim; ++i, ++j)
	coord_[i-j] = static_cast<C>(v[j]);
    }
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
  dpoint<G,C>::dpoint(const Function_v2v<F>& f_)
  {
    mlc_converts_to(mln_result(F), C)::check();
    const F& f = exact(f_);
    for (unsigned i = 0; i < dim; ++i)
      coord_[i] = static_cast<C>(f(i));
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
  dpoint<G,C>::operator mln::algebra::vec<dpoint<G,C>::dim, Q> () const
  {
    return to_vec();
  }

  template <typename G, typename C>
  inline
  typename dpoint<G,C>::vec
  dpoint<G,C>::to_vec() const
  {
    algebra::vec<G::dim, float> tmp;

    // FIXME: to be improved.
    if (dim == 1)
      tmp[0] = coord_[0];
    else
    {
      unsigned j = 0;
      for (unsigned i = dim - 2; i < dim; ++i)
	tmp[j++] = coord_[i];
      for (unsigned i = 2; i < dim; ++i, ++j)
	tmp[j] = coord_[i-j];
    }

    return tmp;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_DPOINT_HH
