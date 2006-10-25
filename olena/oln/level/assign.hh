// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006 EPITA Research and
// Development Laboratory
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

#ifndef OLN_LEVEL_ASSIGN_HH
# define OLN_LEVEL_ASSIGN_HH

# include <iostream>

# include <mlc/assert.hh>
# include <mlc/is_a.hh>

# include <oln/core/abstract/image.hh>
# include <oln/core/abstract/iterator.hh>
# include <oln/core/automatic/image/mutable_image.hh>


namespace oln
{

  namespace level
  {

    /// Fwd decls.

    template <typename Idest, typename Isrc>
    void assign(abstract::mutable_image<Idest>& dest, const abstract::image<Isrc>& src);


# ifndef OLN_INCLUDE_ONLY

    namespace impl
    {

      /// Generic version.
      template <typename Idest, typename Isrc>
      void assign(abstract::mutable_image<Idest>& dest, const abstract::image<Isrc>& src)
      {
	oln_piter(Isrc) p(src.topo());
	for_all(p)
	  dest(p) = src(p);
      }

    } // end of namespace oln::level::impl


    /// Facade.
    template <typename Idest, typename Isrc>
    void assign(abstract::mutable_image<Idest>& dest, const abstract::image<Isrc>& src)
    {
      impl::assign(dest.exact(), src.exact());
    }

# endif

  } // end of namespace oln::level

} // end of namespace oln


#endif // ! OLN_LEVEL_ASSIGN_HH
