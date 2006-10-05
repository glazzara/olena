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

#ifndef OLN_MORPHER_INTERNAL_IMAGE_EXTENSION
# define OLN_MORPHER_INTERNAL_IMAGE_EXTENSION

# include <oln/core/image_entry.hh>
# include <oln/morpher/tags.hh>


namespace oln
{
  namespace morpher
  {
    namespace internal
    {

      // Forward declaration.
      template <typename Image, typename Exact>
      class image_extension;

    } // end of namespace oln::morpher::internal
    
  } // end of namespace oln::morpher


  /// Create an ``uplink'' from
  /// oln::morpher::internal::image_extension to the morphed \a Image,
  /// so as to get all its virtual types.
  template <typename Image, typename Exact>
  struct
  set_pseudosuper_type< morpher::internal::image_extension<Image, Exact> >
  {
    typedef Image ret;
  };


  template <typename Image, typename Exact>
  struct vtypes< morpher::internal::image_extension<Image, Exact> >
  {
    // Morpher type.
    typedef oln::morpher::tag::identity morpher_type;
  };


  namespace morpher
  {
    namespace internal
    {

      /// Image_Extension morpher.
      template <typename Image, typename Exact>
      class image_extension : public oln::image_entry<Exact>
      {
      public:

// 	template <typename I>
// 	image_extension(const abstract::image<I>& image)
// 	{
// 	  mlc::abort_<I>::check();
// 	}

	// FIXME: Handle the constness.
	image_extension(const Image& image) :
	  image_(image)
	{
	}

	const Image& delegate() const
	{
	  return image_;
	}

      protected:
	Image image_;
      };

    } // end of namespace oln::morpher::internal
    
  } // end of namespace oln::morpher

} // end of namespace oln

#endif // ! OLN_MORPHER_INTERNAL_IMAGE_EXTENSION
