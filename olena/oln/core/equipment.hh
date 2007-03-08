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

# include <cassert>
# include <mlc/value.hh>
# include <mlc/contract.hh>
# include <oln/stc/scoop.hh> // FIXME: Remove "oln/" later.


namespace oln
{

# include <oln/stc/scoop.hxx> // FIXME: Remove "oln/" later.

  // b
  stc_decl_associated_type( bkd_niter );
  stc_decl_associated_type( bkd_piter );
  stc_decl_associated_type( bkd_qiter );
  stc_decl_associated_type( box );

  // c
  stc_decl_associated_type( coord );
  stc_decl_associated_type( ch_value );

  // d
  stc_decl_associated_type( data );
  stc_decl_associated_type( dim );
  stc_decl_associated_type( dpoint );

  // f
  stc_decl_associated_type( fwd_niter );
  stc_decl_associated_type( fwd_piter );
  stc_decl_associated_type( fwd_qiter );

  // g
  stc_decl_associated_type( grid );

  // i
  stc_decl_associated_type( index );

  // l
  stc_decl_associated_type( left );
  stc_decl_associated_type( lvalue );
  stc_decl_associated_type( lvaluep );

  // n
  stc_decl_associated_type( n );
  stc_decl_associated_type( nbh );
  stc_decl_associated_type( niter );

  // o
  stc_decl_associated_type( oper );
  stc_decl_associated_type( output );

  // p
  stc_decl_associated_type( piter );
  stc_decl_associated_type( point );
  stc_decl_associated_type( pset );
  stc_decl_associated_type( psite );

  // q
  stc_decl_associated_type( qiter );

  // r
  stc_decl_associated_type( right );
  stc_decl_associated_type( rvalue );
  stc_decl_associated_type( rvaluep );

  // s
  stc_decl_associated_type( std_container );

  // t
  stc_decl_associated_type( tag );

  // v
  stc_decl_associated_type( value );
  stc_decl_associated_type( vsite );


} // end of namespace oln



#endif // ! OLN_CORE_EQUIPMENT_HH

