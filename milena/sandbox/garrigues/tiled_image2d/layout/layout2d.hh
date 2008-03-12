// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef layout2d_hh
#define layout2d_hh

#include "block.hh"
#include "layout/image2d/lrtb.hh"
#include "layout/page2d/lrtb.hh"

namespace mln
{

  template<unsigned width, unsigned height,
	   template<unsigned,unsigned> class ImageLayout = layout::image2d_lrtb,
	   template<unsigned,unsigned> class PageLayout = layout::page2d_lrtb >
  struct layout2d : public Object< layout2d<width, height> > {
    enum { dim = 2 , w = width, h = height };

    typedef ImageLayout<width, height> image_layout;
    typedef PageLayout<width, height> page_layout;

    template<typename T>
    struct block_ : block<T, width * height> {};

  };


} // end of namespace mln


#endif
