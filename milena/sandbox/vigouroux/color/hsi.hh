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

#ifndef NTG_COLOR_HSI_HH
# define NTG_COLOR_HSI_HH

# include <ntg/color/color.hh>

namespace ntg
{

  enum hsi_comp
    {
      hsi_H = 0,
      hsi_S = 1,
      hsi_I = 2
    };

  template<unsigned icomp> struct hsi_traits;
  template<> struct hsi_traits<hsi_H> : public interval<0,360> {};
  template<> struct hsi_traits<hsi_S>
  {
    static float lower_bound() { return 0.; }
    static float upper_bound() { return 0.816497; } //2 / sqrt(6)
  };

  template<> struct hsi_traits<hsi_I>
  {
    static float lower_bound() { return 0.; }
    static float upper_bound() { return 1.7320508; } //sqrt(3)
  };

  typedef color<3,8,hsi_traits>  hsi_8;
  typedef color<3,16,hsi_traits> hsi_16;
  typedef color<3,32,hsi_traits> hsi_32;

} // end of ntg.

#endif // !NTG_COLOR_HSI_HH
