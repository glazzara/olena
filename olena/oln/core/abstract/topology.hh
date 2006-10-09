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

#ifndef OLN_CORE_ABSTRACT_TOPOLOGY_HH
# define OLN_CORE_ABSTRACT_TOPOLOGY_HH

# include <oln/core/typedefs.hh>


namespace oln
{


  namespace abstract
  {


    /// Abstract topology class.
    template <typename E>
    class topology : public virtual stc::any__simple<E>,
		     public virtual oln::type
    {
    public:

      struct decl
      {
	oln_virtual_typedef(bbox);
	// for being bboxed; provides .bbox()

	oln_virtual_typedef(is_random_accessible);
	// provides .has(p) and .has_large(p)

	oln_virtual_typedef(point);
	oln_virtual_typedef(neighborhood);

	oln_virtual_typedef(morpher);

	decl();
      };

    protected:

      topology();

      ~topology();

    }; // end of class oln::abstract::topology<E>



# ifndef OLN_INCLUDE_ONLY

    template <typename E>
    topology<E>::decl::decl()
    {
      // constraint:
// 	  mlc::assert_< mlc::implies_< mlc::neq_< bbox, mlc::none >,
// 	                               mlc_is_a(bbox, abstract::bbox) > >::check();
    }

    template <typename E>
    topology<E>::topology()
    {
    }

    template <typename E>
    topology<E>::~topology()
    {
      decl();
    }

# endif


  } // end of namespace oln::abstract

} // end of namespace oln



# include <oln/core/abstract/topology_hierarchies.hh>



#endif // ! OLN_CORE_ABSTRACT_TOPOLOGY_HH
