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

#ifndef OLN_ARITH_NEGATE_HH
# define OLN_ARITH_NEGATE_HH

# include <oln/core/concept/image.hh>
# include <oln/core/internal/max_value.hh>
# include <oln/core/internal/min_value.hh>


namespace oln
{
  
  namespace arith
  {

    // Fwd decl.

    template <typename I>
    oln_plain(I)
    negate(const Image<I>& input);


# ifndef OLN_INCLUDE_ONLY

    namespace impl
    {

      // FIXME: Trash code! Not so simple...

      template <typename T>
      T value_negate_(const T& v)
      {
	return static_cast<T>(oln_min(T) + (oln_max(T) - v));
      }


      // Generic version.

      template <typename I>
      oln_plain(I)
      negate_(const Image<I>& input)
      {
	oln_plain(I) output;
	prepare(output, with, input);
	oln_piter(I) p(input.points());
	for_all(p)
	  output(p) = value_negate_<oln_value(I)>(input(p));
	return output;
      }

    } // end of namespace oln::arith::impl


    // Facade.

    template <typename I>
    oln_plain(I)
    negate(const Image<I>& input)
    {
      return impl::negate_(exact(input));
    }


  } // end of namespace oln::arith

# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln

#endif // ! OLN_ARITH_NEGATE_HH
