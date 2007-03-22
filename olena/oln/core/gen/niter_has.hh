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

#ifndef OLN_CORE_GEN_NITER_HAS_HH
# define OLN_CORE_GEN_NITER_HAS_HH

# include <oln/core/internal/piter_adaptor.hh>
# include <oln/core/gen/dpoints_piter.hh>


namespace oln
{


# define current  niter_has_<It, Ps>
  //             --------------------

# define super internal::piter_adaptor_< current >


  // Fwd decl.
  template <typename It, typename Ps> class niter_has_;


  // Super type.
  template <typename It, typename Ps>
  struct super_trait_< current >
  {
    typedef super ret;
  };


  // Virtual type.
  template <typename It, typename Ps>
  struct vtypes< current >
  {
    typedef It adapted;
  };


  // Forward point iterator class on a set of dpoints.
  template <typename It, typename Ps>
  class niter_has_ : public super
  {
  public:

    // FIXME: Strenghten sigs (Pl is either a Point or an Iterator_on_Points).

    template <typename Pl, typename I>
    niter_has_(const Pl& p, const Image_with_Nbh<I>& ima);

    void impl_start();
    void impl_next();

  protected:

    const Ps pset_;

  }; // end of class oln::niter_has_<P>


  

# ifndef OLN_INCLUDE_ONLY

  template <typename It, typename Ps>
  template <typename Pl, typename I>
  current::niter_has_(const Pl& p, const Image_with_Nbh<I>& ima)
    :
    super( It(p, ima) ),
    pset_( ima.points() )
  {
  }

  template <typename It, typename Ps>
  void
  current::impl_start()
  {
    this->p_.start();
    while (this->p_.is_valid() and not this->pset_.has(this->p_))
      this->p_.next();
  }

  template <typename It, typename Ps>
  void
  current::impl_next()
  {
    do
      this->p_.next();
    while (this->p_.is_valid() and not this->pset_.has(this->p_));
  }

# endif // ! OLN_INCLUDE_ONLY

# undef super
# undef current


} // end of namespace oln


#endif // ! OLN_CORE_GEN_NITER_HAS_HH
