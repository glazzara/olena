// Copyright (C) 2001, 2002, 2003, 2004, 2006, 2007 EPITA Research and
// Development Laboratory
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

#ifndef OLN_CORE_3D_NEIGHB3D_HH
# define OLN_CORE_3D_NEIGHB3D_HH

# include <oln/core/gen/neighb.hh>
# include <oln/core/3d/dpoint3d.hh>


namespace oln
{

  // FIXME: neighb3d should be an actual type, not an alias...
  typedef neighb_<dpoint3d> neighb3d;


# ifdef OLN_ENABLE_DEFAULT

  // Default is c6.
  template <typename D>
  bool init_(neighb3d* this_, const D&);

# endif // OLN_ENABLE_DEFAULT


  namespace internal
  {

    neighb3d mk_c6();
    neighb3d mk_c18();
    neighb3d mk_c26();

    neighb3d mk_c4_sli();
    neighb3d mk_c8_sli();

    neighb3d mk_c2_sli();
    neighb3d mk_c2_row();
    neighb3d mk_c2_col();


# ifndef OLN_INCLUDE_ONLY

    neighb3d mk_c6()
    {
      neighb3d tmp;
      tmp
	.take(dpoint3d(0, 0, 1))
	.take(dpoint3d(0, 1, 0))
	.take(dpoint3d(1, 0, 0));
      return tmp;
    }

    neighb3d mk_c18()
    {
      neighb3d tmp;
      tmp
	.take(dpoint3d(0,  0,  1))
	.take(dpoint3d(0,  1, -1))
	.take(dpoint3d(0,  1,  0))
	.take(dpoint3d(0,  1,  1))
	.take(dpoint3d(1, -1,  0))
	.take(dpoint3d(1,  0, -1))
	.take(dpoint3d(1,  0,  0))
	.take(dpoint3d(1,  0,  1))
	.take(dpoint3d(1,  1,  0));
      return tmp;
    }

    neighb3d mk_c26()
    {
      neighb3d tmp;
      tmp
	.take(dpoint3d(0,  0,  1))
	.take(dpoint3d(0,  1, -1))
	.take(dpoint3d(0,  1,  0))
	.take(dpoint3d(0,  1,  1))
	.take(dpoint3d(1, -1, -1))
	.take(dpoint3d(1, -1,  0))
	.take(dpoint3d(1, -1,  1))
	.take(dpoint3d(1,  0, -1))
	.take(dpoint3d(1,  0,  0))
	.take(dpoint3d(1,  0,  1))
	.take(dpoint3d(1,  1, -1))
	.take(dpoint3d(1,  1,  0))
	.take(dpoint3d(1,  1,  1));
      return tmp;
    }

    neighb3d mk_c4_sli()
    {
      neighb3d tmp;
      tmp
	.take(dpoint3d(0, 0, 1))
	.take(dpoint3d(0, 1, 0));
      return tmp;
    }

    neighb3d mk_c8_sli()
    {
      neighb3d tmp;
      tmp
	.take(dpoint3d(0,  0,  1))
	.take(dpoint3d(0,  1, -1))
	.take(dpoint3d(0,  1,  0))
	.take(dpoint3d(0,  1,  1));
      return tmp;
    }

    neighb3d mk_c2_sli()
    {
      neighb3d tmp;
      tmp
	.take(dpoint3d(1, 0, 0));
      return tmp;
    }

    neighb3d mk_c2_row3d()
    {
      neighb3d tmp;
      tmp
	.take(dpoint3d(0, 1, 0));
      return tmp;
    }

    neighb3d mk_c2_col3d()
    {
      neighb3d tmp;
      tmp
	.take(dpoint3d(0, 0, 1));
      return tmp;
    }

# endif // ! OLN_INCLUDE_ONLY

  } // end of namespace oln::internal


  extern neighb3d c6;
  extern neighb3d c18;
  extern neighb3d c26;

  extern neighb3d c4_sli;
  extern neighb3d c8_sli;

  extern neighb3d c2_sli;
  extern neighb3d c2_row3d;
  extern neighb3d c2_col3d;


# ifndef OLN_INCLUDE_ONLY

#  ifdef OLN_ENABLE_DEFAULT

  /// Default is c6.
  template <typename D>
  bool init_(neighb3d* this_, const D&)
  {
    *this_ = c6;
    return true;
  }

#  endif // OLN_ENABLE_DEFAULT

  neighb3d c6  = internal::mk_c6();
  neighb3d c18 = internal::mk_c18();
  neighb3d c26 = internal::mk_c26();

  neighb3d c4_sli = internal::mk_c4_sli();
  neighb3d c8_sli = internal::mk_c8_sli();

  neighb3d c2_sli   = internal::mk_c2_sli();
  neighb3d c2_row3d = internal::mk_c2_row3d();
  neighb3d c2_col3d = internal::mk_c2_col3d();

# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln


#endif // ! OLN_CORE_3D_NEIGHB3D_HH
