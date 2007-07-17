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

#ifndef MLN_ARITH_MINUS_HH
# define MLN_ARITH_MINUS_HH

/*! \file mln/arith/minus.hh
 *
 * \brief Point-wise substraction between images.
 */

# include <mln/core/concept/image.hh>


namespace mln
{

  namespace arith
  {

    /*! Point-wise substraction of images \p lhs and \p rhs.
     *
     * \param[in] lhs First operand image.
     * \param[in] rhs Second operand image.
     * \param[out] output The result image.
     *
     * \pre \p output.domain == \p lhs.domain == \p rhs.domain
     */
    template <typename L, typename R, typename O>
    void minus(const Image<L>& lhs, const Image<R>& rhs, Image<O>& output);



# ifndef MLN_INCLUDE_ONLY

    template <typename L, typename R, typename O>
    void minus(const Image<L>& lhs_, const Image<R>& rhs_, Image<O>& output_)
    {
      const L& lhs = exact(lhs_);
      const R& rhs = exact(rhs_);
      O& output = exact(output_);

      mln_precondition(rhs.domain() == lhs.domain());
      mln_precondition(output.domain() == lhs.domain());

      mln_piter(I) p(output.domain());
      for_all(p)
	output(p) = lhs(p) - rhs(p);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::arith

} // end of namespace mln


#endif // ! MLN_ARITH_MINUS_HH
