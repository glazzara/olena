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

#ifndef MLN_LABELING_FOREGROUND_HH
# define MLN_LABELING_FOREGROUND_HH

/*! \file mln/labeling/foreground.hh
 *
 * \brief Connected component labeling of the object part in a binary
 * image.
 */

# include <mln/labeling/level.hh>


namespace mln
{

  namespace labeling
  {

    /*! Connected component labeling of the object part in a binary
     * image.
     *
     * \param[in]  input  The input image.
     * \param[in]  nbh    The neighborhood to consider.
     * \param[out] output The label image.
     * \param[out] nlabels The number of labels.
     *
     * \return The number of labels.
     */
    template <typename I, typename N, typename O>
    bool foreground(const Image<I>& input, const Neighborhood<N>& nbh,
		    Image<O>& output,
		    unsigned& nlabels);


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename N, typename O>
    bool foreground(const Image<I>& input, const Neighborhood<N>& nbh,
		    Image<O>& output,
		    unsigned& nlabels)
    {
      mln_precondition(exact(output).domain() == exact(input).domain());
      return labeling::level(input, true, nbh, output, nlabels);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::labeling

} // end of namespace mln


#endif // ! MLN_LABELING_FOREGROUND_HH
