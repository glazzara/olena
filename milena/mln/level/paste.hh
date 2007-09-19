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

#ifndef MLN_LEVEL_PASTE_HH
# define MLN_LEVEL_PASTE_HH

/*! \file mln/level/paste.hh
 *
 * \brief Paste the contents of an image into another one.
 */

# include <mln/core/concept/image.hh>
# include <mln/core/inplace.hh>
# include <mln/level/memcpy_.hh>


namespace mln
{

  namespace level
  {

    /*! Paste the contents of image \p data into the image \p
     *  destination.
     *
     * \param[in] data The input image providing pixels values.
     * \param[in,out] destination The image in which values are
     * assigned.
     *
     * This routine runs: \n
     * for all p of \p data, \p destination(p) = \p data(p).
     *
     * \warning The definition domain of \p data has to be included in
     * the one of \p destination; so using mln::safe_image does not 
     * make pasting outside the destination domain work.
     *
     * \pre \p data.domain <= \p destination.domain
     */
    template <typename I, typename J>
    void paste(const Image<I>& data, Image<J>& destination);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I, typename J>
      void paste_generic_(const Image<I>& data_, Image<J>& destination_)
      {
	const I& data  = exact(data_);
	J& destination = exact(destination_);

	mln_piter(I) p(data.domain());
	for_all(p)
	  destination(p) = data(p);
      }

      template <typename I, typename J>
      void paste_fast_(const Fast_Image<I>& data_, Fast_Image<J>& destination_)
      {
	const I& data  = exact(data_);
	J& destination = exact(destination_);

	typedef mln_point(I) P;
	std::size_t n = data.bbox().len(P::dim - 1);

	typename I::line_piter p(data.domain());
	for_all(p)
	  memcpy_(inplace(make::pixel(destination, p)),
		  make::pixel(data, p),
		  n);
      }

      // Disjunction.

      template <typename I, typename J>
      void paste_(const Image<I>& data, Image<J>& destination)
      {
	paste_pw_(data, destination);
      }

      template <typename I, typename J>
      void paste_(const Fast_Image<I>& data, Fast_Image<J>& destination)
      {
	if (sizeof(mln_value(I)) == sizeof(mln_value(J)))
	  paste_fast_(data, destination);
	else
	  paste_generic_(data, destination);
      }

    } // end of namespace mln::level::impl


    // Facade.

    template <typename I, typename J>
    void paste(const Image<I>& data, Image<J>& destination)
    {
      mln_precondition(exact(data).domain() <= exact(destination).domain());
      impl::paste_(exact(data), exact(destination));
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::level

} // end of namespace mln


#endif // ! MLN_LEVEL_PASTE_HH
