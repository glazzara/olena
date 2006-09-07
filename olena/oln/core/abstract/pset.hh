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

#ifndef OLENA_CORE_ABSTRACT_PSET_HH
# define OLENA_CORE_ABSTRACT_PSET_HH

# include <oln/core/typedefs.hh>


namespace oln
{

  namespace abstract
  {


    /// Abstract point class.
    template <typename E>
    class pset : public virtual stc::any__simple<E>,
		 public virtual oln::type
    {
    public:

      bool is_valid() const
      {
	return this->exact().impl_is_valid();
      }

      struct decl
      {
	stc_virtual_typedef(point);

	stc_virtual_typedef(piter);
	stc_virtual_typedef(fwd_piter);
	stc_virtual_typedef(bkd_piter);

	stc_virtual_typedef(bbox);  // for being bboxed;  provides .bbox()
	stc_virtual_typedef(ra);    // for random access; provides .has(p)
	stc_virtual_typedef(fixed); // for fixed size;    provides .npoints()
	stc_virtual_typedef(cnx);   // for connected;     provides, e.g., .nrows()

	// derived from point:
	stc_virtual_typedef(coord);
	stc_virtual_typedef(grid);
	stc_virtual_typedef(dim);

	decl() {
	  // coherence check:
	  mlc::assert_equal_< oln_type_of(fwd_piter, grid),
	                      oln_type_of(point,     grid) >::check();
	  mlc::assert_equal_< oln_type_of(bkd_piter, grid),
	                      oln_type_of(point,     grid) >::check();
	}
      };

    protected:

      pset()
      {}

      ~pset() { decl(); }

    }; // end of class oln::abstract::pset<E>



  } // end of namespace oln::abstract

} // end of namespace oln



# include <oln/core/abstract/pset_hierarchies.hh>



#endif // ! OLENA_CORE_ABSTRACT_PSET_HH
