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

#ifndef OLN_CORE_ABSTRACT_POINT_SET_HH
# define OLN_CORE_ABSTRACT_POINT_SET_HH

# include <oln/core/typedefs.hh>


namespace oln
{

  namespace abstract
  {


    /// Abstract point class.
    template <typename E>
    class point_set : public virtual stc::any__simple<E>,
		      public virtual oln::type
    {
    public:

      bool is_valid() const;

      struct decl
      {
	stc_virtual_typedef(point);

	stc_virtual_typedef(piter);
	stc_virtual_typedef(fwd_piter);
	stc_virtual_typedef(bkd_piter);

	stc_virtual_typedef(bbox);                 // provides .bbox()
	stc_virtual_typedef(is_random_accessible); // provides .has(p)
	stc_virtual_typedef(has_known_size);       // provides .npoints()
	stc_virtual_typedef(is_connected);         // provides, e.g., .nrows()

	// derived from point:
	stc_virtual_typedef(coord);
	stc_virtual_typedef(grid);
	stc_virtual_typedef(dim);

	decl();
      };

      ~point_set()
      {
      }

    protected:

      point_set();

    }; // end of class oln::abstract::point_set<E>


# ifndef OLN_INCLUDE_ONLY

    template <typename E>
    bool point_set<E>::is_valid() const
    {
      return this->exact().impl_is_valid();
    }

    template <typename E>
    point_set<E>::decl::decl()
    {
      // coherence check:
      mlc::assert_equal_< oln_type_of(fwd_piter, grid),
	                  oln_type_of(point,     grid) >::check();
      mlc::assert_equal_< oln_type_of(bkd_piter, grid),
	                  oln_type_of(point,     grid) >::check();
    }

    template <typename E>
    point_set<E>::point_set()
    {
    }

//     template <typename E>
//     point_set<E>::~point_set()
//     {
//       decl();
//     }

# endif


  } // end of namespace oln::abstract

} // end of namespace oln



# include <oln/core/abstract/point_set_hierarchies.hh>



#endif // ! OLN_CORE_ABSTRACT_POINT_SET_HH
