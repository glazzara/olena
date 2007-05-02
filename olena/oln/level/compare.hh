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

#ifndef OLN_LEVEL_COMPARE_HH
# define OLN_LEVEL_COMPARE_HH

# include <oln/core/concept/image.hh>
# include <oln/core/gen/pset_compare.hh>
# include <oln/core/gen/value.hh>

// FIXME: Rename this file as "comparison.hh".


namespace oln
{

  // Fwd decls.

  template <typename L, typename R>
  bool operator == (const Image<L>& lhs, const Image<R>& rhs);
  
  template <typename L, typename R>
  bool operator < (const Image<L>& lhs, const Image<R>& rhs);
  
  template <typename L, typename R> // required!
  bool operator <= (const Image<L>& lhs, const Image<R>& rhs);


  // with a value:

  template <typename I, typename V>
  bool operator == (const Image<I>& lhs, const value_<V>& val);

  template <typename I, typename V>
  bool operator < (const Image<I>& lhs, const value_<V>& val);

  template <typename I, typename V>
  bool operator <= (const Image<I>& lhs, const value_<V>& val);

  template <typename I, typename V>
  bool operator > (const Image<I>& lhs, const value_<V>& val);

  template <typename I, typename V>
  bool operator >= (const Image<I>& lhs, const value_<V>& val);


  // FIXME: is-it a good idea to have "ima == lit"?
  // FIXME: should we rather have "ima == f:p->v"?


# ifndef OLN_LNCLUDE_ONLY

  namespace impl
  {

    //  Image L  ==  Image R
    // ----------------------

    // Generic version.

    template <typename L, typename R>
    bool operator_equal_(const Image<L>& lhs, const Image<R>& rhs)
    {
      oln_piter(L) p(lhs.points());
      for_all(p)
	if (not (lhs(p) == rhs(p)))
	  return false;
      return true;
    }


    //  Image L  <  Image R
    // ---------------------

    // Generic version.

    template <typename L, typename R>
    bool operator_less_(const Image<L>& lhs, const Image<R>& rhs)
    {
      oln_piter(L) p(lhs.points());
      for_all(p)
	if (not (lhs(p) < rhs(p)))
	  return false;
      return true;
    }


    //  Image L  <=  Image R
    // ----------------------

    // Generic version.

    template <typename L, typename R>
    bool operator_less_or_equal_(const Image<L>& lhs, const Image<R>& rhs)
    {
      oln_piter(L) p(lhs.points());
      for_all(p)
	if (not (lhs(p) <= rhs(p)))
	  return false;
      return true;
    }


    //  Image I  ==  Value V
    // ----------------------

    // Generic version.

    template <typename I>
    bool operator_image_equal_to_value_(const Image<I>& lhs,
					const oln_value(I)& val)
    {
      oln_piter(I) p(lhs.points());
      for_all(p)
	if (not (lhs(p) == val))
	  return false;
      return true;
    }


    //  Image I  cmp  Value V
    // -----------------------

    // Generic versions.

    template <typename I>
    bool operator_image_less_than_value_(const Image<I>& lhs, const oln_value(I)& val)
    {
      oln_piter(I) p(lhs.points());
      for_all(p)
	if (not (lhs(p) < val))
	  return false;
      return true;
    }

    template <typename I>
    bool operator_image_less_or_equal_than_value_(const Image<I>& lhs, const oln_value(I)& val)
    {
      oln_piter(I) p(lhs.points());
      for_all(p)
	if (not (lhs(p) <= val))
	  return false;
      return true;
    }

    template <typename I>
    bool operator_image_greater_than_value_(const Image<I>& lhs, const oln_value(I)& val)
    {
      oln_piter(I) p(lhs.points());
      for_all(p)
	if (not (lhs(p) > val))
	  return false;
      return true;
    }

    template <typename I>
    bool operator_image_greater_or_equal_than_value_(const Image<I>& lhs, const oln_value(I)& val)
    {
      oln_piter(I) p(lhs.points());
      for_all(p)
	if (not (lhs(p) >= val))
	  return false;
      return true;
    }

  } // end of namespace oln::level::impl


  // Facades.

  template <typename L, typename R>
  bool operator == (const Image<L>& lhs, const Image<R>& rhs)
  {
    assert_same_grid_<L, R>::check();
    precondition(lhs.points() == rhs.points());
    return impl::operator_equal_(exact(lhs), exact(rhs));
  }

  template <typename L, typename R>
  bool operator < (const Image<L>& lhs, const Image<R>& rhs)
  {
    assert_same_grid_<L, R>::check();
    precondition(lhs.points() == rhs.points());
    return impl::operator_less_(exact(lhs), exact(rhs));
  }

  template <typename L, typename R>
  bool operator <= (const Image<L>& lhs, const Image<R>& rhs)
  {
    assert_same_grid_<L, R>::check();
    precondition(lhs.points() == rhs.points());
    return impl::operator_less_or_equal_(exact(lhs), exact(rhs));
  }


  // with a value:

  template <typename I, typename V>
  bool operator == (const Image<I>& lhs, const value_<V>& val)
  {
    // FIXME: insert static assertion?
    return impl::operator_image_equal_to_value_(exact(lhs), val.unwrap());
  }

  template <typename I, typename V>
  bool operator < (const Image<I>& lhs, const value_<V>& val)
  {
    // FIXME: insert static assertion?
    return impl::operator_image_less_than_value_(exact(lhs), val.unwrap());
  }

  template <typename I, typename V>
  bool operator <= (const Image<I>& lhs, const value_<V>& val)
  {
    // FIXME: insert static assertion?
    return impl::operator_image_less_or_equal_than_value_(exact(lhs), val.unwrap());
  }

  template <typename I, typename V>
  bool operator > (const Image<I>& lhs, const value_<V>& val)
  {
    // FIXME: insert static assertion?
    return impl::operator_image_greater_than_value_(exact(lhs), val.unwrap());
  }

  template <typename I, typename V>
  bool operator >= (const Image<I>& lhs, const value_<V>& val)
  {
    // FIXME: insert static assertion?
    return impl::operator_image_greater_or_equal_than_value_(exact(lhs), val.unwrap());
  }

# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln


#endif // ! OLN_LEVEL_COMPARE_HH
