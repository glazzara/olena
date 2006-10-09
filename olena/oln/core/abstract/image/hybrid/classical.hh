// Copyright (C) 2005, 2006 EPITA Research and Development Laboratory
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

#ifndef OLN_CORE_ABSTRACT_IMAGE_HYBRID_CLASSICAL_HH
# define OLN_CORE_ABSTRACT_IMAGE_HYBRID_CLASSICAL_HH

# include <oln/core/abstract/image.hh>

# include <oln/core/abstract/image/dimension/2d.hh>
# include <oln/core/abstract/image/bbox/hierarchy.hh>
# include <oln/core/abstract/image/accessibility/hierarchy.hh>


namespace oln
{

  namespace abstract
  {

    template <typename E>
    struct classical_image
      : public virtual abstract::image_having_bbox<E>,
	public virtual abstract::image_being_random_accessible<E>
    {
    protected:
      classical_image();
    };

    template <typename E>
    struct classical_2d_image
      : public virtual abstract::classical_image<E>,
	public virtual abstract::image2d<E>
    {
    protected:
      classical_2d_image();
    };


# ifndef OLN_INCLUDE_ONLY

    template <typename E>
    classical_image<E>::classical_image()
    {
    }

    template <typename E>
    classical_2d_image<E>::classical_2d_image()
    {
    }

# endif

  } // end of namespace oln::abstract


  /// 2-D case.
  template <typename E>
  struct case_< image_hybrid_hierarchy_wrt_classical, E, 1 > :
    where_< mlc::and_list_< mlc::eq_< oln_type_of(E, grid), oln::grid2d >,
			    mlc::eq_< oln_deduce_type_of(E, topo, is_random_accessible), mlc::true_ >,
			    mlc::neq_< oln_deduce_type_of(E, topo, bbox), mlc::not_found >
                           >
           >
  {
    typedef abstract::classical_2d_image<E> ret;
  };


  /// General case.
  template <typename E>
  struct case_< image_hybrid_hierarchy_wrt_classical, E, 2 > :
    where_< mlc::and_< mlc::eq_< oln_deduce_type_of(E, topo, is_random_accessible), mlc::true_ >,
		       mlc::neq_< oln_deduce_type_of(E, topo, bbox), mlc::not_found >
                      >
           >
  {
    typedef abstract::classical_image<E> ret;
  };


} // end of namespace oln


#endif // ! OLN_CORE_ABSTRACT_IMAGE_HYBRID_CLASSICAL_HH
