// Copyright (C) 2008 EPITA Research and Development Laboratory
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_TRAIT_WINDOW_PRINT_HH
# define MLN_TRAIT_WINDOW_PRINT_HH

/*! \file mln/trait/window/print.hh
 *
 * \brief Print the collection of traits for a window type.
 */

# include <iostream>

# include <mln/trait/windows.hh>
# include <mln/metal/is_a.hh>
# include <mln/metal/bexpr.hh>



namespace mln
{

  // Forward declaration.
  template <typename E> struct Window;
  template <typename E> struct Weighted_Window;
  template <typename E> struct Neighborhood;


  namespace trait
  {

    namespace window
    {

      template <typename T>
      void print(std::ostream& ostr = std::cout);

      template <typename T>
      void print(const Neighborhood<T>& nbh, std::ostream& ostr = std::cout);

      template <typename T>
      void print(const Window<T>& win, std::ostream& ostr = std::cout);

      template <typename T>
      void print(const Weighted_Window<T>& w_win, std::ostream& ostr = std::cout);


# ifndef MLN_INCLUDE_ONLY

      template <typename T>
      inline
      void print(std::ostream& ostr)
      {
	metal::or_< mlc_is_a(T, Neighborhood),
	            metal::or_< mlc_is_a(T, Window),
	                        mlc_is_a(T, Weighted_Window) > 
	  >::check();
	typedef mln::trait::window_<T> the;
	ostr << "{ "
	     << typename the::size()       .name() << ", "
	     << typename the::support()    .name() << ", "
	     << typename the::definition() .name() << " }" << std::endl;
      }

      template <typename T>
      inline
      void print(const Neighborhood<T>&, std::ostream& ostr)
      {
	print<T>(ostr);
      }

      template <typename T>
      inline
      void print(const Window<T>&, std::ostream& ostr)
      {
	print<T>(ostr);
      }

      template <typename T>
      inline
      void print(const Weighted_Window<T>&, std::ostream& ostr)
      {
	print<T>(ostr);
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::trait::window

  } // end of namespace mln::trait

} // end of namespace mln


#endif // ! MLN_TRAIT_WINDOW_PRINT_HH
