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

#ifndef NTG_NRGB_HH
# define NTG_NRGB_HH

# include <ntg/color/color.hh>

namespace ntg
{

  /*!
    NTSC RGB format.
  */

  enum nrgb_comp
    {
      nrgb_R = 0,
      nrgb_G = 1,
      nrgb_B = 2
    };

  template<unsigned icomp> struct nrgb_traits;
  template<>
  struct nrgb_traits<nrgb_R>
  {
    static float lower_bound() { return 0;}
    static float upper_bound() { return 1.0885004;}
  };


  template<>
  struct nrgb_traits<nrgb_G>
  {
    static float lower_bound() { return -0.3321984;}
    static float upper_bound() { return  1.3189264;}
  };


  template<>
  struct nrgb_traits<nrgb_B>
  {
    static float lower_bound() { return -0.0688480;}
    static float upper_bound() { return 0.904727;}
  };


  typedef color<3,8,nrgb_traits>  nrgb_8;
  typedef color<3,16,nrgb_traits> nrgb_16;
  typedef color<3,32,nrgb_traits> nrgb_32;

  // FIXME: not thread safe !
# define DEFINE_COLOR(Type, Name, V1, V2, V3)	\
  inline const Type& Name() {			\
    static const Type tmp(V1, V2, V3);		\
    return tmp;					\
  }

  namespace nrgb_8_color
  {

    DEFINE_COLOR(nrgb_8, white, 255, 255, 255)
    DEFINE_COLOR(nrgb_8, gray,  128, 128, 128)
    DEFINE_COLOR(nrgb_8, black, 0,   0,   0)
    DEFINE_COLOR(nrgb_8, red,   255, 0,   0)
    DEFINE_COLOR(nrgb_8, green, 0,   255, 0)
    DEFINE_COLOR(nrgb_8, blue,  0,   0,   255)
    DEFINE_COLOR(nrgb_8, yellow,255, 255, 0)
    DEFINE_COLOR(nrgb_8, cyan,  0,   255, 255)
    DEFINE_COLOR(nrgb_8, pink,  255, 0,   255)

  } // end of nrgb_8_color

  namespace nrgb_16_color
  {

    DEFINE_COLOR(nrgb_16, white, 255, 255, 255)
    DEFINE_COLOR(nrgb_16, gray,  128, 128, 128)
    DEFINE_COLOR(nrgb_16, black, 0,   0,   0)
    DEFINE_COLOR(nrgb_16, red,   255, 0,   0)
    DEFINE_COLOR(nrgb_16, green, 0,   255, 0)
    DEFINE_COLOR(nrgb_16, blue,  0,   0,   255)
    DEFINE_COLOR(nrgb_16, yellow,255, 255, 0)
    DEFINE_COLOR(nrgb_16, cyan,  0,   255, 255)
    DEFINE_COLOR(nrgb_16, pink,  255, 0,   255)

  } // end of nrgb_16_color

  namespace nrgb_32_color
  {

    DEFINE_COLOR(nrgb_32, white, 255, 255, 255)
    DEFINE_COLOR(nrgb_32, gray,  128, 128, 128)
    DEFINE_COLOR(nrgb_32, black, 0,   0,   0)
    DEFINE_COLOR(nrgb_32, red,   255, 0,   0)
    DEFINE_COLOR(nrgb_32, green, 0,   255, 0)
    DEFINE_COLOR(nrgb_32, blue,  0,   0,   255)
    DEFINE_COLOR(nrgb_32, yellow,255, 255, 0)
    DEFINE_COLOR(nrgb_32, cyan,  0,   255, 255)
    DEFINE_COLOR(nrgb_32, pink,  255, 0,   255)

  } // end of nrgb_32_color.

} // end of ntg.

#endif // !NTG_COLOR_NRGB_HH
