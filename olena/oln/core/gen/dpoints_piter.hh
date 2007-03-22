// Copyright (C) 2006, 2007 EPITA Research and Development Laboratory
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

#ifndef OLN_CORE_GEN_DPOINTS_PITER_HH
# define OLN_CORE_GEN_DPOINTS_PITER_HH

# include <oln/core/concept/window.hh>
# include <oln/core/concept/neighborhood.hh>
# include <oln/core/concept/image.hh>
# include <oln/core/internal/dpoints_piter_impl.hh>


namespace oln
{


  // ---------------------------  dpoints_fwd_piter_<P>



  // Fwd decl.
  template <typename P> class dpoints_fwd_piter_;


  // Super type.
  template <typename P>
  struct super_trait_< dpoints_fwd_piter_<P> >
  {
    typedef dpoints_fwd_piter_<P> current__;
    typedef Iterator_on_Points<current__> ret;
  };


  // Virtual type.
  template <typename P>
  struct vtypes< dpoints_fwd_piter_<P> >
  {
    typedef P point;
  };


  // Forward point iterator class on a set of dpoints.
  template <typename P>
  class dpoints_fwd_piter_ : public Iterator_on_Points< dpoints_fwd_piter_<P> >,
			     public internal::dpoints_fwd_piter_impl_<P>
  {
  public:

    // FIXME: Strenghten sigs (Pl is either a Point or an Iterator_on_Points).

    template <typename Pl, typename W>
    dpoints_fwd_piter_(const Pl& p, const Window<W>& win);

    template <typename Pl, typename I>
    dpoints_fwd_piter_(const Pl& p, const Image_with_Nbh<I>& ima);

    template <typename Pl, typename N>
    dpoints_fwd_piter_(const Pl& p, const Neighborhood<N>& nbh);

  }; // end of class oln::dpoints_fwd_piter_<P>



  // ---------------------------  dpoints_bkd_piter_<P>



  // Fwd decl.
  template <typename P> class dpoints_bkd_piter_;


  // Super type.
  template <typename P>
  struct super_trait_< dpoints_bkd_piter_<P> >
  {
    typedef dpoints_bkd_piter_<P> current__;
    typedef Iterator_on_Points<current__> ret;
  };


  // Virtual type.
  template <typename P>
  struct vtypes< dpoints_bkd_piter_<P> >
  {
    typedef P point;
  };


  /// Backward point iterator class on a set of dpoints.
  template <typename P>
  class dpoints_bkd_piter_ : public Iterator_on_Points< dpoints_bkd_piter_<P> >,
			     public internal::dpoints_bkd_piter_impl_<P>
  {
  public:

    // FIXME: Strenghten sigs (Pl is either a Point or an Iterator_on_Points).

    template <typename Pl, typename W>
    dpoints_bkd_piter_(const Pl& p, const Window<W>& win);

    template <typename Pl, typename I>
    dpoints_bkd_piter_(const Pl& p, const Image_with_Nbh<I>& ima);

    template <typename Pl, typename N>
    dpoints_bkd_piter_(const Pl& p, const Neighborhood<N>& nbh);

  }; // end of class oln::dpoints_bkd_piter_<P>

  

# ifndef OLN_INCLUDE_ONLY

  // fwd

  template <typename P>
  template <typename Pl, typename W>
  dpoints_fwd_piter_<P>::dpoints_fwd_piter_(const Pl& p, const Window<W>& win)
    :
    internal::dpoints_fwd_piter_impl_<P>(p, exact(win))
  {
  }

  template <typename P>
  template <typename Pl, typename I>
  dpoints_fwd_piter_<P>::dpoints_fwd_piter_(const Pl& p, const Image_with_Nbh<I>& ima)
    :
    internal::dpoints_fwd_piter_impl_<P>(p, ima.nbhood())
  {
  }

  template <typename P>
  template <typename Pl, typename N>
  dpoints_fwd_piter_<P>::dpoints_fwd_piter_(const Pl& p, const Neighborhood<N>& nbh)
    :
    internal::dpoints_fwd_piter_impl_<P>(p, exact(nbh))
  {
  }

  // bkd

  template <typename P>
  template <typename Pl, typename W>
  dpoints_bkd_piter_<P>::dpoints_bkd_piter_(const Pl& p, const Window<W>& win)
    :
    internal::dpoints_bkd_piter_impl_<P>(p, exact(win))
  {
  }

  template <typename P>
  template <typename Pl, typename I>
  dpoints_bkd_piter_<P>::dpoints_bkd_piter_(const Pl& p, const Image_with_Nbh<I>& ima)
    :
    internal::dpoints_bkd_piter_impl_<P>(p, ima.nbhood())
  {
  }

  template <typename P>
  template <typename Pl, typename N>
  dpoints_bkd_piter_<P>::dpoints_bkd_piter_(const Pl& p, const Neighborhood<N>& nbh)
    :
    internal::dpoints_bkd_piter_impl_<P>(p, exact(nbh))
  {
  }

# endif

} // end of namespace oln


#endif // ! OLN_CORE_GEN_DPOINTS_PITER_HH
