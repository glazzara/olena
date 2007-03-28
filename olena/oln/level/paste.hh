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

#ifndef OLN_LEVEL_PASTE_HH
# define OLN_LEVEL_PASTE_HH

# include <oln/core/concept/image.hh>


namespace oln
{

  namespace level
  {

    // Fwd decl.

    template <typename I, typename J>
    void paste(const Image<I>& data, /* in */ Mutable_Image<J>& destination);


# ifndef OLN_INCLUDE_ONLY

    namespace impl
    {

      // Generic version.

      template <typename I, typename J>
      void paste_(const Image<I>& data, Mutable_Image<J>& destination)
      {
	oln_piter(I) p(data.points());
	for_all(p)
	  destination(p) = data(p);
      }

      // FIXME: Fast version...

    } // end of namespace oln::level::impl


    // Facade.

    template <typename I, typename J>
    void paste(const Image<I>& data, Mutable_Image<J>& destination)
    {
      assert_same_grid_<I, J>::check();
      precondition(data.points() <= destination.points());
      impl::paste_(exact(data), exact(destination));
    }

# endif // ! OLN_INCLUDE_ONLY

  } // end of namespace oln::level

} // end of namespace oln


#endif // ! OLN_LEVEL_PASTE_HH
