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

#ifndef OLN_DEBUG_PRINT_NBH_HH
# define OLN_DEBUG_PRINT_NBH_HH

# include <iostream>
# include <oln/core/concept/image.hh>


namespace oln
{

  namespace debug
  {

    // fwd decl
    template <typename I>
    void print_nbh(const Image_with_Nbh<I>& input, std::ostream& ostr = std::cout);


# ifndef OLN_INCLUDE_ONLY

    namespace impl
    {

      // Image

      template <typename I>
      void print_nbh(const Image<I>&,
		     const I& input, std::ostream& ostr)
      {
	oln_piter(I) p(input.points());
	oln_niter(I) n(p, input);
	for_all(p)
	  {
	    ostr << input(p) << ": ";
	    for_all(n)
	      if (input.owns_(n))
		ostr << input(n) << " ";
	    ostr << std::endl;
	  }
      }


      // Point_Wise_Accessible_Image

      template <typename I>
      void print_nbh(const Point_Wise_Accessible_Image<I>&,
		     const I& input, std::ostream& ostr)
      {
	oln_piter(I) p(input.points());
	oln_niter(I) n(p, input);
	for_all(p)
	  {
	    ostr << input(p) << ": ";
	    for_all(n)
	      if (input.has(n))
		ostr << input(n) << " ";
	    ostr << std::endl;
	  }
      }

    } // end of namespace oln::debug::impl


    // facade
    template <typename I>
    void print_nbh(const Image_with_Nbh<I>& input, std::ostream& ostr)
    {
      impl::print_nbh(exact(input), exact(input), ostr);
    }

# endif // ! OLN_INCLUDE_ONLY


  } // end of namespace oln::debug

} // end of namespace oln


#endif // ! OLN_DEBUG_PRINT_NBH_HH
