// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_LEVEL_CONVERT_HH
# define MLN_LEVEL_CONVERT_HH

/// \file mln/level/convert.hh
///
/// Convert the contents of an image into another one.
///
/// \todo Re-write doc.

# include <mln/fun/v2v/convert.hh>
# include <mln/level/transform.hh>


namespace mln
{

  namespace level
  {

    /*! Convert the image \p input by changing the value type.
     *
     * \param[in] v A value of the destination type.
     * \param[in] input The input image.
     * \param[out] output The result image.
     */
    template <typename V, typename I>
    mln_ch_value(I, V)
    convert(const V&, const Image<I>& input);



# ifndef MLN_INCLUDE_ONLY

    // Facade.

    template <typename V, typename I>
    inline
    mln_ch_value(I, V)
    convert(const V&, const Image<I>& input)
    {
      trace::entering("level::convert");

      mln_precondition(exact(input).has_data());
      fun::v2v::convert<V> f;
      mln_ch_value(I, V) output = level::transform(input, f);

      trace::exiting("level::convert");
      return output;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::level

} // end of namespace mln


#endif // ! MLN_LEVEL_CONVERT_HH
