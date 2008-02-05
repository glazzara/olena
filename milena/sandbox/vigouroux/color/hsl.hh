// Copyright (C) 2001, 2002, 2003  EPITA Research and Development Laboratory
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

#ifndef NTG_COLOR_HSL_HH
# define NTG_COLOR_HSL_HH

# include <ntg/color/color.hh>

namespace ntg
{

  enum hsl_comp
    {
      hsl_H = 0,
      hsl_S = 1,
      hsl_L = 2
    };

  template<unsigned icomp> struct hsl_traits;
  template<> struct hsl_traits<hsl_H> : public interval<0,360> {};
  template<> struct hsl_traits<hsl_S> : public interval<0,1> {};
  template<> struct hsl_traits<hsl_L> : public interval<0,1> {};

  typedef color<3,8,hsl_traits>  hsl_8;
  typedef color<3,16,hsl_traits> hsl_16;
  typedef color<3,32,hsl_traits> hsl_32;

} // end of ntg

#endif // !NTG_COLOR_HSL_HH
