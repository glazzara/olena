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

#ifndef MLN_BORDER_DUPLICATE_HH
# define MLN_BORDER_DUPLICATE_HH

/*! \file mln/border/duplicate.hh
 *
 * \brief Define function that fills border using nearest pixels.
 */

# include <mln/core/concept/image.hh>
# include <mln/level/memset_.hh>
# include <mln/core/line_piter.hh>
# include <mln/core/pixel.hh>


namespace mln
{

  namespace border
  {

    /*! Assign the virtual (outer) border of image \p ima with the
     *  dupplicate of the inner border of this image.
     *
     * \param[in,out] ima The image whose border is to be duplicated.
     *
     * \pre \p ima has to be initialized.
     *
     * \todo Optimize with memcpy if possible.
     */
    template <typename I>
    void duplicate(const Image<I>& ima);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I>
      inline
      void duplicate_1d_(const I& ima)
      {
	trace::entering("border::impl::duplicate_1d_");

	typedef mln_psite(I) P;
	typename I::line_piter pl(ima.domain());
 	std::size_t len_c = exact(ima).bbox().len(P::dim - 1);
 	std::size_t border = ima.border ();

	for (std::size_t i = 0; i < border; ++i)
	  const_cast<I&>(ima)[i] = ima[border];

	std::size_t st = border + len_c - 1;
	for (std::size_t i = st + 1; i < ima.nelements (); ++i)
	  const_cast<I&>(ima)[i] = ima[st];

	trace::exiting("border::impl::duplicate_1d_");
      }

      template <typename I>
      inline
      void duplicate_2d_(const I& ima)
      {
	trace::entering("border::impl::duplicate_2d_");

	typedef mln_psite(I) P;
	typename I::line_piter pl(ima.domain());
 	std::size_t border = ima.border ();
 	std::size_t border_2x = 2 * ima.border ();
 	std::size_t len_c = exact(ima).bbox().len(1);
 	std::size_t len_r = exact(ima).bbox().len(0);
 	std::size_t real_len_c = len_c + border_2x;
 	std::size_t st;

	// Duplicate
	for_all (pl)
	  {
 	    st = ima.index_of_point (pl);
	    for (std::size_t i = 1; i <= border; ++i)
	      const_cast<I&>(ima)[st - i] = ima[st];
	    st = st + len_c - 1;
	    for (std::size_t i = 1; i <= border; ++i)
	      const_cast<I&>(ima)[st + i] = ima[st];
 	  }

	// Duplicate n first * border line
	st = real_len_c * border;
	for (std::size_t k = 0; k < border; ++k)
	  for (std::size_t i = 0; i < real_len_c; ++i)
	    const_cast<I&>(ima)[k * real_len_c + i] = ima[st + i];

	// Duplicate n last * border line
	st = real_len_c * (border + len_r - 1);
	for (std::size_t k = 1; k <= border; ++k)
	  for (std::size_t i = st; i < st + real_len_c; ++i)
	    const_cast<I&>(ima)[k * real_len_c + i] = ima[i];

	trace::exiting("border::impl::duplicate_2d_");
      }

      template <typename I>
      inline
      void duplicate_3d_(const Image<I>& ima_)
      {
	trace::entering("border::impl::duplicate_3d_");

	const I& ima = exact(ima_);
	mln_precondition(ima.has_data());

	typedef mln_psite(I) P;
	typename I::line_piter pl(ima.domain());
 	std::size_t border = ima.border ();
 	std::size_t border_2x = 2 * ima.border ();
 	std::size_t len_c = exact(ima).bbox().len(P::dim - 1);
 	std::size_t len_r = exact(ima).bbox().len(1);
 	std::size_t len_s = exact(ima).bbox().len(0);
 	std::size_t real_len_c = len_c + border_2x;
 	std::size_t real_len_r = len_r + border_2x;
	std::size_t face = real_len_c * real_len_r;
	std::size_t st;

	pl.start ();

	for (std::size_t k = 0; k < len_s; ++k)
	  {

 	    // Duplicate
	    for (std::size_t j = 0; j < len_r; ++j)
	      {
		st = ima.index_of_point (pl);
		for (std::size_t i = 1; i <= border; ++i)
		  const_cast<I&>(ima)[st - i] = ima[st];
		st = st + len_c - 1;
		for (std::size_t i = 1; i <= border; ++i)
		  const_cast<I&>(ima)[st + i] = ima[st];
		pl.next ();
	      }

	    // Duplicate n last * border line
	    st = border * face + k * face + border * real_len_c ;
	    for (std::size_t j = 1; j <= border; ++j)
	      for (std::size_t i = 0; i < real_len_c; ++i)
		const_cast<I&>(ima)[st - j * real_len_c + i] = ima[st + i];

	    // Duplicate n last * border line
	    st = border * face + k * face + (len_r + border - 1) * real_len_c ;
	    for (std::size_t j = 1; j <= border; ++j)
	      for (std::size_t i = 0; i < real_len_c; ++i)
		const_cast<I&>(ima)[st + j * real_len_c + i] = ima[st + i];
	  }

	// Duplicate n first * border face
	st = border * face;
	for (std::size_t k = 0; k < border; ++k)
	  for (std::size_t i = 0; i < face; ++i)
	    const_cast<I&>(ima)[k * face + i] = ima[st + i];

	// Duplicate n last * border face
	st = (len_s + border - 1) * face;
	for (std::size_t k = 1; k <= border; ++k)
	  for (std::size_t i = 0; i < face; ++i)
	    const_cast<I&>(ima)[st + k * face + i] = ima[st + i];

	trace::exiting("border::impl::duplicate_3d_");
      }

    } // end of namespace mln::border::impl


    // Facade.

    template <typename I>
    inline
    void duplicate(const Image<I>& ima_)
    {
      trace::entering("border::duplicate");

      const I& ima = exact(ima_);
      mln_precondition(ima.has_data());

      mlc_is(mln_trait_image_speed(I), trait::image::speed::fastest)::check();

      typedef mln_psite(I) P;

      if (!ima.border ())
	return;

      if (P::dim == 1)
	impl::duplicate_1d_(ima);
      if (P::dim == 2)
	impl::duplicate_2d_(ima);
      if (P::dim == 3)
	impl::duplicate_3d_(ima);

      trace::exiting("border::duplicate");
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::border

} // end of namespace mln


#endif // ! MLN_BORDER_DUPLICATE_HH
