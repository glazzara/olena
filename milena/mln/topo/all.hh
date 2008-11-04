// Copyright (C) 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_TOPO_ALL_HH
# define MLN_TOPO_ALL_HH

/*! \file mln/topo/all.hh
 *
 * \brief File that includes all "point-wise" expression tools.
 */


namespace mln
{

  /// Namespace of "point-wise" expression tools.
  namespace topo {}

} // end of namespace mln


# include <mln/core/topo/adj_higher_dim_connected_n_face_iter.hh>
# include <mln/core/topo/adj_higher_face_iter.hh>
# include <mln/core/topo/adj_lower_dim_connected_n_face_iter.hh>
# include <mln/core/topo/adj_lower_face_iter.hh>
# include <mln/core/topo/adj_lower_higher_face_iter.hh>
# include <mln/core/topo/center_only_iter.hh>
# include <mln/core/topo/centered_iter_adapter.hh>
# include <mln/core/topo/complex.hh>
# include <mln/core/topo/complex_iterators.hh>
# include <mln/core/topo/face.hh>
# include <mln/core/topo/face_data.hh>
# include <mln/core/topo/face_iter.hh>
# include <mln/core/topo/n_face.hh>
# include <mln/core/topo/n_face_iter.hh>
# include <mln/core/topo/n_faces_set.hh>
# include <mln/core/topo/static_n_face_iter>

#endif // ! MLN_TOPO_ALL_HH
