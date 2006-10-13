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

#ifndef OLN_CORE_ABSTRACT_IMAGE_NEIGHBORHOOD_HIERARCHY_HH
# define OLN_CORE_ABSTRACT_IMAGE_NEIGHBORHOOD_HIERARCHY_HH

# include <oln/core/abstract/image.hh>
# include <oln/core/abstract/image/hierarchies.hh>
# include <oln/core/automatic/image/image_having_neighborhood.hh>


namespace oln
{
 
  /*---------------------------.
  | Neighborhood abstraction.  |
  `---------------------------*/

  namespace abstract
  {

    /// Image having a neighborhood.
    template <typename E>
    struct image_having_neighborhood :
      public virtual image<E>,
      public automatic::get_impl<image_having_neighborhood, E>
    {
    private:
      typedef oln_type_of(E, neighborhood) neighborhood_t;

    public:

      struct decl {
	// oln_virtual_typedef(niter);
	oln_virtual_typedef(fwd_niter);
	oln_virtual_typedef(bkd_niter);
      };

      const neighborhood_t& neighborhood() const;

    protected:

      /// Constructor (protected, empty).
      image_having_neighborhood();

      /// Destructor (protected).
      ~image_having_neighborhood();

    };



# ifndef OLN_INCLUDE_ONLY

    template <typename E>
    const typename image_having_neighborhood<E>::neighborhood_t&
    image_having_neighborhood<E>::neighborhood() const
    {
      return this->topo().neighborhood();
    }

    template <typename E>
    image_having_neighborhood<E>::image_having_neighborhood()
    {
    }
    
    template <typename E>
    image_having_neighborhood<E>::~image_having_neighborhood()
    {
      decl();
    }

# endif
 
  } // end of namespace oln::abstract


  /*----------------------.
  | Neighborhood switch.  |
  `----------------------*/

  /// With neighborhood.
  template <typename E>
  struct case_< image_hierarchy_wrt_neighborhood, E, 1 > :
    where_< mlc::neq_< oln_type_of(E, neighborhood), mlc::not_found > >
  {
    typedef abstract::image_having_neighborhood<E> ret;
  };

} // end of namespace oln


#endif // ! OLN_CORE_ABSTRACT_IMAGE_NEIGHBORHOOD_HIERARCHY_HH
