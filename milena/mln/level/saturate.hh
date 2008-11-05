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

#ifndef MLN_LEVEL_SATURATE_HH
# define MLN_LEVEL_SATURATE_HH

/*! \file mln/level/saturate.hh
 *
 * \brief Apply a saturation function to image pixel values.
 */

# include <mln/fun/v2v/saturate.hh>
# include <mln/level/apply.hh>
# include <mln/level/transform.hh>


namespace mln
{

  namespace level
  {


    /*! Apply the saturate function to image pixel values.
     *
     * \param[in] input The input image.
     * \param[out] output The output image.
     *
     * The saturation is based on the min and max values of the output
     * value type.  This assumes that the range of values in the input
     * image is larger than the one of the output image.
     *
     * \pre \p input and \p output have to own the same domain.
     *
     */
    template <typename I, typename O>
    void saturate(const Image<I>& input, Image<O>& output);


    /*! Apply the saturate function to image pixel values.
     *
     * \param[in] input The input image.
     * \param[in] min The minimum output value.
     * \param[in] max The maximum output value.
     * \param[out] output The output image.
     *
     * \pre \p input and \p output have to own the same domain.
     *
     */
    template <typename I, typename O>
    void saturate(const Image<I>& input,
		  const mln_value(O)& min, const mln_value(O)& max,
		  Image<O>& output);


    /*! Apply the saturate function to image pixel values.
     *
     * \param[in,out] input The input image.
     * \param[in] min The minimum output value.
     * \param[in] max The maximum output value
     *
     * \pre \p input has to be initialized.
     *
     */
    template <typename I>
    void saturate_inplace(Image<I>& input,
			  const mln_value(I)& min, const mln_value(I)& max);


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename O>
    inline
    void saturate(const Image<I>& input, Image<O>& output)
    {
      trace::entering("level::saturate");

      mln_precondition(exact(input).domain() == exact(output).domain());

      fun::v2v::saturate<mln_value(O)> f;
      output = level::transform(input, f);

      trace::exiting("level::saturate");
    }

    template <typename I, typename O>
    inline
    void saturate(const Image<I>& input,
		  const mln_value(O)& min, const mln_value(O)& max,
		  Image<O>& output)
    {
      trace::entering("level::saturate");

      mln_precondition(exact(input).domain() == exact(output).domain());

      fun::v2v::saturate<mln_value(O)> f(min, max);
      output = level::transform(input, f);

      trace::exiting("level::saturate");
    }

    template <typename I>
    inline
    void saturate_inplace(Image<I>& input,
			  const mln_value(I)& min, const mln_value(I)& max)
    {
      trace::entering("level::saturate_inplace");

      mln_precondition(exact(input).has_data());

      fun::v2v::saturate<mln_value(I)> f(min, max);
      level::apply(input, f);

      trace::exiting("level::saturate_inplace");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::level

} // end of namespace mln


#endif // ! MLN_LEVEL_SATURATE_HH
