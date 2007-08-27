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

#ifndef MLN_CORE_METAL_MAKE_VEC_HH
# define MLN_CORE_METAL_MAKE_VEC_HH

# include <mln/metal/vec.hh>


namespace mln
{

  namespace metal
  {

    namespace make
    {

      template <typename T>
      mln::metal::vec<1,T> vec(T c0);

      template <typename T>
      mln::metal::vec<2,T> vec(T c0, T c1);

      template <typename T>
      mln::metal::vec<3,T> vec(T c0, T c1, T c2);

      template <typename T>
      mln::metal::vec<4,T> vec(T c0, T c1, T c2, T c3);


# ifndef MLN_INCLUDE_ONLY
 
      template <typename T>
      mln::metal::vec<1,T> vec(T c0)
      {
	mln::metal::vec<1,T> tmp;
	tmp[0] = c0;
	return tmp;
      }

      template <typename T>
      mln::metal::vec<2,T> vec(T c0, T c1)
      {
	mln::metal::vec<2,T> tmp;
	tmp[0] = c0;
	tmp[1] = c1;
	return tmp;
      }

      template <typename T>
      mln::metal::vec<3,T> vec(T c0, T c1, T c2)
      {
	mln::metal::vec<3,T> tmp;
	tmp[0] = c0;
	tmp[1] = c1;
	tmp[2] = c2;
	return tmp;
      }

      template <typename T>
      mln::metal::vec<4,T> vec(T c0, T c1, T c2, T c3)
      {
	mln::metal::vec<4,T> tmp;
	tmp[0] = c0;
	tmp[1] = c1;
	tmp[2] = c2;
	tmp[3] = c3;
	return tmp;
      }

# endif // ! MLN_INCLUDE_ONLY
  
    } // end of namespace mln::metal::make
  
  } // end of namespace mln::metal
  
} // end of namespace mln


# include <mln/metal/make/vec.hh>


#endif // ! MLN_CORE_METAL_MAKE_VEC_HH
