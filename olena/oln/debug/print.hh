// Copyright (C) 2006, 2007 EPITA Research and Development Laboratory
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

#ifndef OLN_DEBUG_PRINT_HH
# define OLN_DEBUG_PRINT_HH

# include <iostream>
# include <oln/core/concept/image.hh>

# ifdef OLN_ENV_2D
#  include <oln/core/2d/point2d.hh>
# endif


namespace oln
{


  namespace debug
  {

    /// Fwd decl.
    template <typename I>
    void print(const Image<I>& input, std::ostream& ostr = std::cout);


# ifndef OLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename T>
      const T& format(const T& value)
      {
	return value;
      }

      unsigned format(const unsigned char& value)
      {
	return value;
      }


      /// Generic version.

      template <typename I>
      void print_Gen(const Image<I>& input, std::ostream& ostr)
      {
	oln_fwd_piter(I) p(input.points());
	for_all(p)
	  ostr << p.to_point() << ':' << format(input(p)) << ' ';
      }

      template <typename I>
      void print(const Image<I>& input, std::ostream& ostr)
      {
	print_Gen(input, ostr);
      }



#  ifdef OLN_ENV_2D

      /// Default version.

      template <typename I>
      void print_2D(const Image<I>&, const I& input, std::ostream& ostr)
      {
	print_Gen(input, ostr);
      }

      /// Version for classical 2D images.

      template <typename I>
      void print_2D(const Point_Wise_Accessible_Image<I>&, const I& input,
		    std::ostream& ostr)
      {
	const oln_coord(I)
	  min_row = input.bbox().pmin().row(),
	  max_row = input.bbox().pmax().row();
	const oln_coord(I)
	  min_col = input.bbox().pmin().col(),
	  max_col = input.bbox().pmax().col();
	for (oln_coord(I) row = min_row; row <= max_row; ++row)
	  {
	    for (oln_coord(I) col = min_col; col <= max_col; ++col)
	      {
		point2d p(row, col);
		if (input.has(p))
		  ostr << format(input(p));
		else
		  ostr << '-';
		ostr << ' ';
	      }
	    ostr << std::endl;
	  }
      }

      template <typename I>
      void print(const Image_2D<I>& input, std::ostream& ostr)
      {
	impl::print_2D(exact(input), exact(input), ostr);
      }

#  endif //  OLN_ENV_2D


    } // end of namespace oln::debug::impl


    /// Facade.
    template <typename I>
    void print(const Image<I>& input, std::ostream& ostr)
    {
      impl::print(exact(input), ostr);
    }


# endif // ! OLN_INCLUDE_ONLY


  } // end of namespace oln::debug

} // end of namespace oln


#endif // ! OLN_DEBUG_PRINT_HH
