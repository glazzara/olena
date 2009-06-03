// Copyright (C) 2007, 2009 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_TEST_POSITIVE_HH
# define MLN_TEST_POSITIVE_HH

/// \file mln/test/positive.hh
///
/// Test if an image only contains positive values.

# include <mln/test/predicate.hh>
# include <mln/pw/all.hh>
# include <mln/fun/v2v/id.hh>
# include <mln/literal/zero.hh>


namespace mln
{

  namespace test
  {

    /// Test if an image only contains positive values.
    template <typename I>
    bool positive(const Image<I>& input);

  
# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    inline
    bool positive(const Image<I>& input_)
    {
      trace::entering("test::positive");

      const I& input = exact(input_);
      mln_precondition(input.is_valid());
      // FIXME: Below the '>=' op should properly work with signed/unsigned without
      // FIXME: warnings; so we really need to overload ops for functions when literals
      // FIXME: are involved.
      mln_value(I) zero_ = literal::zero;
      bool res = test::predicate(input.domain(),
				 pw::value(input) >= pw::cst(zero_));
      // FIXME: test the version below.
//       return test::predicate(input,
// 			     fun::v2v::id<mln_value(I)>() >= pw::cst(0));

      trace::exiting("test::positive");
      return res;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::test

} // end of namespace mln


#endif // ! MLN_TEST_POSITIVE_HH
