// Copyright (C) 2007, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_DATA_COMPARE_HH
# define MLN_DATA_COMPARE_HH

/// \file
///
/// Comparison operators between the pixel values of images.

# include <mln/core/concept/image.hh>
# include <mln/fun/vv2b/eq.hh>
# include <mln/fun/vv2b/le.hh>
# include <mln/fun/vv2b/lt.hh>
# include <mln/test/predicate.hh>


namespace mln
{

  /*! \brief Point-wise test if the pixel values of \p lhs are equal
   * to the pixel values of \p rhs.
   *
   * \param[in] lhs A first image.
   * \param[in] rhs A second image.
   *
   * \pre lhs.domain == rhs.domain
   *
   * \ingroup mlnimageops
   */
  template <typename L, typename R>
  bool operator == (const Image<L>& lhs, const Image<R>& rhs);


  /*! \brief Point-wise test if the pixel values of \p lhs are point-wise
   * less than the pixel values of \p rhs.
   *
   * \param[in] lhs A first image.
   * \param[in] rhs A second image.
   *
   * \pre lhs.domain == rhs.domain
   *
   * \ingroup mlnimageops
   */
  template <typename L, typename R>
  bool operator < (const Image<L>& lhs, const Image<R>& rhs);


  /*! \brief Point-wise test if the pixel values of \p lhs are point-wise
   * less than or equal to the pixel values of \p rhs.
   *
   * \param[in] lhs A first image.
   * \param[in] rhs A second image.
   *
   * \pre lhs.domain == rhs.domain
   *
   * \ingroup mlnimageops
   */
  template <typename L, typename R> // required!
  bool operator <= (const Image<L>& lhs, const Image<R>& rhs);



# ifndef MLN_INCLUDE_ONLY


  template <typename L, typename R>
  inline
  bool operator == (const Image<L>& lhs_, const Image<R>& rhs_)
  {
    trace::entering("data::compare (==)");

    const L& lhs = exact(lhs_);
    const R& rhs = exact(rhs_);

    mln_precondition(lhs.is_valid());
    mln_precondition(rhs.is_valid());
    mln_precondition(lhs.domain() == rhs.domain());

    typedef fun::vv2b::eq<mln_value(L), mln_value(R)> F;
    bool res = test::predicate(lhs, rhs, F());

    trace::exiting("data::compare (==)");
    return res;
  }


  template <typename L, typename R>
  inline
  bool operator < (const Image<L>& lhs_, const Image<R>& rhs_)
  {
    trace::entering("data::compare (<)");

    const L& lhs = exact(lhs_);
    const R& rhs = exact(rhs_);

    mln_precondition(lhs.domain() == rhs.domain());

    typedef fun::vv2b::lt<mln_value(L), mln_value(R)> F;
    bool res = test::predicate(lhs_, rhs_, F());

    trace::exiting("data::compare (<)");
    return res;
  }


  template <typename L, typename R> // required!
  inline
  bool operator <= (const Image<L>& lhs_, const Image<R>& rhs_)
  {
    trace::entering("data::compare (<=)");

    const L& lhs = exact(lhs_);
    const R& rhs = exact(rhs_);

    mln_precondition(lhs.domain() == rhs.domain());

    typedef fun::vv2b::le<mln_value(L), mln_value(R)> F;
    bool res = test::predicate(lhs_, rhs_, F());

    trace::exiting("data::compare (<=)");
    return res;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_DATA_COMPARE_HH
