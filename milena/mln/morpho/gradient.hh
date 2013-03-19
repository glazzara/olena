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

#ifndef MLN_MORPHO_GRADIENT_HH
# define MLN_MORPHO_GRADIENT_HH

/// \file
///
/// Morphological gradient.
///
/// It is based on dilation and erosion so that handling side effects
/// (with extension domains) is easy.  It does *not* rely on
/// morpho::general (what the extension initialization would be in
/// that case?!)
///
/// \todo Save memory.

# include <mln/morpho/includes.hh>


namespace mln
{

  namespace morpho
  {

    /*! \brief Morphological gradient.
     *
     *  This operator is d_B - e_B.
     *
     *  \ingroup mlnmorpho
     */
    template <typename I, typename W>
    mln_concrete(I) gradient(const Image<I>& input, const Window<W>& win);


    /*! \brief Morphological internal gradient.
     *
     *  This operator is Id - e_B.
     *
     * \ingroup mlnmorpho
     */
    template <typename I, typename W>
    mln_concrete(I) gradient_internal(const Image<I>& input, const Window<W>& win);


    /*! \brief Morphological external gradient.
     *
     *  This operator is d_B - Id.
     *
     * \ingroup mlnmorpho
     */
    template <typename I, typename W>
    mln_concrete(I) gradient_external(const Image<I>& input, const Window<W>& win);



# ifndef MLN_INCLUDE_ONLY


    template <typename I, typename W>
    inline
    mln_concrete(I) gradient(const Image<I>& input, const Window<W>& win)
    {
      mln_trace("morpho::gradient");
      mln_precondition(exact(input).is_valid());
      mln_precondition(! exact(win).is_empty());

      mln_concrete(I) output = morpho::minus(dilation(input, win),
					     erosion(input, win));

      mln_postcondition(test::positive(output));
      return output;
    }


    template <typename I, typename W>
    inline
    mln_concrete(I) gradient_internal(const Image<I>& input, const Window<W>& win)
    {
      mln_trace("morpho::gradient_internal");
      mln_precondition(exact(input).is_valid());
      mln_precondition(! exact(win).is_empty());

      mln_concrete(I) output = morpho::minus(input,
					     erosion(input, win));

      mln_postcondition(test::positive(output));
      return output;
    }


    template <typename I, typename W>
    inline
    mln_concrete(I) gradient_external(const Image<I>& input, const Window<W>& win)
    {
      mln_trace("morpho::gradient_external");
      mln_precondition(exact(input).is_valid());
      mln_precondition(! exact(win).is_empty());

      mln_concrete(I) output = morpho::minus(dilation(input, win),
					     input);

      mln_postcondition(test::positive(output));
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_GRADIENT_HH
