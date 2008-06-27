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
 *
 */

# include <mln/core/concept/image.hh>
# include <mln/core/inplace.hh>


// Specializations are in:
# include <mln/level/paste.spe.hh>



namespace mln
{

  namespace level
  {

    /*!
     *  \brief Paste the contents of image \p data into the image \p
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

      namespace generic
      {

	template <typename I, typename J>
	inline
	void paste_(const I& data, J& destination)
	{
	  trace::entering("level::impl::generic::paste_");

	  mln_piter(I) p(data.domain());
	  for_all(p)
	    destination(p) = data(p);

	  trace::exiting("level::impl::generic::paste_");
	}

      } // end of namespace mln::level::impl::generic

    } // end of namespace mln::level::impl


    // Facade.

    template <typename I, typename J>
    inline
    void paste(const Image<I>& data_, Image<J>& destination_)
    {
      trace::entering("level::paste");

      const I& data  = exact(data_);
      J& destination = exact(destination_);

      mlc_is(mln_trait_image_value_io(J), trait::image::value_io::read_write)::check();
      mlc_converts_to(mln_value(I), mln_value(J))::check();
      mln_precondition(data.domain() <= destination.domain());

      impl::paste_(mln_trait_image_value_storage(I)(), data,
		   mln_trait_image_value_storage(J)(), destination);

      trace::exiting("level::paste");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::level

} // end of namespace mln



#endif // ! MLN_LEVEL_PASTE_HH
