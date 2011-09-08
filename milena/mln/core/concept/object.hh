// Copyright (C) 2007, 2008, 2009, 2010, 2011 EPITA Research and
// Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_CORE_CONCEPT_OBJECT_HH
# define MLN_CORE_CONCEPT_OBJECT_HH

/// \file
///
/// Definition of the top Milena class mln::Object.

# include <cassert>
# include <iostream>

# include <mln/core/macros.hh>
# include <mln/core/category.hh>
# include <mln/core/contract.hh>
# include <mln/core/internal/fixme.hh>
# include <mln/trace/all.hh>

// metal
# include <mln/metal/abort.hh>
# include <mln/metal/is_a.hh>
# include <mln/metal/is_not_a.hh>
# include <mln/metal/is.hh>
# include <mln/metal/is_not.hh>
# include <mln/metal/equal.hh>
# include <mln/metal/not_equal.hh>
# include <mln/metal/converts_to.hh>
# include <mln/metal/ret.hh>
# include <mln/metal/unqualif.hh>
# include <mln/metal/math/all.hh>


/** \mainpage Documentation of milena

   \section intro_sec Introduction

   This is the documentation of Milena.

   \section mln_sec Overview of Milena.

   <UL>
   <LI> \ref mln
   <LI> \ref mln::accu
   <LI> \ref mln::algebra
   <LI> \ref mln::arith
   <LI> \ref mln::binarization
   <LI> \ref mln::border
   <LI> \ref mln::canvas
   <LI> \ref mln::convert
   <LI> \ref mln::data
   <LI> \ref mln::debug
   <LI> \ref mln::display
   <LI> \ref mln::draw
   <LI> \ref mln::estim
   <LI> \ref mln::extension
   <LI> \ref mln::fun
   <LI> \ref mln::geom
   <LI> \ref mln::graph
   <LI> \ref mln::histo
   <LI> \ref mln::io
   <LI> \ref mln::labeling
   <LI> \ref mln::data
   <LI> \ref mln::linear
   <LI> \ref mln::literal
   <LI> \ref mln::logical
   <LI> \ref mln::make
   <LI> \ref mln::math
   <LI> \ref mln::metal
   <LI> \ref mln::morpho
   <LI> \ref mln::norm
   <LI> \ref mln::opt
   <LI> \ref mln::pw
   <LI> \ref mln::registration
   <LI> \ref mln::set
   <LI> \ref mln::tag
   <LI> \ref mln::test
   <LI> \ref mln::topo
   <LI> \ref mln::trace
   <LI> \ref mln::trait
   <LI> \ref mln::transform
   <LI> \ref mln::util
   <LI> \ref mln::value
   <LI> \ref mln::win

   \section copyright Copyright and License.
   Copyright (C) 2007, 2008, 2009, 2010, 2011 EPITA Research and
   Development (LRDE)

   This documentation is part of Olena.

   Olena is free software: you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation, version 2 of the License.

   Olena is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Olena.  If not, see <http://www.gnu.org/licenses/>.
 */



/** \namespace mln
   \brief The namespace mln corresponds to the Milena (mini-Olena) project.

   FIXME: Re-write this description.

   The contents of mln mimics the contents of the olena project but
   in a simplified way.  Some classes have the same name in both
   projects and roughly have the same behavior.

   \warning The Milena project is independent from the Olena
   project; the user has to choose between both the project she
   wants to work with.
 */
namespace mln
{
  /// Implementation namespace of mln namespace.
  namespace impl {}

  /// Internal namespace of mln namespace.
  namespace internal
  {
    /// FIXME : doc
    namespace check {}
  }

  // Fwd decl.
  template <typename E> struct Object;

  // Object category flag type.
  template <>
  struct Object<void>
  {
    typedef Unknown<void> super;
  };


  /*! \brief Base class for almost every class defined in Milena.
   *
   * The parameter \a E is the exact type.
   */
  template <typename E>
  struct Object
  {
    typedef E exact_t;
    typedef Object<void> category; // Default.
  protected:
    Object();
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  inline
  Object<E>::Object()
  {
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


# include <mln/core/routine/exact.hh>
# include <mln/core/routine/ops.hh>


#endif // ! MLN_CORE_CONCEPT_OBJECT_HH
