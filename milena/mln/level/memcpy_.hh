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

#ifndef MLN_LEVEL_MEMCPY__HH
# define MLN_LEVEL_MEMCPY__HH

/*! \file mln/level/memcpy_.hh
 *
 * \brief Same as std::memcpy but for fastest images.
 *
 * \todo Overload with images and points; Cf. memset_.
 */

# include <mln/core/concept/image.hh>
# include <mln/core/pixel.hh>
# include <mln/metal/is_not_const.hh>

namespace mln
{

  namespace level
  {

    /*! Copy \p n pixels starting from pixel \p src to destination
     *  starting from pixel \p dest.
     *
     * \param[in,out] dest The destination pixel (to set values).
     * \param[in] src The source pixel (to get values).
     * \param[in] n The number of pixels to copy.
     *
     * \pre \p src has to be initialized.
     * \pre \p dest has to be initialized.
     * \pre \p src and dest image values have to own the same sizeof.
     * \pre \p dest pixel has to be on \p dest image domain.
     * \pre \p src pixel has to be on \p src image domain.
     * \pre \p dest + \p n has to be on \p dest image domain.
     * \pre \p src + \p  n has to be on \p src image domain.
     */
    template <typename Pd, typename Ps>
    void memcpy_(Generalized_Pixel<Pd>& dest, const Generalized_Pixel<Ps>& src,
		 std::size_t n);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename Pd, typename Ps>
      void memcpy__(Pd& dest, const Ps& src, std::size_t n)
      {
	trace::entering("level::impl::memcpy__");

	typedef mln_image(Pd) Id;
	typedef mln_image(Ps) Is;

	if (n == 0)
	  {
	    return; // no-op
	  }

	if (n == 1)
	  {
	    dest.val() = src.val(); // one assignment
	    return;
	  }

	if (sizeof(mln_value(Id)) == 1)
	  {
	    std::memcpy((      void*)(& dest.val()), // violent casts
			(const void*)(&  src.val()),
			n);
	  }
	else
	  {
	    mln_value(Id)* p_d = & dest.val();
	    const mln_value(Is)* p_s = & src.val();
	    for (std::size_t i = 0; i < n; ++i)
	      *p_d++ = *p_s++;
	  }

	trace::exiting("level::impl::memcpy__");
      }

    }

    template <typename Pd, typename Ps>
    void memcpy_(Generalized_Pixel<Pd>& dest_, const Generalized_Pixel<Ps>& src_,
		 std::size_t n)
    {
      trace::entering("level::memcpy_");

      typedef mln_image(Pd) Id;
      metal::is_not_const<Id>::check();
      typedef mln_image(Ps) Is;
      Pd& dest = internal::force_exact<Pd>(dest_);
      Ps& src  = internal::force_exact<Ps>(src_);

      mln_precondition(sizeof(mln_value(Id)) == sizeof(mln_value(Is)));
      mln_precondition(dest.ima().has_data());
      mln_precondition(src.ima().has_data());

      mln_precondition(& dest.val() >= & dest.ima()[0]);
      mln_precondition(& dest.val() < & dest.ima()[0] + dest.ima().ncells());
      mln_precondition(& dest.val() + n <= & dest.ima()[0] + dest.ima().ncells());

      mln_precondition(& src.val() >= & src.ima()[0]);
      mln_precondition(& src.val() < & src.ima()[0] + src.ima().ncells());
      mln_precondition(& src.val() + n <= & src.ima()[0] + src.ima().ncells());

      impl::memcpy__(dest, src, n);

      trace::exiting("level::memcpy_");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::level

} // end of namespace mln


#endif // ! MLN_LEVEL_MEMCPY__HH
