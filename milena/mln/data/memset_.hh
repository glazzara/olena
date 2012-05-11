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

#ifndef MLN_DATA_MEMSET__HH
# define MLN_DATA_MEMSET__HH

/// \file
///
/// Same as std::memset but for fastest images.

# include <cstring>

# include <mln/core/concept/image.hh>
# include <mln/core/pixel.hh>
# include <mln/metal/is_not_const.hh>
# include <mln/opt/element.hh>



namespace mln
{

  namespace data
  {

    /*! Set \p n pixels at value \p v starting from pixel \p p.
     *
     * \param[in,out] pix The first pixel to set.
     * \param[in] v The value to set pixels with.
     * \param[in] n The number of pixels to set.
     *
     * \pre \p pix image has to be initialized.
     * \pre \p pix has to be on \p pix image domain.
     * \pre \p pix + \p n has to be on \p pix image domain.
     */
    template <typename P>
    void memset_(Generalized_Pixel<P>& pix,
		 const mln_value(P)& v, std::size_t n);

    /*! Set \p n points of image \p ima at value \p v starting from
     *  point \p p.
     *
     * \param[in,out] input The image.
     * \param[in] p The first point to set.
     * \param[in] v The value to set to points.
     * \param[in] n The number of points to set.
     *
     * \pre \p input image has to be initialized.
     * \pre \p input has to own \p p.
     * \pre \p p + \p n is <= \p input size.
     */
    template <typename I>
    void memset_(I& input, const mln_psite(I)& p,
		 const mln_value(I)& v, std::size_t n);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename P>
      inline
      void memset__(P& pix, const mln_value(P)& v, std::size_t n)
      {
	trace::entering("data::impl::memset_");

	typedef mln_image(P) I;
	if (n == 0)
	  {
	    return; // no-op
	  }

	if (n == 1)
	  {
	    pix.val() = v; // one assignment
	    return;
	  }

	if (sizeof(mln_value(I)) == 1)
	  {
	    /* Because of strict-aliasing rules, we cannot use

	         *(const int*)(&v)

	       as second argument for std::memset.  Hence the use of
	       std::memcpy. See the `-fstrict-aliasing' entry in GCC's
	       documentation for more information.  */
	    char c;
	    std::memcpy(&c, &v, 1);
	    std::memset((void*)(& pix.val()), c, n);
	  }
	else
	  {
	    mln_value(I)* ptr = & pix.val();
	    for (std::size_t i = 0; i < n; ++i)
	      *ptr++ = v;
	  }

	trace::exiting("data::impl::memset_");
      }

    } // end of namespace mln::data::impl


    template <typename P>
    inline
    void memset_(Generalized_Pixel<P>& pix_,
		 const mln_value(P)& v, std::size_t n)
    {
      trace::entering("data::memset_");

      typedef mln_image(P) I;
      metal::is_not_const<I>::check();

      P& pix = mln::internal::force_exact<P>(pix_);
      mln_precondition(pix.ima().is_valid());
      mln_precondition(& pix.val() >= & pix.ima()[0]);
      mln_precondition(& pix.val() < & pix.ima()[0] +
                       opt::nelements(pix.ima()));
      mln_precondition(& pix.val() + n <= & pix.ima()[0] +
                       opt::nelements(pix.ima()));

      impl::memset__(pix, v, n);

      trace::exiting("data::memset_");
    }

    template <typename I>
    inline
    void memset_(I& input, const mln_psite(I)& p,
		 const mln_value(I)& v, std::size_t n)
    {
      trace::entering("data::memset_");

      mlc_is(mln_trait_image_speed(I), trait::image::speed::fastest)::check();

      mln_precondition(input.is_valid());
      mln_precondition(input.has(p));
      mln_precondition(input.offset_of_point(p) + n <= opt::nelements(input));

      pixel<I> pix(input, p);
      impl::memset__(pix, v, n);

      trace::exiting("data::memset_");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::data

} // end of namespace mln


#endif // ! MLN_DATA_MEMSET__HH
