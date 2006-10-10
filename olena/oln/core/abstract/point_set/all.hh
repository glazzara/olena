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

#ifndef OLN_CORE_ABSTRACT_POINT_SET_ALL_HH
# define OLN_CORE_ABSTRACT_POINT_SET_ALL_HH

# include <oln/core/abstract/point_set.hh>

# include <oln/core/abstract/point_set/hierarchies.hh>

// Hierarchy 1: point set w.r.t. size.
# include <oln/core/abstract/point_set/point_set_having_known_size.hh>
// Hierarchy 2: point set w.r.t. accessibility.
# include <oln/core/abstract/point_set/point_set_being_random_accessible.hh>
// Hierarchy 3: point set w.r.t. bbox.
# include <oln/core/abstract/point_set/point_set_having_bbox.hh>
# include <oln/core/abstract/bbox.hh>
// Hierarchy 4: point set w.r.t. connexity.
# include <oln/core/abstract/point_set/point_set_being_connected.hh>

#endif // ! OLN_CORE_ABSTRACT_POINT_SET_ALL_HH
