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

#ifndef OLENA_MORPHER_INTERNAL_IMAGE_EXTENSION
# define OLENA_MORPHER_INTERNAL_IMAGE_EXTENSION

# include <oln/core/image_entry.hh>


namespace oln
{
  namespace morpher
  {
    namespace internal
    {

      // Forward declaration.
      template <typename Image, typename Exact>
      struct image_extension;

    } // end of namespace oln::morpher::internal
    
  } // end of namespace oln::morpher


  // FIXME: Use set_super_type instead?
  /// Create an ``uplink'' from
  /// oln::morpher::internal::image_extension to the morphed \a Image,
  /// so as to get all its virtual types.
  template <typename Image, typename Exact>
  struct set_pseudosuper_type< morpher::internal::image_extension<Image,
								  Exact> >
  {
    typedef Image ret;
  };


  namespace morpher
  {
    namespace internal
    {

      /// Image_Extension morpher.
      template <typename Image, typename Exact>
      class image_extension :
	public oln::image_entry< image_extension<Image, Exact> >
      {
      private:
	typedef image_extension<Image, Exact> self_t;

	/// Type of morphed image.
	typedef Image image_t;

	typedef oln_type_of(self_t, topo) topo_t;
	typedef oln_type_of(self_t, value) value_t;
	typedef oln_type_of(self_t, point) point_t;

      public:
	// FIXME: Handle the constness.
	image_extension(const Image& image) :
	  image_(image)
	  {
	  }

	const Image& delegate() const
	{
	  return image_;
	}

	/// Delegations.
	/// \{
	const topo_t& impl_topo() const
	{
	  return delegate()->impl_topo();
	}

	value_t impl_op_read(const point_t& p) const
	{
	  return delegate()->impl_op_read(p);
	}

	bool impl_has(const point_t& p) const
	{
	  return delegate()->impl_has(p);
	}
	/// \}

      protected:
	const Image& image_;
      };

    } // end of namespace oln::morpher::internal
    
  } // end of namespace oln::morpher

} // end of namespace oln

#endif // ! OLENA_MORPHER_INTERNAL_IMAGE_EXTENSION
