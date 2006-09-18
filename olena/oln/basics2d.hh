// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006 EPITA Research and
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

#ifndef OLENA_BASICS2D_HH
# define OLENA_BASICS2D_HH


# include <oln/core/2d/aliases.hh>

# include <oln/core/2d/point2d.hh>
namespace oln { template class point2d_<int>; }

# include <oln/core/2d/dpoint2d.hh>
namespace oln { template class dpoint2d_<int>; }

# include <oln/core/gen/bbox.hh>
namespace oln { template class bbox_<point2d>; }

# include <oln/core/gen/bbox_fwd_piter.hh>
namespace oln { template class bbox_fwd_piter_<point2d>; }

# include <oln/core/gen/bbox_bkd_piter.hh>
namespace oln { template class bbox_bkd_piter_<point2d>; }

# include <oln/core/gen/topo_bbox.hh>
namespace oln { template class topo_bbox_<point2d>; }


# include <oln/core/2d/image2d.hh>


#endif // ! OLENA_BASICS2D_HH
