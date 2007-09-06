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

#ifndef MLN_LEVEL_MEMSET_HH
# define MLN_LEVEL_MEMSET_HH

/*! \file mln/level/memset_.hh
 *
 * \brief Same as std::memset but for fast images.
 */

# include <mln/core/concept/fast_image.hh>
# include <mln/core/pixel.hh>


namespace mln
{

  namespace level
  {

    /*! Set \p n pixels at value \p v starting from pixel \p p.
     *
     * \param[in,out] pix The first pixel to set.
     * \param[in] v The value to set pixels with.
     * \param[in] n The number of pixels to set.
     *
     * \pre FIXME: !
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
     * \pre \p input has to be initialized.  FIXME: More.
     */
    template <typename I>
    void memset_(Fast_Image<I>& input, const mln_point(I)& p,
		 const mln_value(I)& v, std::size_t n);


# ifndef MLN_INCLUDE_ONLY

    template <typename P>
    void memset_(Generalized_Pixel<P>& pix_,
		 const mln_value(P)& v, std::size_t n)
    {
      typedef mln_image(P) I;
      // FIXME: metal::is_not_const<I>::check();
      
      P& pix = internal::force_exact<P>(pix_);
      mln_precondition(pix.ima().has_data());
      mln_precondition(& pix.val() >= & pix.ima()[0]);
      mln_precondition(& pix.val() < & pix.ima()[0] + pix.ima().ncells());
      mln_precondition(& pix.val() + n <= & pix.ima()[0] + pix.ima().ncells());
      
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
	  std::memset((void*)(& pix.val()),
		      *(const int*)(&v), // violent cast
		      n);
	}
      else
	{
	  mln_value(I)* ptr = & pix.val();
	  for (std::size_t i = 0; i < n; ++i)
	    *ptr++ = v;
	}
    }

    template <typename I>
    void memset_(Fast_Image<I>& input_, const mln_point(I)& p,
		 const mln_value(I)& v, std::size_t n)
    {
      I& input = exact(input_);
      mln_precondition(input.has_data());
      mln_precondition(input.owns_(p));
      mln_precondition(input.offset_at(p) + n <= input.ncells());

      pixel<I> pix(input, p);
      memset_(pix, v, n);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::level

} // end of namespace mln


#endif // ! MLN_LEVEL_MEMSET_HH
