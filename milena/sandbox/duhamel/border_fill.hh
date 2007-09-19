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

#ifndef SANDBOX_MLN_BORDER_FILL_HH
# define SANDBOX_MLN_BORDER_FILL_HH

/*! \file mln/border/fill.hh
 *
 * \brief FIXME.
 */

# include <mln/core/concept/image.hh>
# include <mln/core/internal/fixme.hh>
# include <mln/core/line_piter.hh>
# include <mln/level/memset_.hh>

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
    void fill(const Fast_Image<I>& ima, const mln_value(I)& v);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {
      template <typename I>
      void fill_1d_size_1_(const Fast_Image<I>& ima_, const mln_value(I)& v)
      {
	const I& ima = exact(ima_);
	mln_precondition(ima.has_data());

	typedef mln_point(I) P;
	std::size_t border = ima.border ();
	std::size_t len = exact(ima).bbox().len(P::dim - 1);

	std::memset((void*)&ima[0],
		    *(const int*)(&v), // violent cast
		    border);

	std::memset((void*)&ima[border + len],
		    *(const int*)(&v), // violent cast
		    border);
      }

      template <typename I>
      void fill_1d_size_n_(const Fast_Image<I>& ima_, const mln_value(I)& v)
      {
	const I& ima = exact(ima_);
	mln_precondition(ima.has_data());

	typedef mln_point(I) P;
	std::size_t border = ima.border ();
	std::size_t len = exact(ima).bbox().len(P::dim - 1);
	std::size_t i = 0;

	for (; i < border; ++i)
	  const_cast<I&>(ima)[i] = v;

	std::size_t e = 2 * border + len;
	for (i += len; i < e; ++i)
	  const_cast<I&>(ima)[i] = v;
      }

      template <typename I>
      void fill_2d_size_1_(const Fast_Image<I>& ima_, const mln_value(I)& v)
      {
	const I& ima = exact(ima_);
	mln_precondition(ima.has_data());

	typedef mln_point(I) P;

	std::size_t border = ima.border ();
	std::size_t border_2x = 2 * border;
	std::size_t len = exact(ima).bbox().len(P::dim - 1);
	std::size_t n = len + 2 * border;
	std::size_t s = border * (n + 1);
	std::size_t e = n;

	for (int i = P::dim - 2; i >= 0; --i)
	  e *= exact(ima).bbox().len(i) + 2 * border;

	std::memset((void*)&ima[0],
		    *(const int*)(&v), // violent cast
		    s);

	std::size_t end = e - ((border + 1) * n);
	for (s += len; s < end; s += n)
	  std::memset((void*)&ima[s],
		      *(const int*)(&v), // violent cast
		      border_2x);
	
	std::memset((void*)&ima[s],
		    *(const int*)(&v), // violent cast
		    e - s);
      }

      template <typename I>
      void fill_2d_size_n_(const Fast_Image<I>& ima_, const mln_value(I)& v)
      {
	const I& ima = exact(ima_);
	mln_precondition(ima.has_data());

	typedef mln_point(I) P;

	std::size_t border = ima.border ();
	std::size_t border_2x = 2 * border;
	std::size_t len = exact(ima).bbox().len(P::dim - 1);
	std::size_t n = len + 2 * border;
	std::size_t s = border * (n + 1);
	std::size_t e = n;

	for (int i = P::dim - 2; i >= 0; --i)
	  e *= exact(ima).bbox().len(i) + 2 * border;

	for (std::size_t i = 0; i < s; ++i)
	  const_cast<I&>(ima)[i] = v;
	  
 	std::size_t end = e - ((border + 1) * n);
	for (s += len; s < end; s += len)
	  for (std::size_t j = 0; j < border_2x; ++j, ++s)
	    (const_cast<I&>(ima))[s] = v;
	
	for (std::size_t i = s; i < e; ++i)
	  const_cast<I&>(ima)[i] = v;
      }

//       template <typename I>
//       void fill_3d_size_n_(const Fast_Image<I>& ima_, const mln_value(I)& v)
//       {
// 	const I& ima = exact(ima_);
// 	mln_precondition(ima.has_data());

//  	typedef mln_point(I) P;

// 	std::size_t border = ima.border ();
//  	std::size_t border_2x = 2 * border;
// 	std::size_t len_s = exact(ima).bbox().len(0);
// 	std::size_t len_r = exact(ima).bbox().len(1);
// 	std::size_t len_c = exact(ima).bbox().len(2);

// 	//	std::size_t real_len_s = len_s + border_2x;
// 	std::size_t real_len_r = len_r + border_2x;
// 	std::size_t real_len_c = len_c + border_2x;

// 	std::size_t face = real_len_r * real_len_c;

// 	std::size_t cpt = 0;

// 	for (std::size_t k = 0; k < border; ++k)
// 	  for (std::size_t i = 0; i < face; ++i, ++cpt)
// 	    const_cast<I&>(ima)[cpt] = v;

//  	for (std::size_t k = 0; k < len_s; ++k)
// 	  {
// 	    std::size_t s = real_len_c * border + border;
// 	    for (std::size_t i = 0; i < s; ++i, ++cpt)
// 	      const_cast<I&>(ima)[cpt] = v;


// 	    cpt += len_r;
//  	    std::size_t end = cpt + len_r + len_c * len_r;

// 	    for (; cpt < end; cpt += len_r)
// 	      {
// 		std::cout << "cpt = "
// 			  << cpt
// 			  << "end = "
// 			  << end
// 			  << std::endl;
// 		for (std::size_t j = 0; j < border_2x; ++j, ++cpt)
// 		  (const_cast<I&>(ima))[cpt] = v;
// 	      }
	    
// 	    for (std::size_t i = 0; i < s; ++i, ++cpt)
// 	      const_cast<I&>(ima)[cpt] = v;
// 	  }

// 	for (std::size_t k = 0; k < border; ++k)
// 	  for (std::size_t i = 0; i < face; ++i, ++cpt)
// 	    const_cast<I&>(ima)[cpt] = v;
//       }

      template <typename I>
      void fill_size_1_(const Fast_Image<I>& ima_, const mln_value(I)& v)
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
      void fill_size_n_(const Fast_Image<I>& ima_, const mln_value(I)& v)
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
    } // end of namespace mln::border::impl


    // Facade.

    template <typename I>
    void fill(const Fast_Image<I>& ima_, const mln_value(I)& v)
    {
      typedef mln_point(I) P;
      const I& ima = exact(ima_);
      mln_precondition(ima.has_data());

      if (!ima.border ())
	return;
      if (sizeof(mln_value(I)) == 1)
	impl::fill_size_1_(ima_, v);
      else
	impl::fill_size_n_(ima_, v);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::border

} // end of namespace mln


#endif // ! SANDBOX_MLN_BORDER_FILL_HH
