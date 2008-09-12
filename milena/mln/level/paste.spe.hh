// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_LEVEL_PASTE_SPE_HH
# define MLN_LEVEL_PASTE_SPE_HH

/*! \file mln/level/paste.spe.hh
 *
 * \brief Specializations for mln::level::paste.
 *
 * \todo Rewrite the dispatch!!!
 */

# ifndef MLN_LEVEL_PASTE_HH
#  error "Forbidden inclusion of *.spe.hh"
# endif // ! MLN_LEVEL_PASTE_HH

# include <mln/level/memcpy_.hh>



# ifndef MLN_INCLUDE_ONLY

namespace mln
{

  namespace level
  {

    namespace impl
    {

      namespace generic
      {
	template <typename I, typename J>
	void paste(const Image<I>& data, Image<J>& destination);
      }

//       template <typename I, typename J>
//       inline
//       void paste_lines_(const I& data, J& destination)
//       {
// 	trace::entering("level::impl::paste_lines_");
// 	typedef mln_psite(I) P;
// 	std::size_t n = data.bbox().len(P::dim - 1);
// 	// FIXME: Works only for raw data images
// 	// FIXME: For linear data images, we should get the len for each line...

// 	typename I::line_piter p(data.domain()); // FIXME: Alias mln_line_piter!
// 	//	mln_line_piter(I) p(data.domain());
// 	for_all(p)
// 	  memcpy_(inplace(make::pixel(destination, p)),
// 		  make::pixel(data, p),
// 		  n);
// 	trace::exiting("level::impl::paste_lines_");
//       }


//       // Disjunction.

//       // Remember: raw < linear < stored, computed.

//       template <typename I, typename J>
//       inline
//       void paste_(trait::image::value_storage::any, const I& data,
// 		  trait::image::value_storage::any, J& destination)
//       {
// 	generic::paste_(data, destination);
//       }

//       template <typename I, typename J>
//       inline
//       void paste_(trait::image::value_storage::one_block, const I& data,
// 		  trait::image::value_storage::one_block, J& destination)
//       {
// 	if (sizeof(mln_value(I)) == sizeof(mln_value(J)))
// 	  paste_lines_(data, destination);
// 	else
// 	  generic::paste_(data, destination);
//       }


    } // end of namespace mln::level::impl

    namespace internal
    {

      template <typename I, typename J>
      inline
      void paste_dispatch(const Image<I>& data, Image<J>& destination)
      {
	impl::generic::paste(data, destination);
//       impl::paste_(mln_trait_image_value_storage(I)(), data,
// 		   mln_trait_image_value_storage(J)(), destination);
      }      

    } // end of namespace mln::level::internal

  } // end of namespace mln::level

} // end of namespace mln

# endif // ! MLN_INCLUDE_ONLY


#endif // ! MLN_LEVEL_PASTE_HH
