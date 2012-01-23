// Copyright (C) 2007, 2008, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_DATA_ABS_HH
# define MLN_DATA_ABS_HH

/*! \file
 *
 * \brief Apply the absolute value (abs) function to image pixel
 * values.
 */

# include <mln/fun/v2v/abs.hh>
# include <mln/data/apply.hh>
# include <mln/data/transform.hh>


namespace mln
{

  namespace data
  {


    /*! \brief Apply the absolute value (abs) function to image pixel
     *  values.
     *
     * \param[in] input The input image.
     * \param[out] output The output image.
     *
     * \ingroup mlndata
     */
    template <typename I, typename O>
    void abs(const Image<I>& input, Image<O>& output);


    /*! \brief Apply the absolute value (abs) function to image pixel
     *  values.
     *
     * \param[in,out] input The input image.
     *
     * \ingroup mlndata
     */
    template <typename I>
    void abs_inplace(Image<I>& input);


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename O>
    inline
    void abs(const Image<I>& input, Image<O>& output)
    {
      trace::entering("data::abs");

      mln_precondition(exact(input).domain() == exact(output).domain());
      exact(output) = data::transform(input, fun::v2v::abs<mln_value(I)>());

      trace::exiting("data::abs");
    }

    template <typename I>
    inline
    void abs_inplace(Image<I>& input)
    {
      trace::entering("data::abs_inplace");

      mln_precondition(exact(input).is_valid());
      data::apply(input, fun::v2v::abs<mln_value(I)>());

      trace::exiting("data::abs_inplace");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::data

} // end of namespace mln


#endif // ! MLN_DATA_ABS_HH
