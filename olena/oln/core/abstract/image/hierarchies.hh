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

#ifndef OLN_CORE_ABSTRACT_IMAGE_HIERARCHIES_HH
# define OLN_CORE_ABSTRACT_IMAGE_HIERARCHIES_HH

# include <oln/core/abstract/image.hh>


namespace oln
{

  typedef hierarchy<abstract::image, 1> image_hierarchy_wrt_dimension;
  typedef hierarchy<abstract::image, 2> image_hierarchy_wrt_type;
  typedef hierarchy<abstract::image, 3> image_hierarchy_wrt_neighborhood;
  typedef hierarchy<abstract::image, 4> image_hierarchy_wrt_bbox;
  typedef hierarchy<abstract::image, 5> image_hierarchy_wrt_accessibility;
  typedef hierarchy<abstract::image, 6> image_hierarchy_wrt_mutability;

  // FIXME: To be continued.
#if 0
  typedef hierarchy<abstract::image, 7> image_hierarchy_wrt_value;
  typedef hierarchy<abstract::image, 8> image_hierarchy_wrt_data_retrieval;
  // ...
#endif

} // end of namespace oln


// Hierarchy 1: topology w.r.t. dimension.
# include <oln/core/abstract/image/dimension/hierarchy.hh>

// Hierarchy 2: topology w.r.t. type of data.
# include <oln/core/abstract/image/type/hierarchy.hh>

// Hierarchy 3: topology w.r.t. neighborhood.
# include <oln/core/abstract/image/neighborhood/hierarchy.hh>

// Hierarchy 4: topology w.r.t. bounding box.
# include <oln/core/abstract/image/bbox/hierarchy.hh>

// Hierarchy 5: topology w.r.t. accessibility.
# include <oln/core/abstract/image/accessibility/hierarchy.hh>

// // // Hierarchy 6: topology w.r.t. data mutability.
# include <oln/core/abstract/image/mutability/hierarchy.hh>


#endif // ! OLN_CORE_ABSTRACT_IMAGE_HIERARCHIES_HH
