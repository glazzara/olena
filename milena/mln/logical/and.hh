// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_LOGICAL_AND_HH
# define MLN_LOGICAL_AND_HH

/// \file
///
/// Point-wise "logical and" between binary images.

# include <mln/logical/includes.hh>
# include <mln/fun/vv2v/land.hh>


namespace mln
{

  namespace logical
  {

    /*! \brief Point-wise "logical and" between images \p lhs and \p
     *  rhs.
     *
     * \param[in] lhs First operand image.
     * \param[in] rhs Second operand image.
     * \result The result image.
     *
     * \pre \p lhs.domain == \p rhs.domain
     *
     * \ingroup mlnimageops
     */
    template <typename L, typename R>
    mln_ch_fun_vv2v(land, L, R)
    and_(const Image<L>& lhs, const Image<R>& rhs);


    /*! Point-wise in-place "logical and" of image \p rhs in image \p lhs.
     *
     * \param[in,out] lhs First operand image.
     * \param[in] rhs Second operand image.
     *
     * It performs: \n
     *   for all p of rhs.domain \n
     *     lhs(p) = lhs(p) and rhs(p)
     *
     * \pre \p rhs.domain >= \p lhs.domain
     *
     * \ingroup mlnimageops
     */
    template <typename L, typename R>
    void and_inplace(Image<L>& lhs, const Image<R>& rhs);


# ifndef MLN_INCLUDE_ONLY

    template <typename L, typename R>
    inline
    mln_ch_fun_vv2v(land, L, R)
    and_(const Image<L>& lhs, const Image<R>& rhs)
    {
      trace::entering("logical::and_");

      internal::tests(lhs, rhs);

      mln_fun_vv2v(land, L, R) f;
      mln_ch_fun_vv2v(land, L, R) output = data::transform(lhs, rhs, f);

      trace::exiting("logical::and_");
      return output;
    }

    template <typename L, typename R>
    inline
    void and_inplace(Image<L>& lhs, const Image<R>& rhs)
    {
      trace::entering("logical::and_inplace");

      mlc_converts_to(mln_fun_vv2v_result(land, L, R),
		      mln_value(L))::check();

      internal::tests(lhs, rhs);

      mln_fun_vv2v(land, L, R) f;
      data::transform_inplace(lhs, rhs, f);

      trace::exiting("logical::and_inplace");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::logical

} // end of namespace mln


#endif // ! MLN_LOGICAL_AND_HH
