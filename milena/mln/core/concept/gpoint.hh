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

#ifndef MLN_CORE_CONCEPT_GPOINT_HH
# define MLN_CORE_CONCEPT_GPOINT_HH

/*! \file mln/core/concept/gpoint.hh
 *
 * \brief Definition of the concept of mln::Gpoint.
 */

# include <mln/core/concept/site.hh>
# include <mln/core/concept/gdpoint.hh>
# include <mln/value/concept/scalar.hh>
# include <mln/algebra/vec.hh>
# include <mln/util/ord.hh>


namespace mln
{

  // Fwd decl.
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


  /*! \brief Base class for implementation of point classes.
   *
   * A point is an element of a space.
   *
   * For instance, mln::point2d is the type of elements defined on the
   * discrete square grid of the 2D plane.
   */
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


  namespace convert
  {

    template <typename P>
    void
    from_to(const Gpoint<P>& from, mln_delta(P)& to);

    template <typename P, unsigned n, typename T>
    void
    from_to(const Gpoint<P>& from, algebra::vec<n,T>& to);

    template <unsigned n, typename T, typename P>
    void
    from_to(const algebra::vec<n,T>& from, Gpoint<P>& to);

  } // end of namespace::convert



  /*! \brief Equality comparison between a couple of grid point \p lhs
   *  and \p rhs.
   *
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


  /*! \brief Difference between a couple of grid point \p lhs and \p
   *  rhs.
   *
   * \param[in] lhs A first grid point.
   * \param[in] rhs A second grid point.
   *
   * \warning There is no type promotion in milena so the client
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


  /*! \brief Add a delta-point \p rhs to a grid point \p lhs.
   *
   * \param[in] p  A grid point.
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



  /*! \brief Substract a delta-point \p rhs to a grid point \p lhs.
   *
   * \param[in] p A grid point.
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


  /*! \brief Print a grid point \p p into the output stream \p ostr.
   *
   * \param[in,out] ostr An output stream.
   * \param[in] p A grid point.
   *
   * \return The modified output stream \p ostr.
   *
   * \relates mln::Gpoint
   */
  template <typename P>
  std::ostream& operator<<(std::ostream& ostr, const Gpoint<P>& p);


  /*! \brief Shift a point \p by a delta-point \p dp.
   *
   * \param[in,out] p The targeted point.
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


  /*! \brief Shift a point \p by the negate of a delta-point \p dp.
   *
   * \param[in,out] p The targeted point.
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

  /*! \brief Divise a point \p by a scalar \p s.
   *
   * \param[in,out] p The targeted point.
   * \param[in] dp A scalar.
   * \return A reference to the point \p p once divised by \p s.
   *
   * \relates mln::Gpoint
   */
  template <typename P, typename D>
  P operator/(Gpoint<P>& p, const value::Scalar<D>& dp);

  // FIXME : add operators and traits?



# ifndef MLN_INCLUDE_ONLY


  // Gpoint

  template <typename E>
  inline
  Gpoint<E>::Gpoint()
  {
    typedef mln_grid(E)  grid;
    typedef mln_delta(E) delta;
    typedef mln_vec(E)   vec;
    const vec& (E::*m)() const = & E::to_vec;
    m = 0;
  }


  // convert::from_to

  namespace convert
  {

    // Gpoint -> delta
    template <typename P>
    inline
    void
    from_to(const Gpoint<P>& p_, mln_delta(P)& dp)
    {
      // Instead of "dp.to_vec() = exact(p).to_vec();" that
      // does not compile (cause to_vec returns const), we
      // have:
      enum { n = P::dim };
      const P& p = exact(p_);
      for (unsigned i = 0; i < n; ++i)
	dp[i] = p[i];
    }

    // Gpoint -> algebra::vec.
    template <typename P, unsigned n, typename T>
    void
    from_to(const Gpoint<P>& from_, algebra::vec<n,T>& to)
    {
      mlc_bool(n == P::dim)::check();
      const P& from = exact(from_);
      for (unsigned i = 0; i < n; ++i)
	to[i] = static_cast< T >(from[i]); // FIXME: cast -> best effort...
    }

    // algebra::vec -> Gpoint.
    template <unsigned n, typename T, typename P>
    inline
    void
    from_to(const algebra::vec<n,T>& from, Gpoint<P>& to_)
    {
      mlc_bool(P::dim == n)::check();
      P& to = exact(to_);
      for (unsigned i = 0; i < n; ++i)
	to[i] = static_cast< typename P::coord >(from[i]); // FIXME: cast -> best effort...
    }

  } // end of namespace::convert


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

  template <typename P>
  inline
  std::ostream& operator<<(std::ostream& ostr, const Gpoint<P>& p)
  {
    return ostr << exact(p).to_vec();
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
  operator/(Gpoint<P>& p, const value::Scalar<S>& s_)
  {
    return p.to_vec() / s_;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln



#endif // ! MLN_CORE_CONCEPT_GPOINT_HH
