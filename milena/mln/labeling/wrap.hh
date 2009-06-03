// Copyright (C) 2009 EPITA Research and Development Laboratory
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

#ifndef MLN_LABELING_WRAP_HH
# define MLN_LABELING_WRAP_HH

/// \file mln/labeling/wrap.hh
///
/// Wrap labels such as 0 -> 0 and [1, lmax] maps to [1,
/// Lmax] (using modulus).

# include <mln/core/concept/image.hh>
# include <mln/data/transform.hh>
# include <mln/fun/v2v/wrap.hh>
# include <mln/metal/converts_to.hh>
# include <mln/metal/is_a.hh>
# include <mln/value/label_8.hh>

namespace mln
{

  namespace labeling
  {

    /// Wrap labels such as 0 -> 0 and [1, lmax] maps to [1,
    /// Lmax] (using modulus).
    ///
    /// \param[in] value_type The type used to wrap the label type.
    /// \param[in] input The label image.
    ///
    /// \return A new image with values wrapped with type V.
    template <typename V, typename I>
    mln_ch_value(I,V)
    wrap(const V& value_type, const Image<I>& input);

    /// Wrap labels such as 0 -> 0 and [1, lmax] maps to [1,
    /// Lmax] (using modulus). Use label_8 as label type.
    ///
    /// \param[in] input The label image.
    ///
    /// \return A new image with values wrapped with type label_8.
    template <typename I>
    mln_ch_value(I,value::label_8)
    wrap(const Image<I>& input);


# ifndef MLN_INCLUDE_ONLY

    template <typename V, typename I>
    inline
    mln_ch_value(I,V)
    wrap(const V& value_type, const Image<I>& input)
    {
      trace::entering("labeling::wrap");

//      mlc_is_a(mln_value(I), value::Symbolic)::check();
      mln_precondition(exact(input).is_valid());
      (void) value_type;

      mln_ch_value(I,V) output = data::transform(input, fun::v2v::wrap<V>());

      trace::exiting("labeling::wrap");
      return output;
    }


    template <typename I>
    inline
    mln_ch_value(I,value::label_8)
    wrap(const Image<I>& input)
    {
      return wrap(value::label_8(), input);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::labeling

} // end of namespace mln

#endif // ! MLN_LABELING_WRAP_HH
