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

#ifndef MLN_BORDER_FILL_HH
# define MLN_BORDER_FILL_HH

/*! \file mln/border/fill.hh
 *
 * \brief FIXME.
 */

# include <mln/core/concept/image.hh>


namespace mln
{

  namespace border
  {

    /*! Fill the virtual (outer) border of image \p ima with the
     *  single value \p v.
     *
     * \param[in,out] ima The image whose border is to be filled.
     * \param[in] v The value to assign to all border pixels.
     *
     * \pre \p ima has to be initialized.
     *
     * \todo Implement it + optimize with memset if possible.
     */
    template <typename I>
    void fill(const Fastest_Image<I>& ima, const mln_value(I)& v);

# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I>
      void fill_size_1_(const Fastest_Image<I>& ima_, const mln_value(I)& v)
      {
	typedef mln_point(I) P;
	const I& ima = exact(ima_);
	typedef mln_point(I) P;
	typename I::line_piter pl(ima.domain());
	std::size_t len_r = exact(ima).bbox().len(P::dim - 1);
	std::size_t st = 0;
	
	for_all (pl)
	  {
	    std::size_t end = ima.offset_at (pl);
	    std::memset((void*)&ima[st],
			*(const int*)(&v),
			end - st);
	    st = end + len_r;
	  }
	std::memset((void*)&ima[st],
		    *(const int*)(&v),
		    ima.ncells () - st);
      }
    
      template <typename I>
      void fill_size_n_(const Fastest_Image<I>& ima_, const mln_value(I)& v)
      {
	typedef mln_point(I) P;
	const I& ima = exact(ima_);
	typedef mln_point(I) P;
	typename I::line_piter pl(ima.domain());
	std::size_t len_r = exact(ima).bbox().len(P::dim - 1);
	std::size_t st = 0;
	
	for_all (pl)
	  {
	    std::size_t end = ima.offset_at (pl);
	    for (std::size_t i = st; i < end; ++i)
	      const_cast<I&>(ima)[i] = v;
	    st = end + len_r;
	  }
	for (std::size_t i = st; i < ima.ncells (); ++i)
	  const_cast<I&>(ima)[i] = v;
      }

    }
    // Facade.

    template <typename I>
    void fill(const Fastest_Image<I>& ima_, const mln_value(I)& v)
    {
      trace::entering("border::fill");
      typedef mln_point(I) P;
      const I& ima = exact(ima_);
      mln_precondition(ima.has_data());
      if (!ima.border ())
	return;
      if (sizeof(mln_value(I)) == 1)
	impl::fill_size_1_(ima_, v);
      else
	impl::fill_size_n_(ima_, v);
      trace::exiting("border::fill");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::border

} // end of namespace mln


#endif // ! MLN_BORDER_FILL_HH
