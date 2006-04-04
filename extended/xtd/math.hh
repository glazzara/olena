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

#ifndef EXTENDED_MATH_HH
# define EXTENDED_MATH_HH



// FIXME: preconditions are not handled in code included below...



/// \{
/// Definition of xtd cmath-like functions.

# include <xtd/math/abs.hh>
# include <xtd/math/acos.hh>
# include <xtd/math/asin.hh>
# include <xtd/math/atan.hh>
# include <xtd/math/atan2.hh>
# include <xtd/math/ceil.hh>
# include <xtd/math/cos.hh>
# include <xtd/math/cosh.hh>
# include <xtd/math/exp.hh>
// 'fabs' is obsolete
# include <xtd/math/floor.hh>
// fmod
// frexp
// ldexp
# include <xtd/math/log.hh>
# include <xtd/math/log10.hh>
// modf
# include <xtd/math/pow.hh>
# include <xtd/math/sin.hh>
# include <xtd/math/sinh.hh>
# include <xtd/math/sqrt.hh>
# include <xtd/math/tan.hh>
# include <xtd/math/tanh.hh>

/// \}



/// \{
/// Definition of xtd operators and related functions.

# include <xtd/math/arith.hh>
# include <xtd/math/logic.hh>
# include <xtd/math/cmp.hh>

/// \}



/// \{
/// Misc functions.

# include <xtd/math/id.hh>

/// \}




#endif // ! EXTENDED_MATH_HH
