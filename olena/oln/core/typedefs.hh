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

#ifndef OLENA_CORE_TYPEDEFS_HH
# define OLENA_CORE_TYPEDEFS_HH

# include <iostream>

# include <mlc/assert.hh>
# include <mlc/contract.hh>
# include <mlc/flags.hh>
# include <mlc/is_a.hh>
# include <mlc/cmp.hh>
# include <mlc/typedef.hh>
# include <mlc/value.hh>
# include <mlc/to_string.hh>

# include <stc/any.hh>
# include <stc/exact.hh>
# include <stc/scoop.hh>

# include <oln/core/type.hh>
// FIXME: Move to oln/core.
# include <oln/core/macros.hh>


/*----------------------.
| Namespace equipment.  |
`----------------------*/

// The virtual types facility is inserted here because it also
// triggers Metalic's typedef introspection equipment.
stc_scoop_equipment_for_namespace(oln);


namespace oln
{

  /*-------.
  | Misc.  |
  `-------*/

  mlc_decl_typedef(exact_type);
  mlc_decl_typedef(morpher_type);
  

  /*----------.
  | General.  |
  `----------*/

  mlc_decl_typedef(grid_type);


  /*------------.
  | Iterators.  |
  `------------*/

// --------------------------------------------------------------------
// FIXME: To be enabled later.
// --------------------------------------------------------------------
//   mlc_decl_typedef(iter_type);
//   mlc_decl_typedef(fwd_iter_type);
//   mlc_decl_typedef(bkd_iter_type);

  mlc_decl_typedef(piter_type);
  mlc_decl_typedef(fwd_piter_type);
  mlc_decl_typedef(bkd_piter_type);

//   mlc_decl_typedef(qiter_type);
//   mlc_decl_typedef(fwd_qiter_type);
//   mlc_decl_typedef(bkd_qiter_type);

//   mlc_decl_typedef(niter_type);
//   mlc_decl_typedef(fwd_niter_type);
//   mlc_decl_typedef(bkd_niter_type);
// --------------------------------------------------------------------


  /*----------------------.
  | category::point_set.  |
  `-----------------------*/

  mlc_decl_typedef(has_known_size_type);
  mlc_decl_typedef(is_random_accessible_type);
  mlc_decl_typedef(is_connected_type);


  /*------------------.
  | category::image.  |
  `------------------*/

// --------------------------------------------------------------------
// FIXME: To be enabled later.
// --------------------------------------------------------------------
//   mlc_decl_typedef(data_type);
// --------------------------------------------------------------------
  mlc_decl_typedef(value_type);
  mlc_decl_typedef(rvalue_type);
  mlc_decl_typedef(lvalue_type);
  mlc_decl_typedef(is_mutable_type);

// --------------------------------------------------------------------
// FIXME: To be enabled later.
// --------------------------------------------------------------------
//   mlc_decl_typedef(value_storage_type);
//   mlc_decl_typedef(storage_type);
// --------------------------------------------------------------------
  mlc_decl_typedef(psite_type);
  mlc_decl_typedef(point_type);
  mlc_decl_typedef(dpoint_type);

  mlc_decl_typedef(neighborhood_type);

// --------------------------------------------------------------------
// FIXME: To be enabled later.
// --------------------------------------------------------------------
//   mlc_decl_typedef(image_type);
// --------------------------------------------------------------------
  mlc_decl_typedef(concrete_type);

  mlc_decl_typedef(delegated_type);
  mlc_decl_typedef(size_type); // FIXME: To be removed.
  mlc_decl_typedef(topo_type);
  mlc_decl_typedef(bbox_type);
// --------------------------------------------------------------------
// FIXME: To be enabled later.
// --------------------------------------------------------------------
//   mlc_decl_typedef(window_type);
// --------------------------------------------------------------------

  // ------------------------------ //
  // Extension in image_operators.  //
  // ------------------------------ //

// --------------------------------------------------------------------
// FIXME: To be enabled later.
// --------------------------------------------------------------------
//   mlc_decl_typedef(output_type);
//   mlc_decl_typedef(input_type);
//   mlc_decl_typedef(input1_type);
//   mlc_decl_typedef(input2_type);
// --------------------------------------------------------------------


  /*-----------------.
  | category::grid.  |
  `-----------------*/

  mlc_decl_typedef(dimvalue_type);
  mlc_decl_typedef(coord_type);


  /*-----------------.
  | category::point. |
  `-----------------*/

  mlc_decl_typedef(dim_type);


  /*------------------------------------.
  | category::fun1 and category::fun2.  |
  `------------------------------------*/

// --------------------------------------------------------------------
// FIXME: To be enabled later.
// --------------------------------------------------------------------
//   mlc_decl_typedef(res_type);
//   mlc_decl_typedef(arg_type);
//   mlc_decl_typedef(left_type);
//   mlc_decl_typedef(right_type);
// --------------------------------------------------------------------

} // end of namespace oln


# include <oln/core/abstract/entry.hh>


#endif // ! OLENA_CORE_TYPEDEFS_HH
