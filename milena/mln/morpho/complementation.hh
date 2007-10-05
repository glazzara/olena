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

#ifndef MLN_MORPHO_COMPLEMENTATION_HH
# define MLN_MORPHO_COMPLEMENTATION_HH

# include <mln/level/compare.hh>
# include <mln/logical/not.hh>
# include <mln/arith/revert.hh>


namespace mln
{

  namespace morpho
  {

    /*! Morphological complementation: either a logical "not" (if
     *  morpho on sets) or an arithmetical complementation (if morpho
     *  on functions).
     */
    template <typename I, typename O>
    void complementation(const Image<I>& input, Image<O>& output);


    /*! Morphological complementation, inplace version: either a
     *  logical "not" (if morpho on sets) or an arithmetical
     *  complementation (if morpho on functions).
     */
    template <typename I>
    void complementation_inplace(Image<I>& input);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I, typename O>
      void complementation_(trait::image::kind::logic, // binary => morphology on sets
			    const Image<I>& input,
			    Image<O>& output)
      {
	return logical::not_(input, output);
      }

      template <typename K, typename I, typename O>
      void complementation_(K, // otherwise => morphology on functions
			    const Image<I>& input,
			    Image<O>& output)
      {
	return arith::revert(input, output);
      }

    } // end of namespace mln::morpho::impl


    // Facades.

    template <typename I, typename O>
    void complementation(const Image<I>& input, Image<O>& output)
    {
      mln_precondition(exact(output).domain() == exact(input).domain());
      impl::complementation_(mln_trait_image_kind(I)(), exact(input), output);
    }

    template <typename I>
    void complementation_inplace(Image<I>& input)
    {
      mln_precondition(exact(input).has_data());
      morpho::complementation(input, input); // Calls the previous version.
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_COMPLEMENTATION_HH
