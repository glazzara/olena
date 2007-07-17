// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef OLN_CORE_EQUIPMENT_HH
# define OLN_CORE_EQUIPMENT_HH

# include <mlc/contract.hh>
# ifndef OLENA_USE_SCOOP_ALT
#   include <stc/scoop.hh>
# else
#   include <stc/scoop-alt.hh>
# endif


# define oln_typename_shortcut__(Type, Alias) typename Type::Alias


# define oln_deduce(Type, Alias_1, Alias_2) typename Type::Alias_1::Alias_2


namespace oln
{

# ifndef OLENA_USE_SCOOP_ALT
#  include <stc/scoop.hxx>
# else
#  include <stc/scoop-alt.inc>
# endif

  // a

  stc_decl_associated_type( adapted );
  stc_decl_associated_type( argument );

# define oln_argument(T) oln_typename_shortcut__(T, argument)

  // b
  stc_decl_associated_type( bkd_niter );
  stc_decl_associated_type( bkd_piter );
  stc_decl_associated_type( bkd_qiter );
  stc_decl_associated_type( box );

# define oln_bkd_niter(T) oln_typename_shortcut__(T, bkd_niter)
# define oln_bkd_piter(T) oln_typename_shortcut__(T, bkd_piter)
# define oln_bkd_qiter(T) oln_typename_shortcut__(T, bkd_qiter)
# define oln_box(T)       oln_typename_shortcut__(T, box)

  // c
  stc_decl_associated_type( coord );
  stc_decl_associated_type( ch_value );

# define oln_coord(T) oln_typename_shortcut__(T, coord)

  // d
  stc_decl_associated_type( data );
  stc_decl_associated_type( dim );
  stc_decl_associated_type( dpoint );

# define oln_dim(T)    oln_typename_shortcut__(T, dim)
# define oln_dpoint(T) oln_typename_shortcut__(T, dpoint)

  // e
  stc_decl_associated_type( encoding );
# define oln_encoding(T) oln_typename_shortcut__(T, encoding)

  // f
  stc_decl_associated_type( fwd_niter );
  stc_decl_associated_type( fwd_piter );
  stc_decl_associated_type( fwd_qiter );

# define oln_fwd_niter(T) oln_typename_shortcut__(T, fwd_niter)
# define oln_fwd_piter(T) oln_typename_shortcut__(T, fwd_piter)
# define oln_fwd_qiter(T) oln_typename_shortcut__(T, fwd_qiter)

  // g
  stc_decl_associated_type( grid );

# define oln_grid(T) oln_typename_shortcut__(T, grid)


  // l
  stc_decl_associated_type( left );
  stc_decl_associated_type( lvalue );
  stc_decl_associated_type( lvaluep );

  // n
  stc_decl_associated_type( n );
  stc_decl_associated_type( nbh );
  stc_decl_associated_type( nbits );
  stc_decl_associated_type( niter );

# define oln_niter(T) oln_typename_shortcut__(T, niter)

  // o
  stc_decl_associated_type( oper );
  stc_decl_associated_type( output );
  stc_decl_associated_type( offset );

  // p
  stc_decl_associated_type( piter );
  stc_decl_associated_type( pixter );
  stc_decl_associated_type( plain );
  stc_decl_associated_type( point );
  stc_decl_associated_type( pset );
  stc_decl_associated_type( psite );

# define oln_piter(T) oln_typename_shortcut__(T, piter)
# define oln_plain(T) oln_typename_shortcut__(T, plain)
# define oln_point(T) oln_typename_shortcut__(T, point)
# define oln_pset(T)  oln_typename_shortcut__(T, pset)
# define oln_psite(T) oln_typename_shortcut__(T, psite)

  // q
  stc_decl_associated_type( qiter );

# define oln_qiter(T) oln_typename_shortcut__(T, qiter)

  // r
  stc_decl_associated_type( result );
  stc_decl_associated_type( right );
  stc_decl_associated_type( rvalue );
  stc_decl_associated_type( rvaluep );

# define oln_result(T) oln_typename_shortcut__(T, result)
# define oln_rvalue(T) oln_typename_shortcut__(T, rvalue)

  // s
  stc_decl_associated_type( std_container );
  stc_decl_associated_type( skeleton );

  // t
  stc_decl_associated_type( tag );

  // v
  stc_decl_associated_type( value );
  stc_decl_associated_type( vborder );
  stc_decl_associated_type( vsite );

# define oln_value(T) oln_typename_shortcut__(T, value)

  // w
  stc_decl_associated_type( weight );


} // end of namespace oln



/// FIXME: Bad!
/// \{

# include <oln/core/init.hh>

/// \}



#endif // ! OLN_CORE_EQUIPMENT_HH

