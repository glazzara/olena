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

#ifndef	OLN_MORPHO_GRADIENT_HH
# define OLN_MORPHO_GRADIENT_HH

# include <oln/core/gen/zero.hh>
# include <oln/morpho/erosion.hh>
# include <oln/morpho/dilation.hh>
# include <oln/arith/minus.hh>


namespace oln
{

  namespace morpho
  {

    // Fwd decl.

    template <typename I, typename W>
    oln_plain(I)
    gradient(const Image<I>& input, const Window<W>& win);


# ifndef OLN_INCLUDE_ONLY

    namespace impl
    {

      // Generic version.

      template <typename I, typename W>
      oln_plain(I)
      gradient_(const Image<I>& input, const Window<W>& win)
      {
	oln_plain(I) dil = dilation(input, win);
	oln_plain(I) ero = erosion(input, win);
	return arith::minus<oln_value(I)>(dil, ero);
      }


    } // end of namespace oln::morpho::impl


    // Facade.

    template <typename I, typename W>
    oln_plain(I)
    gradient(const Image<I>& input, const Window<W>& win)
    {
      oln_plain(I) output = impl::gradient_(exact(input), exact(win));
      postcondition(output >= oln_value(I)(zero));
      return output;
    }

# endif // ! OLN_INCLUDE_ONLY

  } // end of namespace oln::morpho

} // end of namespace oln


#endif // ! OLN_MORPHO_GRADIENT_HH
