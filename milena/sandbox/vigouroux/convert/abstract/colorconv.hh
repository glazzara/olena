// Copyright (C) 2001, 2002, 2003, 2004  EPITA Research and Development Laboratory
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
// the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
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

#ifndef OLENA_CONVERT_ABSTRACT_COLORCONV_HH
# define OLENA_CONVERT_ABSTRACT_COLORCONV_HH

# include <ntg/color/color.hh>
# include <oln/convert/abstract/conversion.hh>

namespace oln {

  namespace convert {

    namespace abstract {

      /*! Base class for color conversion.
      **
      ** \param icomps Number of components in the input.
      ** \param iqbits Number of bits per components in the input.
      ** \param icolor Input color.
      ** \param ocomps Number of components in the output.
      ** \param oqbits Number of bits per components in the output.
      ** \param ocolor Output color.
      ** \param Exact Exact class.
      */
      template< unsigned icomps,
		unsigned iqbits,
		template<unsigned> class icolor,
		unsigned ocomps,
		unsigned oqbits,
		template<unsigned> class ocolor,
		class Exact = mlc::final >
      struct color_conversion :
	public abstract::conversion_from_type_to_type< ntg::color<icomps, iqbits, icolor>,
						       ntg::color<ocomps, oqbits, ocolor>,
						       typename mlc::exact_vt<color_conversion<icomps, iqbits, icolor, ocomps, oqbits, ocolor, Exact>, Exact>::ret >
      {
      };
    }
  } // convert
} // oln


#endif // OLENA_CONVERT_ABSTRACT_COLORCONV_HH
