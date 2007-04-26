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

#ifndef OLN_DRAW_BRESENHAM_HH
# define OLN_DRAW_BRESENHAM_HH

# include <oln/core/concept/image.hh>
# include <oln/core/gen/inplace.hh>
# include <oln/core/gen/over.hh>
# include <oln/level/paste.hh>

# include <oln/core/2d/line2d.hh> // FIXME: 2D!



namespace oln
{

  namespace draw
  {

    // Fwd decl.

    template <typename I>
    void bresenham(inplace_<I> in_out,
		   const oln_point(I)& begin, const oln_point(I)& end,
		   const oln_value(I)& value);


# ifndef OLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I>
      void bresenham_(Mutable_Image<I>& in_out,
		      const oln_point(I)& begin, const oln_point(I)& end,
		      const oln_value(I)& value)
      {
  	level::paste(make_value(value) / line2d(begin, end),
		     inplace(in_out));
      }

    } // end of namespace oln::draw::impl


    // Facade.

    template <typename I>
    void bresenham(inplace_<I> in_out,
		   const oln_point(I)& begin, const oln_point(I)& end,
		   const oln_value(I)& value)
    {
      impl::bresenham_(in_out.unwrap(), begin, end, value);
    }


    // Guard.

    template <typename I, typename P, typename V>
    void bresenham(const Image<I>&,
		   const P&, const P&,
		   const V&)
    {
      mlc::abort_<I>::check(); // FIXME: Add err msg.
    }

# endif // ! OLN_INCLUDE_ONLY

  } // end of namespace oln::draw

} // end of namespace oln


#endif // ! OLN_DRAW_BRESENHAM_HH
