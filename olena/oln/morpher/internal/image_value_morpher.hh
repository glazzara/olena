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

#ifndef OLN_MORPHER_INTERNAL_IMAGE_VALUE_MORPHER_HH
# define OLN_MORPHER_INTERNAL_IMAGE_VALUE_MORPHER_HH

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
      class image_value_morpher;

    } // end of namespace oln::morpher::internal
    
  } // end of namespace oln::morpher


  /// Super type.
  template <typename Image, typename Exact>
  struct set_super_type< morpher::internal::image_value_morpher<Image, Exact> >
  {
    typedef oln::image_entry<Exact> ret;
  };


  /// Vtypes.
  template <typename Image, typename Exact>
  struct vtypes< morpher::internal::image_value_morpher<Image, Exact> >
  {
    // Morpher type.
    typedef oln::morpher::tag::identity morpher_type; // FIXME: Wrong!

    typedef stc::not_delegated is_computed_type;
    typedef stc::not_delegated  value_type;
    typedef stc::not_delegated lvalue_type;
    typedef stc::not_delegated rvalue_type;

    /// Create an ``uplink'' from
    /// oln::morpher::internal::image_value_morpher to the morphed \a Image,
    /// so as to get all its virtual types.
    typedef Image delegatee_type;
  };


  namespace morpher
  {
    namespace internal
    {

      template <typename Image, typename Exact>
      class image_value_morpher : public oln::image_entry<Exact>
      {
      public:

	// FIXME: Hack here; constness should be properly handled.
	image_value_morpher(const Image& image);
	image_value_morpher(Image& image);

	const Image& delegate() const;
	Image& delegate();

      protected:
	Image image_;
      };



# ifndef OLN_INCLUDE_ONLY

      template <typename Image, typename Exact>
      image_value_morpher<Image, Exact>::image_value_morpher(const Image& image) :
	image_(const_cast<Image&>(image))
      {
      }

      template <typename Image, typename Exact>
      image_value_morpher<Image, Exact>::image_value_morpher(Image& image) :
	image_(image)
      {
      }

      template <typename Image, typename Exact>
      const Image&
      image_value_morpher<Image, Exact>::delegate() const
      {
	return image_;
      }

      template <typename Image, typename Exact>
      Image&
      image_value_morpher<Image, Exact>::delegate()
      {
	return image_;
      }

# endif

    } // end of namespace oln::morpher::internal
    
  } // end of namespace oln::morpher

} // end of namespace oln

#endif // ! OLN_MORPHER_INTERNAL_IMAGE_VALUE_MORPHER_HH
