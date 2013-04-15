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

#ifndef MLN_LABELING_FOREGROUND_HH
# define MLN_LABELING_FOREGROUND_HH

/// \file
///
/// Connected component labeling of the object part in a binary
/// image.

# include <mln/labeling/value.hh>


namespace mln
{

  namespace labeling
  {

    /*! Connected component labeling of the object part in a binary
     * image.
     *
     * \param[in]  input    The input image.
     * \param[in]  nbh      The connexity of the foreground.
     * \param[out] nlabels  The number of labels.
     * \return              The label image.
     *
     * \pre The input image has to be binary (checked at compile-time).
     *
     * This routine actually calls mln::labeling::value with the
     * value set to \c true.
     *
     * \see mln::labeling::value
     */
    template <typename I, typename N, typename L>
    mln_ch_value(I, L)
    foreground(const Image<I>& input, const Neighborhood<N>& nbh,
	       L& nlabels);


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename N, typename L>
    inline
    mln_ch_value(I, L)
    foreground(const Image<I>& input, const Neighborhood<N>& nbh,
	       L& nlabels)
    {
      mln_trace("labeling::foreground");

      mlc_equal(mln_trait_image_kind(I),
		mln::trait::image::kind::binary)::check();
      mln_precondition(exact(input).is_valid());
      mln_precondition(exact(nbh).is_valid());

      mln_ch_value(I, L) output = labeling::value(input, true, nbh, nlabels);

      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::labeling

} // end of namespace mln


#endif // ! MLN_LABELING_FOREGROUND_HH
