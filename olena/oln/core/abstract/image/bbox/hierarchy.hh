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

#ifndef OLN_CORE_ABSTRACT_IMAGE_BBOX_HIERARCHY_HH
# define OLN_CORE_ABSTRACT_IMAGE_BBOX_HIERARCHY_HH

# include <oln/core/abstract/image.hh>
# include <oln/core/abstract/bbox.hh>


namespace oln
{
 
  /*-------------------------.
    | Dimension abstractions.  |
    `-------------------------*/

  namespace abstract
  {

    /// Image having a bbox.
    template <typename E>
    struct image_having_bbox :
      public virtual image<E>,
      public automatic::get_impl<image_having_bbox, E>
    {
    private:

      typedef oln_type_of(E, bbox)  bbox_t;
      typedef oln_type_of(E, point) point_t;

    public:

      struct decl
      {
	stc_virtual_typedef(bbox);

	decl();
      };

      // Concrete method.
      const bbox_t& bbox() const;

      // Concrete method.
      const point_t& pmin() const;

      // Concrete method.
      const point_t& pmax() const;

    protected:

      /// Constructor (protected, empty).
      image_having_bbox();

      /// Destructor.
      virtual ~image_having_bbox();
    };


# ifndef OLN_INCLUDE_ONLY

    template <typename E>
    image_having_bbox<E>::decl::decl()
    {
      mlc::assert_< mlc_is_a(bbox, abstract::bbox) >::check();
    }

    template <typename E>
    const typename image_having_bbox<E>::bbox_t&
    image_having_bbox<E>::bbox() const
    {
      return this->topo().bbox();
    }

    template <typename E>
    const typename image_having_bbox<E>::point_t&
    image_having_bbox<E>::pmin() const
    {
      return this->topo().bbox().pmin();
    }

    template <typename E>
    const typename image_having_bbox<E>::point_t&
    image_having_bbox<E>::pmax() const
    {
      return this->topo().bbox().pmax();
    }

    template <typename E>
    image_having_bbox<E>::image_having_bbox()
    {
    }

    template <typename E>
    image_having_bbox<E>::~image_having_bbox()
    {
      decl();
    }

# endif
 
  } // end of namespace oln::abstract


  /*-------------------.
    | Dimension switch.  |
    `-------------------*/

  /// With bbox.
  template <typename E>
  struct case_< image_hierarchy_wrt_bbox, E, 1 > :
    where_< mlc::neq_< oln_deduce_type_of(E, topo, bbox), mlc::not_found > >
  {
    typedef abstract::image_having_bbox<E> ret;
  };


  template <typename E> struct image_entry;


  template <typename E>
  struct single_vtype< image_entry<E>, typedef_::bbox_type >
  {
  private:
    typedef oln_type_of(E, topo) topo_t;
  public:
    typedef oln_type_of(topo_t, bbox) ret;
  };

} // end of namespace oln


#endif // ! OLN_CORE_ABSTRACT_IMAGE_BBOX_HIERARCHY_HH
