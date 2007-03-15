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

#ifndef OLN_CORE_2D_NEIGHB2D_HH
# define OLN_CORE_2D_NEIGHB2D_HH

# include <oln/core/gen/neighb.hh>
# include <oln/core/2d/dpoint2d.hh>


namespace oln
{

  // FIXME: neighb2d should be an actual type, not an alias...
  typedef neighb_<dpoint2d> neighb2d;


# ifdef OLN_ENABLE_DEFAULT

  // Default is c4.
  template <typename D>
  bool init_(neighb2d* this_, const D&);

# endif // OLN_ENABLE_DEFAULT


  namespace internal
  {

    neighb2d mk_c4();
    neighb2d mk_c8();
    neighb2d mk_c2_row();
    neighb2d mk_c2_col();


# ifndef OLN_INCLUDE_ONLY

    neighb2d mk_c4()
    {
      neighb2d tmp;
      tmp
	.take(dpoint2d(0, 1))
	.take(dpoint2d(1, 0));
      return tmp;
    }

    neighb2d mk_c8()
    {
      neighb2d tmp;
      tmp
	.take(dpoint2d(0, 1))
	.take(dpoint2d(1,-1))
	.take(dpoint2d(1, 0))
	.take(dpoint2d(1, 1));
      return tmp;
    }

    neighb2d mk_c2_row()
    {
      neighb2d tmp;
      tmp
	.take(dpoint2d(0, 1));
      return tmp;
    }

    neighb2d mk_c2_col()
    {
      neighb2d tmp;
      tmp
	.take(dpoint2d(1, 0));
      return tmp;
    }

# endif

  } // end of namespace oln::internal


  extern const neighb2d c4;
  extern const neighb2d c8;
  extern const neighb2d c2r;
  extern const neighb2d c2c;


# ifndef OLN_INCLUDE_ONLY

#  ifdef OLN_ENABLE_DEFAULT

    /// Default is c4.
    template <typename D>
    bool init_(neighb2d* this_, const D&)
    {
      *this_ = c4;
      return true;
    }

#  endif // OLN_ENABLE_DEFAULT

  const neighb2d c4  = internal::mk_c4();
  const neighb2d c8  = internal::mk_c8();
  const neighb2d c2r = internal::mk_c2_row();
  const neighb2d c2c = internal::mk_c2_col();

# endif // OLN_INCLUDE_ONLY


} // end of namespace oln



#endif // ! OLN_CORE_2D_NEIGHB2D_HH
