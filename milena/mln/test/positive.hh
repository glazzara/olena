// Copyright (C) 2007, 2008, 2009, 2011, 2014 EPITA Research and
// Development Laboratory (LRDE).
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

#ifndef MLN_TEST_POSITIVE_HH
# define MLN_TEST_POSITIVE_HH

/// \file
///
/// Test if an image only contains positive or null
/// (i.e. ``non-negative'') values.

# include <mln/test/predicate.hh>
# include <mln/pw/all.hh>
# include <mln/fun/v2v/id.hh>
# include <mln/literal/zero.hh>


namespace mln
{

  namespace test
  {

    /// \brief Test if an image only contains positive or null
    /// (i.e. ``non-negative'') values.
    ///
    /// This routine checks whether all the values of an image are
    /// non-negative, thus considering includes 0 as a valid value
    /// (which is usually not the case of the term ``positive'' in
    /// English).
    template <typename I>
    bool positive(const Image<I>& input);

  
# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    inline
    bool positive(const Image<I>& input_)
    {
      mln_trace("test::positive");

      const I& input = exact(input_);
      mln_precondition(input.is_valid());
      /* FIXME: Below the '>=' op should properly work with
	 signed/unsigned without warnings; so we really need to
	 overload ops for functions when literals are involved.  */
      mln_value(I) zero_ = literal::zero;
      bool res = test::predicate(input.domain(),
				 pw::value(input) >= pw::cst(zero_));
      /* FIXME: test the version below.  */
//       return test::predicate(input,
// 			     fun::v2v::id<mln_value(I)>() >= pw::cst(0));

      return res;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::test

} // end of namespace mln


#endif // ! MLN_TEST_POSITIVE_HH
