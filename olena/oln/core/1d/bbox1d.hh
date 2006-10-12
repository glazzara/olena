// Copyright (C) 2006 EPITA Research and Development Laboratory
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

#ifndef OLN_CORE_1D_BBOX1D_HH
# define OLN_CORE_1D_BBOX1D_HH

# include <oln/core/1d/aliases.hh>
# include <oln/core/1d/point1d.hh>
# include <oln/core/gen/bbox.hh>

namespace oln
{

  // FIXME: Inexplicably, this explicit instantiation is required to
  // have topo_lbbox_<point1d> work.  See if we can get rid of it.
  template class bbox_<point1d>;

} // end of namespace oln


#endif // ! OLN_CORE_1D_BBOX1D_HH
