// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_LEVEL_ABS_HH
# define MLN_LEVEL_ABS_HH

/*! \file mln/level/abs.hh
 *
 * \brief Apply the absolute value (abs) function to image pixel
 * values.
 */

# include <mln/fun/v2v/abs.hh>
# include <mln/level/apply.hh>
# include <mln/level/transform.hh>


namespace mln
{

  namespace level
  {


    /*! Apply the absolute value (abs) function to image pixel values.
     *
     * \param[in] input The input image.
     * \param[out] output The output image.
     */
    template <typename I, typename O>
    void abs(const Image<I>& input, Image<O>& output);


    /*! Apply the absolute value (abs) function to image pixel values.
     *
     * \param[in,out] input The input image.
     */
    template <typename I>
    void abs_inplace(Image<I>& input);


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename O>
    inline
    void abs(const Image<I>& input, Image<O>& output)
    {
      trace::entering("level::abs");

      mln_precondition(exact(input).domain() == exact(output).domain());
      output = level::transform(input, fun::v2v::abs<mln_value(I)>());

      trace::exiting("level::abs");
    }

    template <typename I>
    inline
    void abs_inplace(Image<I>& input)
    {
      trace::entering("level::abs_inplace");

      mln_precondition(exact(input).is_valid());
      level::apply(input, fun::v2v::abs<mln_value(I)>());

      trace::exiting("level::abs_inplace");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::level

} // end of namespace mln


#endif // ! MLN_LEVEL_ABS_HH
