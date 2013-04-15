// Copyright (C) 2009, 2010, 2012 EPITA Research and Development
// Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_DEBUG_Z_ORDER_HH
# define MLN_DEBUG_Z_ORDER_HH

/// \file
///
/// \brief Fill an image with Z-order (curve) values.


# include <mln/core/concept/image.hh>


namespace mln
{

  namespace debug
  {

    /*! \brief Fill the image \p input with Z-order (curve) values.

       \param[in,out] input The image in which values are
       assigned.

       Reference: http://en.wikipedia.org/wiki/Z-order_(curve)

       \ingroup mlndebug
     */
    template <typename I>
    void z_order(Image<I>& input);


# ifndef MLN_INCLUDE_ONLY


    // Implementations.

    namespace impl
    {

      namespace generic
      {

	template <typename I>
	inline
	void
	z_order(Image<I>& input)
	{
	  unsigned row, col;
	  mln_piter(I) p(input.domain());
	  for_all(p)
	  {
	    row = p.row();
	    col = p.col();

	    mln_value(I)& v = input(p);
	    v = 0;
	    int mask = 0x00000001;
	    int i = 1;
	    while (row > 0 || col > 0)
	    {
	      if (i % 2)
	      {
		if (col & 0x0000001)
		  v = v | mask;
		col = col >> 1;
	      }
	      else
	      {
		if (row & 0x0000001)
		  v = v | mask;
		row = row >> 1;
	      }

	      mask = mask << 1;
	      ++i;
	    }
	  }

	}


      } // end of namespace mln::debug::impl::generic


    } // end of namespace mln::debug::impl



    // Dispatch

    namespace internal
    {

      template <typename I>
      inline
      void
      z_order_dispatch(trait::image::speed::any, Image<I>& input)
      {
	impl::generic::z_order(input);
      }


      template <typename I>
      inline
      void
      z_order_dispatch(Image<I>& input)
      {
	z_order_dispatch(mln_trait_image_speed(I)(), input);
      }

    }


    // Facade

    template <typename I>
    inline
    void
    z_order(Image<I>& input)
    {
      mln_trace("debug::z_order");
      mln_precondition(exact(input).is_valid());

      internal::z_order_dispatch(input);

    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::debug

} // end of namespace mln


#endif // ! MLN_DEBUG_Z_ORDER_HH
