// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CORE_ROUTINE_INIT_HH
# define MLN_CORE_ROUTINE_INIT_HH

/// \file
///
/// Definition of function that initialize an image from another
/// one.

# include <mln/tag/init.hh>
# include <mln/geom/bbox.hh>
# include <mln/border/find.hh>

# include <mln/core/routine/init.hxx>


namespace mln
{

  /// \cond INTERNAL_API

  // Forward declaration.

  template <typename Subject, typename T, typename M>
  void init_(Subject, T& target, const Object<M>& model);

  // Easy and final versions.

  template <typename B, typename I>
  void init_(tag::bbox_t, B& b, const Image<I>& ima);

  template <typename I>
  void init_(tag::border_t, unsigned& bdr, const Image<I>& ima);

  template <typename I>
  void init_(tag::domain_t, mln_domain(I)& pset, const Image<I>& ima);

  /// \endcond

# ifndef MLN_INCLUDE_ONLY


  template <typename Subject, typename T, typename M>
  inline
  void init_(Subject, T&, const Object<M>&)
  {
    struct ERROR err_; // FIXME: Explicit msg.
  }

  // Easy impl.

  template <typename B, typename I>
  inline
  void init_(tag::bbox_t, B& b, const Image<I>& ima)
  {
    b = geom::bbox(exact(ima).domain());
  }

  template <typename I>
  inline
  void init_(tag::border_t, unsigned& bdr, const Image<I>& ima)
  {
    // 'Find' means that we want a value; this is not always the
    // border thickness of 'ima', but it can be the thickness of
    // a morphed / underlying image over which ima is constructed.
    bdr = border::find(ima);
  }

  template <typename I>
  inline
  void init_(tag::domain_t, mln_domain(I)& pset, const Image<I>& ima)
  {
    pset = exact(ima).domain();
  }


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_ROUTINE_INIT_HH
