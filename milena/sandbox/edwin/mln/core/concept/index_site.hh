// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

///
/// \brief Definition of the concept mln::index_site.
///
/// FIXME: doc!
///


#ifndef MLN_CORE_CONCEPT_INDEX_SITE_HH
# define MLN_CORE_CONCEPT_INDEX_SITE_HH

# include <mln/core/concept/site.hh>
# include <mln/value/concept/integer.hh>
# include <mln/core/concept/dindex_site.hh>


namespace mln
{
  // Forward declaration.
  template <typename E> struct Index_Site;

  namespace trait
  {

    template < typename P, typename D >
    struct set_binary_< op::plus,
			mln::Index_Site, P, mln::Dindex_Site, D >
    {
      typedef P ret;
    };

    template < typename P, typename D >
    struct set_binary_< op::minus,
			mln::Index_Site, P, mln::Dindex_Site, D >
    {
      typedef P ret;
    };

    template < typename L, typename R >
    struct set_binary_< op::minus,
			mln::Index_Site, L, mln::Index_Site, R >
    {
      typedef mln_delta(L) ret;
    };

    template < typename L, typename R >
    struct set_binary_< op::times,
			mln::Index_Site, L,
			mln::Object, mln::value::scalar_<R> >
    {
      typedef L ret;
    };

    template < typename L, typename R >
    struct set_binary_< op::div,
			mln::Index_Site, L,
			mln::Object, mln::value::scalar_<R> >
    {
      typedef L ret;
    };

    template <typename P>
    struct set_unary_< op::ord, mln::Index_Site, P >
    {
      typedef mln::internal::ord_less< P > ret;
    };

  }

  // Index Site category flag type.
  template <>
  struct Index_Site<void>
  {
    typedef Site<void> super;
  };

  template <typename E>
  struct Index_Site : public Site<E>
  {
    typedef Index_Site<void> category;

    /*
    typedef delta;
    typedef index_t;

    const index_t& index() const;
    void change_index(const index_t&);

    void dec_index();
    void inc_index();

    */

  protected:
    Index_Site();
  };

  // Conversion.

  namespace convert
  {

    namespace over_load
    {

      template <typename P>
      void
      from_to_(const Index_Site<P>& from, mln_delta(P)& to);

    } // end of namespace mln::convert::over_load

  } // end of namespace mln::convert


  /// Operators.
  /// \{
  template <typename P>
  std::ostream&
  operator<<(std::ostream& ostr, const Index_Site<P>& dp);

  template <typename L, typename R>
  bool operator==(const Index_Site<L>& lhs, const Index_Site<R>& rhs);

  template <typename L, typename R>
  bool operator<(const Index_Site<L>& lhs, const Index_Site<R>& rhs);

  template <typename P, typename D>
  P operator+(const Index_Site<P>& p, const Dindex_Site<D>& dp);

  template <typename P, typename D>
  P operator-(const Index_Site<P>& p, const Dindex_Site<D>& dp);

  template <typename P, typename S>
  P operator*(const Index_Site<P>& p, const value::scalar_<S>& s);

  template <typename P, typename S>
  P operator/(const Index_Site<P>& p, const value::scalar_<S>& s);

  template <typename L, typename R>
  mln_delta(L) operator-(const Index_Site<L>& lhs, const Index_Site<R>& rhs);

  template <typename P, typename D>
  P& operator+=(Index_Site<P>& p, const Dindex_Site<D>& dp);

  template <typename P, typename D>
  P& operator-=(Index_Site<P>& p, const Dindex_Site<D>& dp);

  template <typename P, typename S>
  P& operator*=(Index_Site<P>& p, const value::scalar_<S>& s);

  template <typename P, typename S>
  P& operator/=(Index_Site<P>& p, const value::scalar_<S>& s);

  /// \}

# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  inline
  Index_Site<E>::Index_Site()
  {
    typedef mln_delta(E) delta;
    typedef typename E::index_t index_t;

    void (E::*m1)() = & E::inc_index;
    m1 = 0;
    void (E::*m2)() = & E::dec_index;
    m2 = 0;
    const index_t& (E::*m3)() const = & E::index;
    m3 = 0;
    void (E::*m4)(const index_t&) = & E::change_index;
    m4 = 0;
  }

  namespace convert
  {

    namespace over_load
    {

      template <typename P>
      void
      from_to_(const Index_Site<P>& from, mln_delta(P)& to)
      {
	to.value() = exact(from).index();
      }

    } // end of namespace mln::convert::over_load

  } // end of namespace mln::convert

  template <typename P>
  std::ostream&
  operator<<(std::ostream& ostr, const Index_Site<P>& p)
    {
      ostr << '(' << debug::format(exact(p).index()) << ')';
      return ostr;
    }

  template <typename L, typename R>
  bool operator==(const Index_Site<L>& lhs, const Index_Site<R>& rhs)
  {
    return exact(lhs).index() == exact(rhs).index();
  }

 template <typename L, typename R>
 bool operator<(const Index_Site<L>& lhs, const Index_Site<R>& rhs)
 {
   return exact(lhs).index() < exact(rhs).index();
 }

  template <typename P, typename D>
  P operator+(const Index_Site<P>& p, const Dindex_Site<D>& dp)
  {
    P tmp = exact(p);
    tmp.change_index(tmp.index() + exact(dp).value());
    return tmp;
  }

  template <typename P, typename D>
  P operator-(const Index_Site<P>& p, const Dindex_Site<D>& dp)
  {
    P tmp = exact(p);
    tmp.change_index(tmp.index() - exact(dp).value());
    return tmp;
  }

  template <typename P, typename S>
  P operator*(const Index_Site<P>& p, const value::scalar_<S>& s)
  {
    S equiv = s.to_equiv();
    P tmp = exact(p);
    tmp.change_index(tmp.index() * equiv);
    return tmp;
  }


  template <typename P, typename S>
  P operator/(const Index_Site<P>& p, const value::scalar_<S>& s)
  {
    S equiv = s.to_equiv();
    P tmp = exact(p);
    tmp.change_index(tmp.index() / equiv);
    return tmp;
  }


  template <typename L, typename R>
  mln_delta(L) operator-(const Index_Site<L>& lhs, const Index_Site<R>& rhs)
  {
    mln_delta(L) delta(exact(lhs).index() - exact(rhs).index());
    mln_postcondition(rhs + delta == lhs);
    return delta;
  }

  template <typename P, typename D>
  P& operator+=(Index_Site<P>& p, const Dindex_Site<D>& dp)
  {
    exact(p).change_index(exact(p).index() + exact(dp).value());
    return exact(p);
  }

  template <typename P, typename D>
  P& operator-=(Index_Site<P>& p, const Dindex_Site<D>& dp)
  {
    exact(p).change_index(exact(p).index() - exact(dp).value());
    return exact(p);
  }

  template <typename P, typename S>
  P& operator*=(Index_Site<P>& p, const value::scalar_<S>& s)
  {
    exact(p).change_index(exact(p).index() * s);
    return exact(p);
  }

  template <typename P, typename S>
  P& operator/=(Index_Site<P>& p, const value::scalar_<S>& s)
  {
    exact(p).change_index(exact(p).index() / s);
    return exact(p);
  }
# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // !MLN_CORE_CONCEPT_INDEX_SITE_HH
