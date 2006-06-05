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

/// \file stc/valist_aggregator.hh
/// \brief Super class aggregation mechanism.
///
/// Converts a mlc::valist_ into a class inheriting from all the
/// elements of the list.

#ifndef STATIC_VALIST_AGGREGATOR_HH
# define STATIC_VALIST_AGGREGATOR_HH

// FIXME: To be moved in Metalic?

// FIXME: Propose a version where inheritance is virtual (with an
// optional fine-grained version, where `virtual' presence can be
// choose individually for each element?)

#include <mlc/abort.hh>
#include <mlc/valist.hh>

namespace stc
{
  namespace ERROR
  {

    struct PARAMETER_OF_internal_valist_aggregator_OUT_OF_BOUNDS;

  } // end of namespace stc::ERROR


  // There is a lot a redundant code here; the Boost Preprocessor
  // Library would be a nice solution, but we don't want to depend on
  // third-party libraries.


  /// Defaut version: abort.
  template <typename super_list>
  struct valist_aggregator :
    private mlc::abort_<
    super_list,
    stc::ERROR::PARAMETER_OF_internal_valist_aggregator_OUT_OF_BOUNDS
    >
  {
  };

  /// Version for 2 super classes.
  template <typename E1, typename E2>
  struct valist_aggregator< mlc::valist_ <E1,E2> > :
    public E1, public E2
  {
  };

  /// Version for 3 super classes.
  template <typename E1, typename E2, typename E3>
  struct valist_aggregator< mlc::valist_ <E1,E2,E3 > > :
    public E1, public E2, public E3
  {
  };

  /// Version for 4 super classes.
  template <typename E1, typename E2, typename E3, typename E4>
  struct valist_aggregator< mlc::valist_ <E1,E2,E3,E4> > :
    public E1, public E2, public E3, public E4
  {
  };

  /// Version for 5 super classes.
  template <typename E1, typename E2, typename E3, typename E4, typename E5>
  struct valist_aggregator< mlc::valist_ <E1,E2,E3,E4,E5> > :
    public E1, public E2, public E3, public E4, public E5
  {
  };

  /// Version for 6 super classes.
  template <typename E1, typename E2, typename E3, typename E4, typename E5,
	    typename E6>
  struct valist_aggregator< mlc::valist_ <E1,E2,E3,E4,E5,E6> > :
    public E1, public E2, public E3, public E4, public E5,
    public E6
  {
  };

  /// Version for 7 super classes.
  template <typename E1, typename E2, typename E3, typename E4, typename E5,
	    typename E6, typename E7>
  struct valist_aggregator< mlc::valist_ <E1,E2,E3,E4,E5,E6,E7> > :
    public E1, public E2, public E3, public E4, public E5,
    public E6, public E7
  {
  };

  /// Version for 8 super classes.
  template <typename E1, typename E2, typename E3, typename E4, typename E5,
	    typename E6, typename E7, typename E8>
  struct valist_aggregator< mlc::valist_ <E1,E2,E3,E4,E5,E6,E7,E8> > :
    public E1, public E2, public E3, public E4, public E5,
    public E6, public E7, public E8
  {
  };

  /// Version for 9 super classes.
  template <typename E1, typename E2, typename E3, typename E4, typename E5,
	    typename E6, typename E7, typename E8, typename E9>
  struct valist_aggregator< mlc::valist_ <E1,E2,E3,E4,E5,E6,E7,E8,E9> > :
    public E1, public E2, public E3, public E4, public E5,
    public E6, public E7, public E8, public E9
  {
  };
  /// \}

} // end of namespace stc

#endif // ! STATIC_VALIST_AGGREGATOR_HH
