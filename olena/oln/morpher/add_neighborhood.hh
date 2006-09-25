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
# include <oln/morpher/tags.hh>
# include <oln/core/2d/aliases.hh>


namespace oln
{

  namespace morpher
  {
    // Forward declaration.
    template <typename Image> struct add_neighborhood;

  } // end of namespace oln::morpher


  /// Super type.
  template <typename Image>
  struct set_super_type< morpher::add_neighborhood<Image> >
  {
    typedef morpher::add_neighborhood<Image> self_t;
    typedef morpher::internal::image_extension<Image, self_t> ret;
  };

  template <typename Image>
  struct vtypes< morpher::add_neighborhood<Image> >
  {
    // Morpher type.
    typedef oln::morpher::tag::add_neighborhood morpher_type;

    // Neighborhood type.
    typedef
      mlc_if( mlc_is_a_(Image, abstract::image2d), neighb2d, mlc::none )
      neighborhood_type;
  };


  namespace morpher
  {
    /// Neighborhood addition morpher.
    template <typename Image>
    class add_neighborhood : public stc_get_supers(add_neighborhood<Image>)
    {
    private:
      typedef add_neighborhood<Image> self_t;
      typedef stc_get_nth_super(self_t, 1) super_t;
      typedef oln_check_type_of(self_t, neighborhood) neighborhood_t;

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


#endif // ! OLENA_MORPHER_ADD_NEIGHBORHOOD
