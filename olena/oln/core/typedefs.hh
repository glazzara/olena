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

#ifndef OLN_CORE_TYPEDEFS_HH
# define OLN_CORE_TYPEDEFS_HH

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

  mlc_decl_typedef(index_comp_type);
  mlc_decl_typedef(row_comp_type);
  mlc_decl_typedef(col_comp_type);
  mlc_decl_typedef(slice_comp_type);


  /*------------.
  | Iterators.  |
  `------------*/

  mlc_decl_typedef(piter_type);
  mlc_decl_typedef(fwd_piter_type);
  mlc_decl_typedef(bkd_piter_type);

  mlc_decl_typedef(qiter_type);
  mlc_decl_typedef(fwd_qiter_type);
  mlc_decl_typedef(bkd_qiter_type);

  mlc_decl_typedef(niter_type);
  mlc_decl_typedef(fwd_niter_type);
  mlc_decl_typedef(bkd_niter_type);

  mlc_decl_typedef(fwd_viter_type);
  mlc_decl_typedef(bkd_viter_type);


  /*------------.
  | Point set.  |
  `------------*/

  mlc_decl_typedef(has_known_size_type);
  mlc_decl_typedef(is_random_accessible_type);
  mlc_decl_typedef(is_connected_type);


  /*--------.
  | Image.  |
  `--------*/

  mlc_decl_typedef(is_computed_type);
  mlc_decl_typedef(value_type);
  mlc_decl_typedef(rvalue_type);
  mlc_decl_typedef(lvalue_type);

  mlc_decl_typedef(psite_type);
  mlc_decl_typedef(point_type);
  mlc_decl_typedef(dpoint_type);

  mlc_decl_typedef(neighborhood_type);
  mlc_decl_typedef(subset_type);

  mlc_decl_typedef(delegated_type); // FIXME: soon obsolete
  mlc_decl_typedef(topo_type);
  mlc_decl_typedef(bbox_type);


  /*-------.
  | Grid.  |
  `-------*/

  // FIXME: Merge with dim_type?
  mlc_decl_typedef(dimvalue_type);
  mlc_decl_typedef(coord_type);


  /*--------.
  | Point.  |
  `--------*/

  mlc_decl_typedef(dim_type);
  mlc_decl_typedef(vec_type);


  /*---------.
  | Values.  |
  `---------*/

  mlc_decl_typedef(value_proxy_type);
  mlc_decl_typedef(mutable_value_proxy_type);


  /*----------------.
  | Look-up table.  |
  `----------------*/

  mlc_decl_typedef(lut_type);
  mlc_decl_typedef(lut_iter_type);


} // end of namespace oln



/// Shortcuts formed such as "oln_something(T) means
/// oln_vtype(T, something)".
/// \{

# define oln_dim(T)   oln_vtype(T,  dim)
# define oln_dim_(T)  oln_vtype_(T, dim)
 
# define oln_vec(T)   oln_vtype(T,  vec)
# define oln_vec_(T)  oln_vtype_(T, vec)
 
# define oln_bbox(T)    oln_vtype(T,  bbox)
# define oln_bbox_(T)   oln_vtype_(T, bbox)

# define oln_coord(T)   oln_vtype(T,  coord)
# define oln_coord_(T)  oln_vtype_(T, coord)

# define oln_grid(T)    oln_vtype(T,  grid)
# define oln_grid_(T)   oln_vtype_(T, grid)

# define oln_topo(T)    oln_vtype(T,  topo)
# define oln_topo_(T)   oln_vtype_(T, topo)

# define oln_point(T)   oln_vtype(T,  point)
# define oln_point_(T)  oln_vtype_(T, point)

# define oln_psite(T)   oln_vtype(T,  psite)
# define oln_psite_(T)  oln_vtype_(T, psite)

# define oln_dpoint(T)  oln_vtype(T,  dpoint)
# define oln_dpoint_(T) oln_vtype_(T, dpoint)

# define oln_is_computed(T)   oln_vtype(T,  is_computed)
# define oln_is_computed_(T)  oln_vtype_(T, is_computed)

# define oln_value(T)   oln_vtype(T,  value)
# define oln_value_(T)  oln_vtype_(T, value)

# define oln_rvalue(T)   oln_vtype(T,  rvalue)
# define oln_rvalue_(T)  oln_vtype_(T, rvalue)

# define oln_lvalue(T)   oln_vtype(T,  lvalue)
# define oln_lvalue_(T)  oln_vtype_(T, lvalue)

# define oln_fwd_piter(T)  oln_vtype(T,  fwd_piter)
# define oln_fwd_piter_(T) oln_vtype_(T, fwd_piter)

# define oln_bkd_piter(T)  oln_vtype(T,  bkd_piter)
# define oln_bkd_piter_(T) oln_vtype_(T, bkd_piter)

# define oln_fwd_qiter(T)  oln_vtype(T,  fwd_qiter)
# define oln_fwd_qiter_(T) oln_vtype_(T, fwd_qiter)

# define oln_bkd_qiter(T)  oln_vtype(T,  bkd_qiter)
# define oln_bkd_qiter_(T) oln_vtype_(T, bkd_qiter)

# define oln_fwd_niter(T)  oln_vtype(T,  fwd_niter)
# define oln_fwd_niter_(T) oln_vtype_(T, fwd_niter)

# define oln_bkd_niter(T)  oln_vtype(T,  bkd_niter)
# define oln_bkd_niter_(T) oln_vtype_(T, bkd_niter)

# define oln_fwd_viter(T)  oln_vtype(T,  fwd_viter)
# define oln_fwd_viter_(T) oln_vtype_(T, fwd_viter)

# define oln_bkd_viter(T)  oln_vtype(T,  bkd_viter)
# define oln_bkd_viter_(T) oln_vtype_(T, bkd_viter)

/// Even shorter shortcuts.
/// \{
# define oln_piter(T)  oln_vtype(T,  fwd_piter)
# define oln_piter_(T) oln_vtype_(T, fwd_piter)

# define oln_qiter(T)  oln_vtype(T,  fwd_qiter)
# define oln_qiter_(T) oln_vtype_(T, fwd_qiter)

# define oln_niter(T)  oln_vtype(T,  fwd_niter)
# define oln_niter_(T) oln_vtype_(T, fwd_niter)

# define oln_viter(T)  oln_vtype(T,  fwd_viter)
# define oln_viter_(T) oln_vtype_(T, fwd_viter)
/// \}

/// \}


// Fwd decls.
namespace oln {
  namespace type_fun {
    template <typename T> struct plain;
    template <typename Image, typename Value> struct ch_value;
  }
}


/// \{
/// Shortcuts for functions.

# define oln_plain(T)   typename oln::type_fun::plain<T>::ret
# define oln_plain_(T)           oln::type_fun::plain<T>::ret

# define oln_plain_value(I, V)  typename oln::type_fun::ch_value< oln_plain(I), V >::ret
# define oln_plain_value_(I, V)          oln::type_fun::ch_value< oln_plain(I), V >::ret

/// \}




# include <oln/core/abstract/entry.hh>


#endif // ! OLN_CORE_TYPEDEFS_HH
