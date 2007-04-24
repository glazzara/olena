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

#ifndef	OLN_LINEAR_LOG_17X17_HH
# define OLN_LINEAR_LOG_17X17_HH

# include <oln/linear/convolution.hh>
# include <oln/core/2d/weighted_window2d.hh>


namespace oln
{

  namespace linear
  {

    // Fwd decl.

    template <typename V, typename I>
    oln_plain_value(I, V)
    log_17x17(const Image_2D<I>& f);


#  ifndef OLN_INCLUDE_ONLY

    namespace impl
    {

      // Generic versions.

      template <typename V, typename I>
      oln_plain_value(I, V)
      log_17x17_(const Image_2D<I>& f)
      {
	// Ref: Cf. Sonka et al., pages 85-86.
	static int values[] = { 0,  0,  0,  0,  0,  0, -1, -1, -1, -1, -1,  0,  0,  0,  0,  0,  0, 
				0,  0,  0,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1,  0,  0,  0,  0, 
				0,  0, -1, -1, -1, -2, -3, -3, -3, -3, -3, -2, -1, -1, -1,  0,  0, 
				0,  0, -1, -1, -2, -3, -3, -3, -3, -3, -3, -3, -2, -1, -1,  0,  0, 
				0, -1, -1, -2, -3, -3, -3, -2, -3, -2, -3, -3, -3, -2, -1, -1,  0, 
				0, -1, -2, -3, -3, -3,  0,  2,  4,  2,  0, -3, -3, -3, -2, -1,  0, 
			       -1, -1, -3, -3, -3,  0,  4, 10, 12, 10,  4,  0, -3, -3, -3, -1, -1, 
			       -1, -1, -3, -3, -2,  2, 10, 18, 21, 18, 10,  2, -2, -3, -3, -1, -1, 
			       -1, -1, -3, -3, -3,  4, 12, 21, 24, 21, 12,  4, -3, -3, -3, -1, -1, 
			       -1, -1, -3, -3, -2,  2, 10, 18, 21, 18, 10,  2, -2, -3, -3, -1, -1, 
			       -1, -1, -3, -3, -3,  0,  4, 10, 12, 10,  4,  0, -3, -3, -3, -1, -1, 
				0, -1, -2, -3, -3, -3,  0,  2,  4,  2,  0, -3, -3, -3, -2, -1,  0, 
				0, -1, -1, -2, -3, -3, -3, -2, -3, -2, -3, -3, -3, -2, -1, -1,  0, 
				0,  0, -1, -1, -2, -3, -3, -3, -3, -3, -3, -3, -2, -1, -1,  0,  0, 
				0,  0, -1, -1, -1, -2, -3, -3, -3, -3, -3, -2, -1, -1, -1,  0,  0, 
				0,  0,  0,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1,  0,  0,  0,  0, 
				0,  0,  0,  0,  0,  0, -1, -1, -1, -1, -1,  0,  0,  0,  0,  0,  0 };
	return oln::linear::convolution<V>(f, values);
      }

    } // end of namespace oln::linear::impl


    // Facade.

    template <typename V, typename I>
    oln_plain_value(I, V)
    log_17x17(const Image_2D<I>& f)
    {
      return impl::log_17x17_<V>(f);
    }

#  endif // ! OLN_INCLUDE_ONLY

  } // end of namespace oln::linear

} // end of namespace oln


#endif // ! OLN_LINEAR_LOG_17X17_HH
