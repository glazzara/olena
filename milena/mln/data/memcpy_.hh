// Copyright (C) 2007, 2008, 2009, 2011 EPITA Research and Development
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

#ifndef MLN_DATA_MEMCPY__HH
# define MLN_DATA_MEMCPY__HH

/// \file
///
/// Same as std::memcpy but for fastest images.
///
/// \todo Overload with images and points; Cf. memset_.

# include <cstring>
# include <mln/core/concept/image.hh>
# include <mln/core/pixel.hh>
# include <mln/metal/is_not_const.hh>
# include <mln/opt/element.hh>



namespace mln
{

  namespace data
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
      inline
      void memcpy__(Pd& dest, const Ps& src, std::size_t n)
      {
	// mln_trace("data::impl::memcpy__");

	typedef mln_image(Pd) Id;
	typedef mln_image(Ps) Is;
	if (n == 0)
	  return; // no-op

	if (n == 1)
	{
	  dest.val() = src.val(); // one assignment
	  return;
	}

	/* Careful, the code generated for this function by g++ 4.2
	   with a high optimization level (`-O3') and without
	   `-fno-strict-aliasing' might be wrong (at least with
	   Debian's g++ 4.2 on IA-32)!  Note that Debian's g++ 4.0,
	   4.1, 4.3 and 4.4 are fine.

	   We used to trigger a warning when g++ 4.2 was detected, but
	   we no longer do this since this warning was popping in
	   virtually every compiler output and because this bug is
	   limited to some specific use cases.  Moreover, g++ 4.2 will
	   be less and less used over time.  */
	if (sizeof(mln_value(Id)) == 1)
	{
	  std::memcpy((void*) (&dest.val()), // violent casts
		      (const void*) (&src.val()),
		      n);
	}
	else
	{
	  mln_value(Id)* p_d = &dest.val();
	  const mln_value(Is)* p_s = &src.val();
	  for (std::size_t i = 0; i < n; ++i)
	    *p_d++ = *p_s++;
	}

      }

    }

    template <typename Pd, typename Ps>
    inline
    void memcpy_(Generalized_Pixel<Pd>& dest_,
		 const Generalized_Pixel<Ps>& src_,
		 std::size_t n)
    {
      // mln_trace("data::memcpy_");

      typedef mln_image(Pd) Id;
      metal::is_not_const<Id>::check();
      typedef mln_image(Ps) Is;
      Pd& dest = mln::internal::force_exact<Pd>(dest_);
      Ps& src  = mln::internal::force_exact<Ps>(src_);

      mln_precondition(sizeof(mln_value(Id)) == sizeof(mln_value(Is)));
      mln_precondition(dest.ima().is_valid());
      mln_precondition(src.ima().is_valid());

      mln_precondition(&dest.val() >= &opt::element(dest.ima(), 0));
      mln_precondition(&dest.val() < &opt::element(dest.ima(), 0) +
                       opt::nelements(dest.ima()));

      mln_precondition(&dest.val() + n <= &opt::element(dest.ima(), 0) +
                       opt::nelements(dest.ima()));

      mln_precondition(&src.val() >= &opt::element(src.ima(), 0));
      mln_precondition(&src.val() < &opt::element(src.ima(), 0) +
                       opt::nelements(src.ima()));
      mln_precondition(&src.val() + n <= &opt::element(src.ima(), 0) +
                       opt::nelements(src.ima()));

      impl::memcpy__(dest, src, n);

    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::data

} // end of namespace mln


#endif // ! MLN_DATA_MEMCPY__HH
