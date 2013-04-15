// Copyright (C) 2007, 2008, 2009, 2011, 2012 EPITA Research and
// Development Laboratory (LRDE)
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

#ifndef MLN_BORDER_DUPLICATE_HH
# define MLN_BORDER_DUPLICATE_HH

/// \file
///
/// Define function that fills border using nearest pixels.

# include <mln/core/concept/image.hh>
# include <mln/core/routine/primary.hh>
# include <mln/core/box_runstart_piter.hh>
# include <mln/border/get.hh>
# include <mln/opt/element.hh>


namespace mln
{

  namespace border
  {

    /*! \brief Assign the virtual (outer) border of image \p ima with the
     *  duplicate of the inner border of this image.
     *
     * \param[in,out] ima The image whose border is to be duplicated.
     *
     * \pre \p ima has to be initialized.
     *
     * \todo Optimize with memcpy if possible.
     *
     * \ingroup mlnborderext
     */
    template <typename I>
    void duplicate(const Image<I>& ima);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I>
      inline
      void duplicate_1D(I& ima)
      {
	mln_trace("border::impl::duplicate_1D");

	typedef mln_psite(I) P;
	mln_box_runstart_piter(I) pl(ima.domain());
 	unsigned len_c = ima.bbox().len(P::dim - 1);
 	unsigned border = ima.border();

	for (unsigned i = 0; i < border; ++i)
          opt::element(ima, i) = opt::element(ima, border);

	unsigned st = border + len_c - 1;
	for (unsigned i = st + 1; i < opt::nelements(ima); ++i)
          opt::element(ima, i) = opt::element(ima, st);

      }

      template <typename I>
      inline
      void duplicate_2D(I& ima)
      {
	mln_trace("border::impl::duplicate_2D");

	typedef mln_psite(I) P;
	mln_box_runstart_piter(I) pl(ima.domain());
 	unsigned border = ima.border();
 	unsigned border_2x = 2 * ima.border();
 	unsigned len_c = ima.bbox().len(1);
 	unsigned len_r = ima.bbox().len(0);
 	unsigned real_len_c = len_c + border_2x;
 	unsigned st;

	// Duplicate
	for_all (pl)
	  {
 	    st = ima.offset_of_point (pl);
	    for (unsigned i = 1; i <= border; ++i)
              opt::element(ima, st - i) = opt::element(ima, st);
	    st = st + len_c - 1;
	    for (unsigned i = 1; i <= border; ++i)
              opt::element(ima, st + i) = opt::element(ima, st);
 	  }

	// Duplicate n first * border line
	st = real_len_c * border;
	for (unsigned k = 0; k < border; ++k)
	  for (unsigned i = 0; i < real_len_c; ++i)
            opt::element(ima, k * real_len_c + i) = opt::element(ima, st + i);

	// Duplicate n last * border line
	st = real_len_c * (border + len_r - 1);
	for (unsigned k = 1; k <= border; ++k)
	  for (unsigned i = st; i < st + real_len_c; ++i)
            opt::element(ima, k * real_len_c + i) = opt::element(ima, i);

      }

      template <typename I>
      inline
      void duplicate_3D(I& ima)
      {
	mln_trace("border::impl::duplicate_3D");

	mln_precondition(ima.is_valid());

	typedef mln_psite(I) P;
	mln_box_runstart_piter(I) pl(ima.domain());
 	unsigned border = ima.border();
 	unsigned border_2x = 2 * ima.border();
 	unsigned len_c = ima.bbox().len(P::dim - 1);
 	unsigned len_r = ima.bbox().len(1);
 	unsigned len_s = ima.bbox().len(0);
 	unsigned real_len_c = len_c + border_2x;
 	unsigned real_len_r = len_r + border_2x;
	unsigned face = real_len_c * real_len_r;
	unsigned st;

	pl.start();

	for (unsigned k = 0; k < len_s; ++k)
	  {

 	    // Duplicate
	    for (unsigned j = 0; j < len_r; ++j)
	      {
		st = ima.offset_of_point (pl);
		for (unsigned i = 1; i <= border; ++i)
                  opt::element(ima, st - i) = opt::element(ima, st);
		st = st + len_c - 1;
		for (unsigned i = 1; i <= border; ++i)
                  opt::element(ima, st + i) = opt::element(ima, st);
		pl.next();
	      }

	    // Duplicate n last * border line
	    st = border * face + k * face + border * real_len_c ;
	    for (unsigned j = 1; j <= border; ++j)
	      for (unsigned i = 0; i < real_len_c; ++i)
                opt::element(ima, st - j * real_len_c + i) =
                  opt::element(ima, st + i);

	    // Duplicate n last * border line
	    st = border * face + k * face + (len_r + border - 1) * real_len_c ;
	    for (unsigned j = 1; j <= border; ++j)
	      for (unsigned i = 0; i < real_len_c; ++i)
                opt::element(ima, st + j * real_len_c + i) =
                  opt::element(ima, st + i);
	  }

	// Duplicate n first * border face
	st = border * face;
	for (unsigned k = 0; k < border; ++k)
	  for (unsigned i = 0; i < face; ++i)
            opt::element(ima, k * face + i) = opt::element(ima, st + i);

	// Duplicate n last * border face
	st = (len_s + border - 1) * face;
	for (unsigned k = 1; k <= border; ++k)
	  for (unsigned i = 0; i < face; ++i)
            opt::element(ima, st + k * face + i) = opt::element(ima, st + i);

      }

    } // end of namespace mln::border::impl


    namespace internal
    {

      template <typename I>
      void duplicate_dispatch_on(metal::int_<1>, I& ima)
      {
	impl::duplicate_1D(ima);
      }

      template <typename I>
      void duplicate_dispatch_on(metal::int_<2>, I& ima)
      {
	impl::duplicate_2D(ima);
      }

      template <typename I>
      void duplicate_dispatch_on(metal::int_<3>, I& ima)
      {
	impl::duplicate_3D(ima);
      }

      template <typename I>
      void duplicate_dispatch_on(trait::image::speed::fastest,
				 const Image<I>& ima)
      {
	typedef mln_site(I) P;
	duplicate_dispatch_on(metal::int_<P::dim>(),
			      const_cast<I&>(exact(ima)));
      }

      template <typename I>
      void duplicate_dispatch_on(trait::image::speed::any,
				 const Image<I>& ima)
      {
	(void) ima;
	// No-op.
      }

      template <typename I>
      void duplicate_dispatch_on(const Image<I>& ima)
      {
	duplicate_dispatch_on(mln_trait_image_speed(I)(),
			      ima);
      }

      template <typename I>
      void duplicate_dispatch(const Image<I>& ima)
      {
	duplicate_dispatch_on(primary(ima));
      }

    } // end of namespace mln::border::internal


    // Facade.

    template <typename I>
    void duplicate(const Image<I>& ima)
    {
      mln_trace("border::duplicate");
      mln_precondition(exact(ima).is_valid());

      if (border::get(ima) != 0)
	internal::duplicate_dispatch(ima);

    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::border

} // end of namespace mln


#endif // ! MLN_BORDER_DUPLICATE_HH
