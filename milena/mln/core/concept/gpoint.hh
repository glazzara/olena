// Copyright (C) 2007, 2008, 2009, 2011, 2012 EPITA Research and
// Development Laboratory (LRDE)
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

#ifndef MLN_CORE_CONCEPT_GPOINT_HH
# define MLN_CORE_CONCEPT_GPOINT_HH

/// \file
///
/// Definition of the concept of mln::Gpoint.


# include <mln/core/concept/site.hh>
# include <mln/core/concept/gdpoint.hh>
# include <mln/value/scalar.hh>
# include <mln/algebra/vec.hh>
# include <mln/util/ord.hh>
# include <mln/debug/format.hh>


namespace mln
{

  // Forward declaration.
  template <typename E> struct Gpoint;



  namespace trait
  {

    template < typename P, typename D >
    struct set_binary_< op::plus,
			mln::Gpoint, P, mln::Gdpoint, D >
    {
      typedef P ret;
    };

    template < typename P, typename D >
    struct set_binary_< op::minus,
			mln::Gpoint, P, mln::Gdpoint, D >
    {
      typedef P ret;
    };

    template < typename L, typename R >
    struct set_binary_< op::minus,
			mln::Gpoint, L, mln::Gpoint, R >
    {
      typedef mln_delta(L) ret;
    };

    template < typename L, typename R >
    struct set_binary_< op::times,
			mln::Gpoint, L,
			mln::Object, mln::value::scalar_<R> >
    {
      typedef L ret;
    };

    template < typename L, typename R >
    struct set_binary_< op::div,
			mln::Gpoint, L,
			mln::Object, mln::value::scalar_<R> >
    {
      typedef L ret;
    };

    template <typename P>
    struct set_unary_< op::ord, mln::Gpoint, P >
    {
      typedef mln::internal::ord_vec< P > ret;
    };

  } // end of namespace mln::trait



  // Gpoint category flag type.
  template <>
  struct Gpoint<void>
  {
    typedef Site<void> super;
  };


  /// Base class for implementation of point classes.
  ///
  /// A point is an element of a space.
  ///
  /// For instance, mln::point2d is the type of elements defined on the
  /// discrete square grid of the 2D plane.
  template <typename E>
  struct Gpoint : public Site<E>
  {
    typedef Gpoint<void> category;

    /*
      typedef grid;
      typedef delta;
      typedef vec;
      const vec& to_vec() const
    */
  protected:
    Gpoint();
  };


  /// Equality comparison between a couple of grid point \p lhs
  ///  and \p rhs.
  /*!
   * \param[in] lhs A first grid point.
   * \param[in] rhs A second grid point.
   *
   * \pre Both \p lhs and \p rhs have to be defined on the same
   * topology; otherwise this test does not compile.
   *
   * \return True if both grid points have the same
   * coordinates, otherwise false.
   *
   * \relates mln::Gpoint
   */
  template <typename L, typename R>
  bool operator==(const Gpoint<L>& lhs, const Gpoint<R>& rhs);


  /// Difference between a couple of grid point \p lhs and \p
  /// rhs.
   /*! \param[in] lhs A first grid point.
   * \param[in] rhs A second grid point.
   *
   * \warning There is no type promotion in Milena so the client
   * has to make sure that both points are defined with the same
   * type of coordinates.
   *
   * \pre Both \p lhs and \p rhs have to be defined on the same
   * topology and with the same type of coordinates; otherwise this
   * test does not compile.
   *
   * \post The result, \p dp, is such as \p lhs == \p rhs + \p dp.
   *
   * \return A delta point (temporary object).
   *
   * \relates mln::Gpoint
   * \see mln::Gdpoint
   */
  template <typename L, typename R>
  mln_delta(L)
  operator-(const Gpoint<L>& lhs, const Gpoint<R>& rhs);


  /// Add a delta-point \p rhs to a grid point \p lhs.
  ///
  /*! \param[in] p  A grid point.
   * \param[in] dp A delta-point.
   *
   * The type of \p dp has to compatible with the type of \p p.
   *
   * \return A point (temporary object).
   *
   * \relates mln::Gpoint
   * \see mln::Gdpoint
   */
  template <typename P, typename D>
  P
  operator+(const Gpoint<P>& p, const Gdpoint<D>& dp);



  /// Substract a delta-point \p dp to a grid point \p p.
  ///
  /*! \param[in] p A grid point.
   * \param[in] dp A delta-point.
   *
   * The type of \p dp has to compatible with the type of \p p.
   *
   * \return A point (temporary object).
   *
   * \see mln::Gdpoint
   * \see mln::Gdpoint
   */
  template <typename P, typename D>
  P
  operator-(const Gpoint<P>& p, const Gdpoint<D>& dp);


  /// Multiply a point \p p by a scalar \p s.
  //
  template <typename P, typename S>
  P
  operator*(const Gpoint<P>& p, const value::scalar_<S>& s);


  /// Print a grid point \p p into the output stream \p ostr.
  /*! \param[in,out] ostr An output stream.
   * \param[in] p A grid point.
   *
   * \return The modified output stream \p ostr.
   *
   * \relates mln::Gpoint
   */
  template <typename P>
  std::ostream& operator<<(std::ostream& ostr, const Gpoint<P>& p);


  /// Shift a point \p by a delta-point \p dp.
  /*! \param[in,out] p The targeted point.
   * \param[in] dp A delta-point.
   * \return A reference to the point \p p once translated by \p dp.
   *
   * \pre The type of \p dp has to be compatible with the type of \p
   * p.
   *
   * \relates mln::Gpoint
   */
  template <typename P, typename D>
  P& operator+=(Gpoint<P>& p, const Gdpoint<D>& dp);


  /// Shift a point \p by the negate of a delta-point \p dp.
  /*! \param[in,out] p The targeted point.
   * \param[in] dp A delta-point.
   * \return A reference to the point \p p once translated by - \p dp.
   *
   * \pre The type of \p dp has to be compatible with the type of \p
   * p.
   *
   * \relates mln::Gpoint
   */
  template <typename P, typename D>
  P& operator-=(Gpoint<P>& p, const Gdpoint<D>& dp);

  /// Divise a point \p by a scalar \p s.
  /*! \param[in,out] p The targeted point.
   * \param[in] dp A scalar.
   * \return A reference to the point \p p once divised by \p s.
   *
   * \relates mln::Gpoint
   */
  template <typename P, typename D>
  P operator/(const Gpoint<P>& p, const value::scalar_<D>& dp);

  // FIXME : add operators and traits?


  /// \internal Conversion: gpoint -> mln_delta
  template <typename P>
  void
  from_to_(const Gpoint<P>& from, mln_delta(P)& to);

  /// \internal Conversion: gpoint -> algebra::vec
  template <typename P, unsigned n, typename T>
  void
  from_to_(const Gpoint<P>& from, algebra::vec<n,T>& to);



# ifndef MLN_INCLUDE_ONLY


  // Gpoint

  template <typename E>
  inline
  Gpoint<E>::Gpoint()
  {
    typedef mln_grid(E)  grid;
    typedef mln_delta(E) delta;
    typedef mln_vec(E)   vec;
    vec (E::*m)() const = & E::to_vec;
    (void) m;
  }

  // Operators.

  template <typename L, typename R>
  inline
  bool operator==(const Gpoint<L>& lhs, const Gpoint<R>& rhs)
  {
    mlc_equal(mln_grid(L), mln_grid(R))::check();
    return exact(lhs).to_vec() == exact(rhs).to_vec();
  }

  template <typename L, typename R>
  inline
  mln_delta(L) // FIXME: promote!
  operator-(const Gpoint<L>& lhs, const Gpoint<R>& rhs)
  {
    mlc_equal(mln_grid(L), mln_grid(R))::check();
    mln_delta(L) tmp = exact(lhs).to_vec() - exact(rhs).to_vec();
    mln_postcondition(rhs + tmp == lhs);
    return tmp;
  }

  template <typename P, typename D>
  inline
  P // FIXME: promote!
  operator+(const Gpoint<P>& p, const Gdpoint<D>& dp)
  {
    mlc_equal(mln_grid(P), mln_grid(D))::check();
    P tmp = exact(p).to_vec() + exact(dp).to_vec();
    return tmp;
  }

  template <typename P, typename D>
  inline
  P // FIXME: promote!
  operator-(const Gpoint<P>& p, const Gdpoint<D>& dp)
  {
    mlc_equal(mln_grid(P), mln_grid(D))::check();
    P tmp = exact(p).to_vec() - exact(dp).to_vec();
    return tmp;
  }

  template <typename P, typename S>
  inline
  P
  operator*(const Gpoint<P>& p, const value::scalar_<S>& s)
  {
    S s_ = s.to_equiv();
    const unsigned n = P::dim;
    P tmp = exact(p);
    for (unsigned i = 0; i < n; ++i)
      tmp[i] *= s_;
    return tmp;
  }

  template <typename P>
  inline
  std::ostream& operator<<(std::ostream& ostr, const Gpoint<P>& p)
  {
    enum { n = P::dim };
    ostr << '(';
    for (unsigned i = 0; i < n; ++i)
      ostr << debug::format(exact(p)[i]) << (i == n - 1 ? ')' : ',');
    return ostr;
  }

  template <typename P, typename D>
  inline
  P& operator+=(Gpoint<P>& p, const Gdpoint<D>& dp)
  {
    mlc_equal(mln_grid(P), mln_grid(D))::check();
    return exact(p) = p + dp;
  }

  template <typename P, typename D>
  inline
  P& operator-=(Gpoint<P>& p, const Gdpoint<D>& dp)
  {
    mlc_equal(mln_grid(P), mln_grid(D))::check();
    return exact(p) = p - dp;
  }


  template <typename P, typename S>
  inline
  P
  operator/(const Gpoint<P>& p, const value::scalar_<S>& s_)
  {
    S s = s_.to_equiv();
    const unsigned n = P::dim;
    P tmp = exact(p);
    for (unsigned i = 0; i < n; ++i)
      tmp[i] /= s;
    return tmp;
  }



  // Conversions

  template <typename P>
  inline
  void
  from_to_(const Gpoint<P>& p_, mln_delta(P)& dp)
  {
    // Instead of "dp.to_vec() = exact(p).to_vec();" that
    // does not compile (cause to_vec returns const), we
    // have:
    enum { n = P::dim };
    const P& p = exact(p_);
    for (unsigned i = 0; i < n; ++i)
      dp[i] = p[i];
  }

  template <typename P, unsigned n, typename T>
  inline
  void
  from_to_(const Gpoint<P>& from_, algebra::vec<n,T>& to)
  {
    mlc_bool(n == P::dim)::check();
    const P& from = exact(from_);
    for (unsigned i = 0; i < n; ++i)
      to[i] = static_cast< T >(from[i]); // FIXME: cast -> best effort...
  }


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln



#endif // ! MLN_CORE_CONCEPT_GPOINT_HH
