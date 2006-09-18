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

#ifndef OLENA_MORPHER_ADD_NEIGHBORHOOD
# define OLENA_MORPHER_ADD_NEIGHBORHOOD

# include <oln/morpher/internal/image_extension.hh>


namespace oln
{

  namespace morpher
  {
    // Forward declaration.
    template <typename Image>
    struct add_neighborhood;

    namespace tag
    {
      /// Tag associated to oln::morpher::add_neighborhood.
      struct add_neighborhood;
    
    } // end of namespace oln::morpher::tag

  } // end of namespace oln::morpher


  /// Super type.
  template <typename Image>
  struct set_super_type< morpher::add_neighborhood<Image> >
  {
    typedef morpher::add_neighborhood<Image> self_t;
    typedef morpher::internal::image_extension<Image, self_t> ret;
  };


  /// New virtual types associated with oln::morpher::add_neighborhood.
  /// \{
  template <typename Image>
  struct single_vtype < morpher::add_neighborhood<Image>,
			typedef_::morpher_type >
  {
    typedef oln::morpher::tag::add_neighborhood ret;
  };
  /// \}


  namespace morpher
  {
    /// Neighborhood addition morpher.
    template <typename Image>
    class add_neighborhood : public stc_get_supers(add_neighborhood<Image>)
    {
    private:
      typedef add_neighborhood<Image> self_t;
      typedef stc_get_nth_super(self_t, 1) super_t;
      typedef oln_type_of(Image, neighborhood) neighborhood_t;

    public:
      // FIXME: Handle the constness.
      add_neighborhood(const Image& image, const neighborhood_t& nbh) :
	super_t(image), nbh_(nbh)
      {
      }

      neighborhood_t impl_neighborhood() const
      {
	return nbh_;
      }

    protected:
      neighborhood_t nbh_;
    };

  } // end of namespace oln::morpher

} // end of namespace oln


// FIXME: This is probably not the right place for this.
// Where should we move this?
#include <oln/core/abstract/image.hh>
namespace oln
{
  /// Neighborhood-related definitions.
  /// \{
  // Forward declarations.
  template <typename T> struct image2d;
  struct neighborhood2d;
  namespace morpher {
    template <typename Image> struct add_neighborhood;
  }

  // External vtype.
  template <typename T>
  struct ext_vtype < morpher::add_neighborhood< oln::image2d<T> >,
		     typedef_::neighborhood_type >
  {
    typedef neighborhood2d ret;
  };
  /// \}
}


#endif // ! OLENA_MORPHER_ADD_NEIGHBORHOOD
