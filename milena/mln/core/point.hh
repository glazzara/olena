// Copyright (C) 2007, 2008, 2009, 2010 EPITA Research and Development
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

#ifndef MLN_CORE_POINT_HH
# define MLN_CORE_POINT_HH

/// \file
///
/// \brief Definition of the generic point class mln::point.
///
/// \todo the structure subject_point_impl is useless: first it is
/// only used for 3D points (and it can be usefull for other points);
/// second there is a room for the couple of methods (in the
/// subject_impl specializations defined in core/alias/point*d.hh).

# include <cmath>
# include <mln/core/def/coord.hh>
# include <mln/core/concept/proxy.hh>
# include <mln/core/concept/gpoint.hh>
# include <mln/core/internal/coord_impl.hh>
# include <mln/fun/i2v/all_to.hh>

# include <mln/metal/bool.hh>
# include <mln/metal/is_not.hh>
# include <mln/algebra/vec.hh>
# include <mln/metal/converts_to.hh>
# include <mln/algebra/h_vec.hh>
# include <mln/util/yes.hh>


namespace mln
{

  /// \{ Forward declarations.
  template <typename G, typename C> struct  point;
  template <typename G, typename C> struct dpoint;
  namespace literal {
    struct zero_t;
    struct one_t;
    struct origin_t;
  }
  /// \}


  namespace convert
  {

    namespace over_load
    {

      template <typename G, typename C1, typename C2>
      void from_to_(const point<G,C1>& from, point<G,C2>& to);

      template <unsigned n, typename C1, typename G, typename C2>
      void
      from_to_(const mln::algebra::vec<n,C1>& from, point<G,C2>& to);

      template <unsigned n, typename C1, typename G>
      void
      from_to_(const mln::algebra::vec<n,C1>& from, point<G,C1>& to);

    } // end of namespace mln::convert::over_load

  } // end of namespace mln::convert



  namespace internal
  {
    // This is a hack so that g++-2.95 can compile the method
    // "point<G,C>::operator vec() const".
    template <typename G, typename C>
    struct vec_of_point
    {
      typedef mln::algebra::vec<G::dim, float> ret;
    };

  }

  /// Generic point class.
  ///
  /// Parameters are \c n the dimension of the space and \c C the
  /// coordinate type in this space.
  template <typename G, typename C>
  struct point : public Gpoint< point<G,C> >,
		 public internal::mutable_coord_impl_< G::dim, C, point<G,C> >
  {
    // FIXME: Temporary hack.
    typedef point site;
    typedef point psite;


    /// \var dim
    /// Dimension of the space.
    /// \invariant dim > 0
    enum { dim = G::dim };

    /// Grid associated type.
    typedef G grid;

    /// Delta associated type.
    typedef dpoint<G,C> delta;

    /// DPsite associated type.
    typedef dpoint<G,C> dpsite;

    /// Coordinate associated type.
    typedef C coord;

    /// Algebra vector (vec) associated type.
    typedef mln::algebra::vec<G::dim, float> vec;

    /// Algebra hexagonal vector (hvec) associated type.
    typedef mln::algebra::h_vec<G::dim, float> h_vec;

    /// Read-only access to the \p i-th coordinate value.
    /// \param[in] i The coordinate index.
    /// \pre \p i < \c dim
    const C& operator[](unsigned i) const;

    /// Read-write access to the \p i-th coordinate value.
    /// \param[in] i The coordinate index.
    /// \pre \p i < \c dim
    C& operator[](unsigned i);


    /// Read-only access to the last coordinate.
    const C& last_coord() const;

    /// Read-write access to the last coordinate.
    C& last_coord();


    /// Constructor without argument.
    point();

    /// Constructor from an algebra vector.
    template <typename C2>
    point(const mln::algebra::vec<dim,C2>& v);

    point(const mln::algebra::vec<dim,C>& v);


    /// \{ Constructors with different numbers of arguments
    /// (coordinates) w.r.t. the dimension.
    explicit point(C ind);
    point(C row, C col);
    point(C sli, C row, C col);
    /// \}

    /// \{ Constructors/assignments with literals.
    point(const literal::origin_t&);
    point<G,C>& operator=(const literal::origin_t&);
    // Works only in 1D:
    point(const literal::zero_t&);
    point<G,C>& operator=(const literal::zero_t&);
    point(const literal::one_t&);
    point<G,C>& operator=(const literal::one_t&);
    /// \}

    /// Constructor; coordinates are set by function \p f.
    template <typename F>
    point(const Function_v2v<F>& f);

    /// Set all coordinates to the value \p c.
    void set_all(C c);

    /// Origin point (all coordinates are 0).
    static const point<G,C> origin;

    /// Shifting by \p dp.
    point<G,C>& operator+=(const delta& dp);

    /// Shifting by \p the inverse of dp.
    point<G,C>& operator-=(const delta& dp);

  /* FIXME: Seems highly non-generic!
     This (non documented change, even in ChangeLog) change was
     introduce by revision 1224, see
     https://trac.lrde.org/olena/changeset/1224#file2
     https://www.lrde.epita.fr/pipermail/olena-patches/2007-October/001592.html
  */
    operator typename internal::vec_of_point<G,C>::ret () const; // Hack to help g++-2.95.

    /// Explicit conversion towards mln::algebra::vec.
    vec to_vec() const;

    /// Transform to point in homogene coordinate system.
    h_vec to_h_vec() const;

    /// Point with all coordinates set to the maximum value.
    static const point<G,C>& plus_infty();

    /// Point with all coordinates set to the mininum value.
    static const point<G,C>& minus_infty();

    /// Return the underlying vector storing the coordinates.
    mln::algebra::vec<G::dim, C>& hook_coord_();

  protected:
    mln::algebra::vec<G::dim, C> coord_;
  };

  namespace internal
  {

    /// subject_impl specialization (Proxy)
    /// \{

    template <typename P, typename E>
    struct subject_point_impl;

    template <typename G, typename C, typename E>
    struct subject_point_impl< point<G,C>, E >
    {
      typename point<G,C>::vec to_vec() const;
      operator typename point<G,C>::vec () const; // Hack to help g++-2.95.

    private:
      const E& exact_() const;
    };

    /// \}

  } // end of namespace mln::internal


  /// FIXME...
  template <typename G, typename C>
  const mln::algebra::vec<point<G,C>::dim - 1, C>& cut_(const point<G,C>& p);

  template <typename C>
  const util::yes& cut_(const point<grid::tick,C>& p);



# ifndef MLN_INCLUDE_ONLY


  namespace internal
  {

    template <typename C, typename C2>
    inline
    C
    convert_data_(metal::bool_<false>, const C2& v)
    {
      return static_cast<C>(v);
    }

    template <typename C, typename C2>
    inline
    C
    convert_data_(metal::bool_<true>, const C2& v)
    {
      return static_cast<C>(round(v));
    }

    template <typename C, typename C2>
    inline
    C
    convert_data(const C2& v)
    {
      // If (C != float && C != double) && (C2 == float || C2 == double)
      // => We want to round the value.
      // Otherwise we can just statically cast.
      //
      return convert_data_<C>(
	typename mlc_and(
	  mlc_and(mlc_is_not(C,float),
		  mlc_is_not(C,double)),
	  mlc_or(mlc_is(C2,float),
		 mlc_is(C2, double)))::eval(), v);
    }



  } // end of namespace mln::internal



  namespace convert
  {

    namespace over_load
    {

      template <typename G, typename C1, typename C2>
      inline
      void
      from_to_(const point<G,C1>& from, point<G,C2>& to)
      {
	mlc_converts_to(C1,C2)::check();
	enum { dim = G::dim };

	for (unsigned i = 0; i < dim; ++i)
	  to[i] = mln::internal::convert_data<C2>(from[i]);
      }


      template <unsigned n, typename C1, typename G, typename C2>
      inline
      void
      from_to_(const mln::algebra::vec<n,C1>& from, point<G,C2>& to)
      {
	mlc_converts_to(C1, C2)::check();
	enum { dim = G::dim };
	mlc_bool(G::dim == n)::check();

	unsigned j = 0;
	for (unsigned i = dim - 2; i < dim; ++i)
	  to[i]   = mln::internal::convert_data<C2>(from[j++]);
	for (unsigned i = 2; i < dim; ++i, ++j)
	  to[i-j] = mln::internal::convert_data<C2>(from[j]);
      }

      template <unsigned n, typename C1, typename G>
      inline
      void
      from_to_(const mln::algebra::vec<n,C1>& from, point<G,C1>& to)
      {
	enum { dim = G::dim };
	mlc_bool(G::dim == n)::check();

	unsigned j = 0;
	for (unsigned i = dim - 2; i < dim; ++i)
	  to[i]   = from[j++];
	for (unsigned i = 2; i < dim; ++i, ++j)
	  to[i-j] = from[j];
      }


    } // end of namespace mln::convert::over_load

  } // end of namespace mln::convert


  template <typename G, typename C>
  inline
  const C& point<G,C>::operator[](unsigned i) const
  {
    assert(i < dim);
    return this->coord_[i];
  }

  template <typename G, typename C>
  inline
  C& point<G,C>::operator[](unsigned i)
  {
    assert(i < dim);
    return this->coord_[i];
  }

  template <typename G, typename C>
  inline
  const C&
  point<G,C>::last_coord() const
  {
    return this->coord_[dim - 1];
  }

  template <typename G, typename C>
  inline
  C&
  point<G,C>::last_coord()
  {
    return this->coord_[dim - 1];
  }


  // Constructors.

  template <typename G, typename C>
  inline
  point<G,C>::point()
  {
  }

  template <typename G, typename C>
  template <typename C2>
  inline
  point<G,C>::point(const mln::algebra::vec<dim,C2>& v)
  {
    convert::over_load::from_to_(v, *this);
  }


  template <typename G, typename C>
  inline
  point<G,C>::point(const mln::algebra::vec<dim,C>& v)
  {
    convert::over_load::from_to_(v, *this);
  }


  template <typename G, typename C>
  inline
  point<G,C>::point(C ind)
  {
    metal::bool_<(dim == 1)>::check();
    coord_[0] = ind;
  }

  template <typename G, typename C>
  inline
  point<G,C>::point(C row, C col)
  {
    metal::bool_<(dim == 2)>::check();
    coord_[0] = row;
    coord_[1] = col;
  }

  template <typename G, typename C>
  inline
  point<G,C>::point(C sli, C row, C col)
  {
    metal::bool_<(dim == 3)>::check();
    coord_[0] = sli;
    coord_[1] = row;
    coord_[2] = col;
  }

  template <typename G, typename C>
  template <typename F>
  inline
  point<G,C>::point(const Function_v2v<F>& f_)
  {
    mlc_converts_to(mln_result(F), C)::check();
    const F& f = exact(f_);
    for (unsigned i = 0; i < dim; ++i)
      coord_[i] = static_cast<C>( f(i) );
  }

  template <typename G, typename C>
  inline
  point<G,C>::point(const literal::origin_t&)
  {
    coord_.set_all(0);
  }

  template <typename G, typename C>
  inline
  point<G,C>&
  point<G,C>::operator=(const literal::origin_t&)
  {
    coord_.set_all(0);
    return *this;
  }

  template <typename G, typename C>
  inline
  point<G,C>::point(const literal::zero_t&)
  {
    metal::bool_<(dim == 1)>::check();
    coord_[0] = 1;
  }

  template <typename G, typename C>
  inline
  point<G,C>&
  point<G,C>::operator=(const literal::zero_t&)
  {
    metal::bool_<(dim == 1)>::check();
    coord_[0] = 1;
    return *this;
  }

  template <typename G, typename C>
  inline
  point<G,C>::point(const literal::one_t&)
  {
    metal::bool_<(dim == 1)>::check();
    coord_[0] = 1;
  }

  template <typename G, typename C>
  inline
  point<G,C>&
  point<G,C>::operator=(const literal::one_t&)
  {
    metal::bool_<(dim == 1)>::check();
    coord_[0] = 1;
    return *this;
  }

  template <typename G, typename C>
  inline
  void point<G,C>::set_all(C c)
  {
    coord_.set_all(c);
  }

  template <typename G, typename C>
  const point<G,C> point<G,C>::origin = all_to(0);

  template <typename G, typename C>
  inline
  point<G,C>&
  point<G,C>::operator+=(const delta& dp)
  {
    for (unsigned i = 0; i < dim; ++i)
      coord_[i] = static_cast<C>(coord_[i] + dp[i]);
    return *this;
  }

  template <typename G, typename C>
  inline
  point<G,C>&
  point<G,C>::operator-=(const delta& dp)
  {
    for (unsigned i = 0; i < dim; ++i)
      coord_[i] -= dp[i];
    return *this;
  }

  template <typename G, typename C>
  inline
  point<G,C>::operator typename internal::vec_of_point<G,C>::ret () const // Hack to help g++-2.95.
  {
    return to_vec();
  }

  template <typename G, typename C>
  inline
  typename point<G,C>::vec
  point<G,C>::to_vec() const
  {
    //FIXME: to be improved.
    if (dim > 2)
    {
      mln::algebra::vec<G::dim, float> tmp;
      unsigned j = 0;
      for (unsigned i = dim - 2; i < dim; ++i)
	tmp[j++] = mln::internal::convert_data<float>(coord_[i]);
      for (unsigned i = 2; i < dim; ++i, ++j)
	tmp[j] = mln::internal::convert_data<float>(coord_[i-j]);

      return tmp;
    }

    return coord_;
  }

  template <typename G, typename C>
  inline
  typename point<G,C>::h_vec
  point<G,C>::to_h_vec() const
  {
    mln::algebra::h_vec<G::dim, float> tmp;

    //FIXME: to be improved.
    if (dim == 1)
      tmp[0] = coord_[0];
    else
    {
      unsigned j = 0;
      for (unsigned i = dim - 2; i < dim; ++i)
	tmp[j++] = coord_[i];

      for (unsigned i = 2; i < dim; ++i, ++j)
	tmp[j] = coord_[i-j];

      tmp[G::dim] = 1;
    }

    return tmp;
  }


  template <typename G, typename C>
  inline
  const point<G,C>&
  point<G,C>::plus_infty()
  {
    static const point<G,C> the_(all_to(mln_max(C)));
    return the_;
  }

  template <typename G, typename C>
  inline
  const point<G,C>&
  point<G,C>::minus_infty()
  {
    static const point<G,C> the_(all_to(mln_min(C)));
    return the_;
  }

  template <typename G, typename C>
  inline
  mln::algebra::vec<G::dim, C>&
  point<G,C>::hook_coord_()
  {
    return coord_;
  }

  namespace internal
  {

    template <typename G, typename C, typename E>
    inline
    const E&
    subject_point_impl< point<G,C>, E >::exact_() const
    {
      return internal::force_exact<const E>(*this);
    }

    template <typename G, typename C, typename E>
    inline
    typename point<G,C>::vec
    subject_point_impl< point<G,C>, E >::to_vec() const
    {
      return exact_().get_subject().to_vec();
    }

    template <typename G, typename C, typename E>
    inline
    subject_point_impl< point<G,C>, E >::operator typename point<G,C>::vec () const // Hack to help g++-2.95.
    {
      return exact_().get_subject();
    }

  } // end of namespace mln::internal

  template <typename G, typename C>
  inline
  const mln::algebra::vec<point<G,C>::dim - 1, C>&
  cut_(const point<G,C>& p)
  {
    return *(mln::algebra::vec<point<G,C>::dim - 1, C>*)(& p.to_vec());
  }

  template <typename C>
  inline
  const util::yes&
  cut_(const point<grid::tick,C>& p)
  {
    util::yes* the_;
    return *the_;
  }

# endif // ! MLN_INCLUDE_ONLY


} // end of namespace mln


#endif // ! MLN_CORE_POINT_HH
