// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_LEVEL_TO_ENC_HH
# define MLN_LEVEL_TO_ENC_HH

/*! \file
 *
 * \brief Transform with fun::v2v::enc the contents of an image into
 * another one.
 */

# include <mln/data/transform.hh>
# include <mln/fun/v2v/enc.hh>


namespace mln
{

  namespace data
  {

    /*! Set the \p output image with the encoding values of the image \p input pixels.
     *
     * \param[in] input The input image.
     * \param[out] output The result image.
     *
     * \pre \p output.domain >= \p input.domain
     */
    template <typename I, typename O>
    void to_enc(const Image<I>& input, Image<O>& output);


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename O>
    inline
    void to_enc(const Image<I>& input, Image<O>& output)
    {
      trace::entering("data::to_enc");

      mln_precondition(exact(output).domain() == exact(input).domain());
      output = data::transform(input, fun::v2v::enc< mln_value(I) >());

      trace::exiting("data::to_enc");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::data

} // end of namespace mln


#endif // ! MLN_LEVEL_TO_ENC_HH
