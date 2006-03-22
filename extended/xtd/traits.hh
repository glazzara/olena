// Copyright (C) 2002, 2005, 2006  EPITA Research and Development Laboratory
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
// the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
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

#ifndef EXTENDED_TRAITS_HH
# define EXTENDED_TRAITS_HH

# include <mlc/flags.hh>


# define xtd_plus(L, R) typename xtd::plus_trait_< L, R >::ret 
# define xtd_mult(L, R) typename xtd::mult_trait_< L, R >::ret 


namespace xtd
{

  template <typename L, typename R>
  struct plus_trait_ : public mlc::undefined
  {
  };

  template <typename L, typename R>
  struct mult_trait_ : public mlc::undefined
  {
  };

} // end of namespace xtd



#endif // ! EXTENDED_TRAITS_HH
