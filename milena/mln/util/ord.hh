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

#ifndef MLN_UTIL_ORD_HH
# define MLN_UTIL_ORD_HH

/// \file
///
/// Definition of an ordering type and routine.


# include <mln/core/concept/object.hh>
# include <mln/trait/op/ord.hh>



namespace mln
{

  namespace util
  {

    /// Function-object that defines an ordering between objects with
    /// type \p T: \a lhs R \a rhs.  Its meaning is "lhs less-than rhs."
    template <typename T>
    struct ord
    {
      bool operator()(const T& lhs, const T& rhs) const;
    };



    /// Routine to test if \a lhs is strictly "less-than" \a rhs.
    template <typename T>
    bool ord_strict(const T& lhs, const T& rhs);


    /// Routine to test if \a lhs is "less-than or equal-to" \a rhs.
    template <typename T>
    bool ord_weak(const T& lhs, const T& rhs);


    template <typename T1, typename T2>
    bool ord_lexi_strict(const T1& lhs_1, const T2& lhs_2,
			 const T1& rhs_1, const T2& rhs_2);



# ifndef MLN_INCLUDE_ONLY

    // ord<T>

    template <typename T>
    inline
    bool
    ord<T>::operator()(const T& lhs, const T& rhs) const
    {
      typedef typename mln::trait::op::ord<T>::ret F;
      static const F f_ord = F();
      return f_ord.strict(lhs, rhs);
    }

    // ord_strict

    template <typename T>
    inline
    bool
    ord_strict(const T& lhs, const T& rhs)
    {
      typedef typename mln::trait::op::ord<T>::ret F;
      static const F f_ord = F();
      return f_ord.strict(lhs, rhs);
    }

    // ord_weak

    template <typename T>
    inline
    bool ord_weak(const T& lhs, const T& rhs)
    {
      typedef typename mln::trait::op::ord<T>::ret F;
      static const F f_ord = F();
      return f_ord.weak(lhs, rhs);
    }

    // ord_lex_strict

    template <typename T1, typename T2>
    inline
    bool
    ord_lexi_strict(const T1& lhs_1, const T2& lhs_2,
		    const T1& rhs_1, const T2& rhs_2)
    {
      return
	util::ord_strict(lhs_1, rhs_1) ||
	(lhs_1 == rhs_1 && util::ord_strict(lhs_2, rhs_2));
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::util

} // end of namespace mln


#endif // ! MLN_UTIL_ORD_HH
