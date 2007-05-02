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

#ifndef	OLN_MORPHO_THINNING_HH
# define OLN_MORPHO_THINNING_HH

# include <oln/morpho/erosion.hh>
# include <oln/morpho/dilation.hh>
/*
# include <oln/core/gen/ternary_fun.hh>
# include <oln/core/level/ternary.hh>
# include <oln/arith/ternary.hh>
*/


namespace oln
{

  namespace morpho
  {

    // Fwd decl.

    template <typename I, typename W1, typename W2>
    oln_plain(I)
    thinning(const Image<I>& input,
	     const Window<W1>& B1, const Window<W2>& B2);


# ifndef OLN_INCLUDE_ONLY

    namespace impl
    {

      // Generic version.

      template <typename I, typename W1, typename W2>
      oln_plain(I)
      thinning_on_function_(const Image<I>& f,
			    const Window<W1>& B1, const Window<W2>& B2)
      {
	oln_plain(I)
	  e = morpho::erosion(f, B1),
	  d = morpho::dilation(f, B2);

	/*
	oln_plain_value(I, bool) t;
	t = d < f and f == e;
	return level::ternary(t, d, f);
	*/

	/*
	oln_plain(I) o;
	prepare(o, with, f);
	level::fill( inplace(o),
		     ternary_fun( pw_value(d) < pw_value(f) and pw_value(f) == pw_value(e),
				  pw_value(d),
				  pw_value(f) ) );
        return o;
	*/

	oln_plain(I) o;
	prepare(o, with, f);
	oln_piter(I) p(f.points());
	for_all(p)
	  o(p) = ( d(p) < f(p) and f(p) == e(p)
		   ? d(p)
		   : f(p) );
	return o;
      }

      // FIXME: Add a fast version.


      // Impl facade.

      template <typename I, typename W1, typename W2>
      oln_plain(I)
      thinning_(const Image<I>& input,
		const W1& B1, const W2& B2)
      {
	return thinning_on_function_(exact(input), B1, B2);
      }

//       template <typename I, typename W1, typename W2>
//       oln_plain(I)
//       thinning_(const Binary_Image<I>& input,
// 		const W1& B1, const W2& B2)
//       {
// 	return thinning_on_set_(exact(input), B1, B2);
//       }
      

    } // end of namespace oln::morpho::impl


    // Facade.

    template <typename I, typename W1, typename W2>
    oln_plain(I)
    thinning(const Image<I>& input,
	     const Window<W1>& B1, const Window<W2>& B2)
    {
      precondition(B1.is_centered());
      oln_plain(I) output = impl::thinning_(exact(input), exact(B1), exact(B2));
      postcondition(output <= input);
      return output;
    }

# endif // ! OLN_INCLUDE_ONLY

  } // end of namespace oln::morpho

} // end of namespace oln


#endif // ! OLN_MORPHO_THINNING_HH
