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

#ifndef	OLN_MORPHO_ELEMENTARY_EROSION_HH
# define OLN_MORPHO_ELEMENTARY_EROSION_HH

# include <oln/level/apply_local.hh>
# include <oln/level/compare.hh>
# include <oln/border/fill.hh>
# include <oln/accumulator/min.hh>
# include <oln/accumulator/and.hh>


namespace oln
{

  namespace morpho
  {

    // Fwd decl.

    template <typename I>
    oln_plain(I)
    elementary_erosion(const Image_with_Nbh<I>& input);


# ifndef OLN_INCLUDE_ONLY

    namespace impl
    {

      // Generic version.

      template <typename I>
      oln_plain(I)
      elementary_erosion_on_function_(const Image<I>& input)
      {
	border::fill(input, oln_max(oln_value(I)));
	accumulator::min_<oln_value(I)> min;
	return level::apply_local(min, input);
      }

      template <typename I>
      oln_plain(I)
      elementary_erosion_on_set_(const Image<I>& input)
      {
	border::fill(input, true);
	accumulator::and_<oln_value(I)> accu_and;
	return level::apply_local(accu_and, input);
      }


      // Fast version.

//       template <typename I>
//       oln_plain(I)
// 	elementary_erosion_on_function_(const /*Fast_*/Image<I>&,
// 			    const I& input)
//       {
// 	std::cout << "fast" << std::endl;

// 	typedef oln_value(I) T;
//   	border::fill(input, oln_max(T));
// 	accumulator::min_<T> min;

// 	oln_plain(I) output;
// 	prepare(output, with, input);

// 	unsigned n = input.nbhood().size();
// 	T* jump = new T[n];
// 	for (unsigned i = 0; i < n; ++i)
// 	  jump[i] = input.image().pad(input.nbhood()[i]); // FIXME: pad is in Fast...

//         oln_piter(I) p(input.points());
//         for_all(p)
// 	  {
// 	    const T* ptr = & input(p);
// 	    min.init_with(*ptr);
// 	    for (unsigned i = 0; i < n; ++i)
// 	      min(*(ptr + jump[i]));
// 	    output(p) = min.value();
// 	  }
// 	delete[] jump;
//         return output;
//       }


      // Impl facade.

      template <typename I>
      oln_plain(I) elementary_erosion_(const Image<I>& input)
      {
	return elementary_erosion_on_function_(exact(input));
      }

      template <typename I>
      oln_plain(I) elementary_erosion_(const Binary_Image<I>& input)
      {
	return elementary_erosion_on_set_(exact(input));
      }


    } // end of namespace oln::morpho::impl


    // Facade.

    template <typename I>
    oln_plain(I)
    elementary_erosion(const Image_with_Nbh<I>& input)
    {
      oln_plain(I) output = impl::elementary_erosion_(exact(input));
      postcondition(output <= input);
      return output;
    }

# endif // ! OLN_INCLUDE_ONLY

  } // end of namespace oln::morpho

} // end of namespace oln


#endif // ! OLN_MORPHO_ELEMENTARY_EROSION_HH
