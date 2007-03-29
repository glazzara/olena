// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef OLN_CORE_INTERNAL_F_PSET_TO_PLAIN_IMAGE_HH
# define OLN_CORE_INTERNAL_F_PSET_TO_PLAIN_IMAGE_HH

# include <oln/core/concept/point_set.hh>
# include <oln/core/internal/f_grid_to_plain_image.hh>


#define oln_f_pset_to_plain_image(Ps, T) typename oln::internal::f_pset_to_plain_image_< Ps, T >::ret


namespace oln
{

  /// \{
  /// Forward declarations.

  // point set types
  template <typename P> class gen_box;
  class box1d;
  class box2d;
  // FIXME: ...

  // gen
  struct such_as;
  struct restricted_to;
  template <typename L, typename OpName, typename R> class op_;

  /// \}



  namespace internal
  {

    // Fwd decl.

    template <typename Ps, typename T> struct f_pset_to_plain_image_;


    /// \{
    /// Definitions.

    template <typename Ps, typename T>
    struct pset_to_plain_image__
    {
      typedef oln_f_grid_to_plain_image(oln_grid(Ps), T) I;
      typedef op_< I, restricted_to, Ps > ret;
    };

    template <typename Ps, typename T>
    struct pset_to_plain_image__< const Ps, T >
      :
      public pset_to_plain_image__< Ps, T >
    {
    };

    template <typename T>
    struct pset_to_plain_image__< box1d, T >
    {
      typedef oln_f_grid_to_plain_image(grid1d, T) ret;
    };

    template <typename T>
    struct pset_to_plain_image__< box2d, T >
    {
      typedef oln_f_grid_to_plain_image(grid2d, T) ret;
    };

    // ...

    template <typename P, typename T>
    struct pset_to_plain_image__< gen_box<P>, T >
    {
      typedef oln_f_grid_to_plain_image(oln_grid(P), T) ret;
    };

    template <typename S, typename F, typename T>
    struct pset_to_plain_image__< op_<S, such_as, F>, T >
    {
      typedef op_<S, such_as, F> Ps;
      typedef typename f_pset_to_plain_image_<S, T>::ret I;
      typedef op_<I, restricted_to, Ps> ret;
    };

    /// \}


    template <typename Ps, typename T>
    struct f_pset_to_plain_image_ : private mlc::assert_< mlc_is_a(Ps, Point_Set) >,
				    public pset_to_plain_image__< Ps, T >
		      
    {
    };

  } // end of namespace oln::internal

} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_F_PSET_TO_PLAIN_IMAGE_HH
