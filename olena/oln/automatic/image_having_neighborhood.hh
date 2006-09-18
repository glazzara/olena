// Copyright (C) 2006 EPITA Research and Development Laboratory
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

#ifndef OLENA_AUTOMATIC_IMAGE_HAVING_NEIGHBORDHOOD_HH
# define OLENA_AUTOMATIC_IMAGE_HAVING_NEIGHBORDHOOD_HH

# include <oln/core/typedefs.hh>
# include <oln/morphers/identity.hh>
# include <oln/core/abstract/image_dimension.hh>

namespace oln
{
  namespace automatic
  {

    /// Implementation corresponding to the interface
    /// oln::abstract::image1d for an identity morpher.
    template <abstract::image_having_neighborhood, morpher::tag::identity,
	      typename E>
    class impl
    {
    private:
      typedef oln_type_of(E, neighborhood) neighborhood_t;

    public:
      /// Accessor delegation.
      neighborhood_t impl_neighborhood()
      {
	return delegate().impl_neighborhood()

      }
    };

  } // end of namespace automatic
  
} // end of namespace oln

#endif // ! OLENA_AUTOMATIC_IMAGE_HAVING_NEIGHBORDHOOD_HH
