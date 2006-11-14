// Copyright (C) 2001, 2002, 2003, 2004, 2006 EPITA Research and
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

  typedef neighb_<dpoint3d> neighb3d;


  namespace internal
  {

    neighb3d mk_c6();
    neighb3d mk_c18();
    neighb3d mk_c26();
    neighb3d mk_c4_slice();
    neighb3d mk_c8_slice();
    neighb3d mk_c4_row();
    neighb3d mk_c8_row();
    neighb3d mk_c4_col();
    neighb3d mk_c8_col();


# ifndef OLN_INCLUDE_ONLY

    neighb3d mk_c6()
    {
      static bool flower = true;
      static neighb3d the_;
      if (flower)
	{
	  the_
	    .add(dpoint3d(0, 0, 1))
	    .add(dpoint3d(0, 1, 0))
	    .add(dpoint3d(1, 0, 0));
	  flower = false;
	}
      return the_;
    }


    neighb3d mk_c18()
    {
      static bool flower = true;
      static neighb3d the_;
      if (flower)
	{
	  the_
	    .add(dpoint3d(0,  0,  1))
	    .add(dpoint3d(0,  1, -1))
	    .add(dpoint3d(0,  1,  0))
	    .add(dpoint3d(0,  1,  1))
	    .add(dpoint3d(1, -1,  0))
	    .add(dpoint3d(1,  0, -1))
	    .add(dpoint3d(1,  0,  0))
	    .add(dpoint3d(1,  0,  1))
	    .add(dpoint3d(1,  1,  0));
	  flower = false;
	}
      return the_;
    }


    neighb3d mk_c26()
    {
      static bool flower = true;
      static neighb3d the_;
      if (flower)
	{
	  the_
	    .add(dpoint3d(0,  0,  1))
	    .add(dpoint3d(0,  1, -1))
	    .add(dpoint3d(0,  1,  0))
	    .add(dpoint3d(0,  1,  1))
	    .add(dpoint3d(1, -1, -1))
	    .add(dpoint3d(1, -1,  0))
	    .add(dpoint3d(1, -1,  1))
	    .add(dpoint3d(1,  0, -1))
	    .add(dpoint3d(1,  0,  0))
	    .add(dpoint3d(1,  0,  1))
	    .add(dpoint3d(1,  1, -1))
	    .add(dpoint3d(1,  1,  0))
	    .add(dpoint3d(1,  1,  1));
	  flower = false;
	}
      return the_;
    }


    neighb3d mk_c4_slice()
    {
      static bool flower = true;
      static neighb3d the_;
      if (flower)
	{
	  the_
	    .add(dpoint3d(0, 0, 1))
	    .add(dpoint3d(0, 1, 0));
	  flower = false;
	}
      return the_;
    }


    neighb3d mk_c8_slice()
    {
      static bool flower = true;
      static neighb3d the_;
      if (flower)
	{
	  the_
	    .add(dpoint3d(0,  0,  1))
	    .add(dpoint3d(0,  1, -1))
	    .add(dpoint3d(0,  1,  0))
	    .add(dpoint3d(0,  1,  1));
	  flower = false;
	}
      return the_;
    }


    neighb3d mk_c4_row()
    {
      static bool flower = true;
      static neighb3d the_;
      if (flower)
	{
	  the_
	    .add(dpoint3d(0, 0, 1))
	    .add(dpoint3d(1, 0, 0));
	  flower = false;
	}
      return the_;
    }


    neighb3d mk_c8_row()
    {
      static bool flower = true;
      static neighb3d the_;
      if (flower)
	{
	  the_
	    .add(dpoint3d(0,  0,  1))
	    .add(dpoint3d(1,  0, -1))
	    .add(dpoint3d(1,  0,  0))
	    .add(dpoint3d(1,  0,  1));
	  flower = false;
	}
      return the_;
    }


    neighb3d mk_c4_col()
    {
      static bool flower = true;
      static neighb3d the_;
      if (flower)
	{
	  the_
	    .add(dpoint3d(0, 1, 0))
	    .add(dpoint3d(1, 0, 0));
	  flower = false;
	}
      return the_;
    }


    neighb3d mk_c8_col()
    {
      static bool flower = true;
      static neighb3d the_;
      if (flower)
	{
	  the_
	    .add(dpoint3d(0,  1,  0))
	    .add(dpoint3d(1, -1,  0))
	    .add(dpoint3d(1,  0,  0))
	    .add(dpoint3d(1,  1,  0));
	  flower = false;
	}
      return the_;
    }

# endif

  } // end of namespace oln::internal


  extern const neighb3d c6;
  extern const neighb3d c18;
  extern const neighb3d c26;

  extern const neighb3d c4_slice;
  extern const neighb3d c8_slice;
  extern const neighb3d c4_row;
  extern const neighb3d c8_row;
  extern const neighb3d c4_col;
  extern const neighb3d c8_col;


# ifndef OLN_INCLUDE_ONLY

  const neighb3d c6       = internal::mk_c6();
  const neighb3d c18      = internal::mk_c18();
  const neighb3d c26      = internal::mk_c26();

  const neighb3d c4_slice = internal::mk_c4_slice();
  const neighb3d c8_slice = internal::mk_c8_slice();
  const neighb3d c4_row   = internal::mk_c4_row();
  const neighb3d c8_row   = internal::mk_c8_row();
  const neighb3d c4_col   = internal::mk_c4_col();
  const neighb3d c8_col   = internal::mk_c8_col();

# endif

} // end of namespace oln


#endif // ! OLN_CORE_3D_NEIGHB3D_HH
