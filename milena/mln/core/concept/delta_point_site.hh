// Copyright (C) 2007, 2009, 2012, 2013 EPITA Research and Development
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

#ifndef MLN_CORE_CONCEPT_DELTA_POINT_SITE_HH
# define MLN_CORE_CONCEPT_DELTA_POINT_SITE_HH

/*! \file
 *
 * \brief Definition of the concept of mln::Delta_Point_Site.
 */

# include <mln/core/concept/object.hh>
# include <mln/core/grids.hh>
# include <mln/trait/all.hh>


namespace mln
{

  // Fwd decl.
  template <typename E> struct Delta_Point_Site;


  /// \cond INTERNAL_API
  namespace trait
  {

    template < typename L, typename R >
    struct set_binary_< op::plus,
			mln::Delta_Point_Site, L, mln::Delta_Point_Site, R >
    {
      typedef mln_dpoint(L) ret;
    };

    template < typename L, typename R >
    struct set_binary_< op::minus,
			mln::Delta_Point_Site, L, mln::Delta_Point_Site, R >
    {
      typedef mln_dpoint(L) ret;
    };

  } // end of namespace mln::trait
  /// \endcond


  /// \cond INTERNAL_API
  /// \brief Delta point site category flag type.
  template <>
  struct Delta_Point_Site<void>
  {
    typedef Object<void> super;
  };
  /// \endcond


  /*!
    \brief FIXME: Doc!
    \ingroup modconcepts
  */
  template <typename E>
  struct Delta_Point_Site : public Object<E>
  {
    typedef Delta_Point_Site<void> category;

    /*
    enum { dim };
    typedef mesh;

    typedef point;
    typedef dpoint;
    typedef coord;

    const dpoint& to_dpoint() const;
    coord operator[](unsigned i) const;
    */

  protected:
    Delta_Point_Site();
  };



  // Operators.

  template <typename D>
  std::ostream&
  operator<<(std::ostream& ostr, const Delta_Point_Site<D>& dp);


  template <typename L, typename R>
  bool
  operator==(const Delta_Point_Site<L>& lhs, const Delta_Point_Site<R>& rhs);

  template <typename L, typename R>
  bool
  operator<(const Delta_Point_Site<L>& lhs, const Delta_Point_Site<R>& rhs);


  template <typename L, typename R>
  mln_dpoint(L) // FIXME: promote!
    operator+(const Delta_Point_Site<L>& lhs, const Delta_Point_Site<R>& rhs);

  template <typename L, typename R>
  mln_dpoint(L) // FIXME: promote!
    operator-(const Delta_Point_Site<L>& lhs, const Delta_Point_Site<R>& rhs);




# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  inline
  Delta_Point_Site<E>::Delta_Point_Site()
  {
    int dim = E::dim;
    mln_invariant(dim > 0);
    dim = 0;
    typedef mln_mesh(E)   mesh;
    typedef mln_point(E)  point;
    typedef mln_dpoint(E) dpoint;
    typedef mln_coord(E)  coord;
    const dpoint& (E::*m1)() const = & E::to_dpoint;
    m1 = 0;
    coord (E::*m2)(unsigned i) const = & E::operator[];
    m2 = 0;
  }


  template <typename D>
  inline
  std::ostream& operator<<(std::ostream& ostr, const Delta_Point_Site<D>& dp_)
  {
    const D& dp = exact(dp_);
    ostr << '(';
    for (unsigned i = 0; i < D::dim; ++i)
      ostr << dp[i] << (i == D::dim - 1 ? ')' : ',');
    return ostr;
  }


  template <typename L, typename R>
  inline
  bool operator==(const Delta_Point_Site<L>& lhs_, const Delta_Point_Site<R>& rhs_)
  {
    mln::metal::bool_<(int(L::dim) == int(R::dim))>::check();
    const L& lhs = exact(lhs_);
    const R& rhs = exact(rhs_);
    for (unsigned i = 0; i < L::dim; ++i)
      if (lhs[i] != rhs[i])
	return false;
    return true;
  }

  template <typename L, typename R>
  inline
  bool operator<(const Delta_Point_Site<L>& lhs_, const Delta_Point_Site<R>& rhs_)
  {
    mln::metal::bool_<(int(L::dim) == int(R::dim))>::check();
    const L& lhs = exact(lhs_);
    const R& rhs = exact(rhs_);
    for (unsigned i = 0; i < L::dim; ++i)
      {
	if (lhs[i] == rhs[i])
	  continue;
	return lhs[i] < rhs[i];
      }
    return false;
  }

  template <typename L, typename R>
  inline
  mln_dpoint(L) // FIXME: promote!
  operator+(const Delta_Point_Site<L>& lhs_, const Delta_Point_Site<R>& rhs_)
  {
    mln::metal::bool_<(int(L::dim) == int(R::dim))>::check();
    const L& lhs = exact(lhs_);
    const R& rhs = exact(rhs_);
    mln_dpoint(L) tmp;
    for (unsigned i = 0; i < L::dim; ++i)
      tmp[i] = lhs[i] + rhs[i];
    return tmp;
  }

  template <typename L, typename R>
  inline
  mln_dpoint(L) // FIXME: promote!
  operator-(const Delta_Point_Site<L>& lhs_, const Delta_Point_Site<R>& rhs_)
  {
    mln::metal::bool_<(int(L::dim) == int(R::dim))>::check();
    const L& lhs = exact(lhs_);
    const R& rhs = exact(rhs_);
    mln_dpoint(L) tmp;
    for (unsigned i = 0; i < L::dim; ++i)
      tmp[i] = lhs[i] - rhs[i];
    return tmp;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_DELTA_POINT_SITE_HH
