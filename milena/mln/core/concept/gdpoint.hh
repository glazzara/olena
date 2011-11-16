// Copyright (C) 2007, 2008, 2009, 2011 EPITA Research and Development
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

#ifndef MLN_CORE_CONCEPT_GDPOINT_HH
# define MLN_CORE_CONCEPT_GDPOINT_HH

/// \file
///
/// Definition of the concept of mln::Gdpoint.

# include <mln/core/concept/object.hh>
# include <mln/core/grids.hh>
# include <mln/trait/all.hh>
# include <mln/value/scalar.hh>
# include <mln/debug/format.hh>


namespace mln
{

  // Forward declaration.
  template <typename E> struct Gdpoint;



  namespace trait
  {

    // FIXME: Add promotion.

    template < typename L, typename R >
    struct set_binary_< op::plus,
			mln::Gdpoint, L, mln::Gdpoint, R >
    {
      typedef L ret;
    };

    template < typename L, typename R >
    struct set_binary_< op::minus,
			mln::Gdpoint, L, mln::Gdpoint, R >
    {
      typedef L ret;
    };

    template < typename D, typename S >
    struct set_binary_< op::times,
			mln::Gdpoint, D,
			mln::value::Scalar, S >
    {
      typedef D ret;
    };

    template <typename D>
    struct set_unary_< op::ord, mln::Gdpoint, D >
    {
      typedef mln::internal::ord_vec< D > ret;
    };

  } // end of namespace mln::trait



  /// Delta point site category flag type.
  template <>
  struct Gdpoint<void>
  {
    typedef Object<void> super;
  };


  /// FIXME: Doc!
  template <typename E>
  struct Gdpoint : public Object<E>
  {
    typedef Gdpoint<void> category;

    /*
    typedef grid;
    typedef vec;
    const vec& to_vec() const;
    */

  protected:
    Gdpoint();
  };



  // Operators.

  template <typename D>
  std::ostream&
  operator<<(std::ostream& ostr, const Gdpoint<D>& dp);


  template <typename L, typename R>
  bool
  operator==(const Gdpoint<L>& lhs, const Gdpoint<R>& rhs);


  template <typename L, typename R>
  L // FIXME: promote!
  operator+(const Gdpoint<L>& lhs, const Gdpoint<R>& rhs);

  template <typename L, typename R>
  L // FIXME: promote!
  operator-(const Gdpoint<L>& lhs, const Gdpoint<R>& rhs);

  template <typename D, typename S>
  D // FIXME: promote!
  operator*(const Gdpoint<D>& lhs, const value::Scalar<S>& rhs);



  namespace convert
  {

    namespace over_load
    {

      template <typename D>
      void
      from_to_(const Gdpoint<D>& from, mln_site(D)& to);

//     template <typename D, unsigned n, typename T>
//     void
//     from_to_(const Gdpoint<D>& from, algebra::vec<n,T>& to);

//     template <unsigned n, typename T, typename D>
//     void
//     from_to_(const algebra::vec<n,T>& from, Gdpoint<D>& to);

    } // end of namespace mln::convert::over_load

  } // end of namespace mln::convert


# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  inline
  Gdpoint<E>::Gdpoint()
  {
    typedef mln_grid(E) grid;
    typedef mln_vec(E)  vec;
    vec (E::*m)() const = & E::to_vec;
    (void) m;
  }


  template <typename D>
  inline
  std::ostream& operator<<(std::ostream& ostr, const Gdpoint<D>& dp)
  {
    enum { n = D::dim };
    ostr << '(';
    for (unsigned i = 0; i < n; ++i)
      ostr << debug::format(exact(dp)[i]) << (i == n - 1 ? ')' : ',');
    return ostr;
  }


  template <typename L, typename R>
  inline
  bool operator==(const Gdpoint<L>& lhs, const Gdpoint<R>& rhs)
  {
    mlc_equal(mln_grid(L), mln_grid(R))::check();
    return exact(lhs).to_vec() == exact(rhs).to_vec();
  }

  template <typename L, typename R>
  inline
  L // FIXME: promote!
  operator+(const Gdpoint<L>& lhs, const Gdpoint<R>& rhs)
  {
    mlc_equal(mln_grid(L), mln_grid(R))::check();
    L tmp = exact(lhs).to_vec() + exact(rhs).to_vec();
    return tmp;
  }

  template <typename L, typename R>
  inline
  L // FIXME: promote!
  operator-(const Gdpoint<L>& lhs, const Gdpoint<R>& rhs)
  {
    mlc_equal(mln_grid(L), mln_grid(R))::check();
    L tmp = exact(lhs).to_vec() - exact(rhs).to_vec();
    return tmp;
  }

  template <typename D, typename S>
  D // FIXME: promote!
  operator*(const Gdpoint<D>& lhs, const value::Scalar<S>& rhs)
  {
    D tmp = exact(lhs).to_vec() * exact(rhs);
    return tmp;
  }


  namespace convert
  {

    namespace over_load
    {

      template <typename D>
      inline
      void
      from_to_(const Gdpoint<D>& dp_, mln_site(D)& p)
      {
	enum { n = D::dim };
	const D& dp = exact(dp_);
	for (unsigned i = 0; i < n; ++i)
	  p[i] = dp[i];
      }

    } // end of namespace mln::convert::over_load

  } // end of namespace mln::convert

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_GDPOINT_HH
