// Copyright (C) 2006 EPITA Research and Development Laboratory
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
# include <oln/core/abstract/image.hh>


namespace oln
{


  namespace debug
  {

    /// Fwd decl.
    template <typename I>
    void print(const abstract::image<I>& input, std::ostream& ostr);

    /// Fwd decl.
    template <typename I>
    void println(const abstract::image<I>& input, std::ostream& ostr);

# ifndef OLN_INCLUDE_ONLY

    namespace impl
    {

      /// Generic version.
      template <typename I>
      void print(const abstract::image<I>& input, std::ostream& ostr)
      {
	oln_type_of(I, fwd_piter) p(input.topo());
	for_all(p)
	  ostr << p.to_point() << ':' << ima(p) << ' ';
      }


      /// Version for classical 2D images.
      template <typename I>
      void print(const abstract::classical_2d_image<I>& input,
		 std::ostream& ostr)
      {
	for (int row = input.pmin().row(); row <= input.pmax().row(); ++row)
	  {
	    for (int col = input.pmin().col(); col <= input.pmax().col(); ++col)
	      {
		point2d p(row, col);
		if (input.has(p))
		  ostr << input(p);
		else
		  ostr << '-';
		ostr << ' ';
	      }
	    ostr << std::endl;
	  }
      }

    } // end of namespace oln::debug::impl



    /// Facade.
    template <typename I>
    void print(const abstract::image<I>& input, std::ostream& ostr)
    {
      impl::print(input.exact(), ostr);
    }


    /// Facade.
    template <typename I>
    void println(const abstract::image<I>& input, std::ostream& ostr)
    {
      print(input, ostr);
      ostr << std::endl;
    }

# endif // ! OLN_INCLUDE_ONLY

  } // end of namespace oln::debug

} // end of namespace oln


#endif // ! OLN_DEBUG_PRINT_HH
