// Copyright (C) 2007, 2008, 2009, 2012, 2013 EPITA Research and
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

#ifndef MLN_CORE_IMAGE_DMORPH_HEXA_PITER_HH
# define MLN_CORE_IMAGE_DMORPH_HEXA_PITER_HH

/// \file
///
/// Definition of iterators on hexagonal points.

# include <mln/core/internal/piter_adaptor.hh>

# include <mln/core/alias/box2d.hh>
# include <mln/core/alias/box2d_h.hh>


namespace mln
{

  /*!
    \brief A generic forward iterator on points of subsets.

    Parameter \c S is a point set type; parameter F is a function
    from point to Boolean.

    \see mln::hexa
  */
  template <typename S>
  class hexa_fwd_piter_
    : public internal::piter_adaptor_< mln_fwd_piter(S),
				       S,
				       hexa_fwd_piter_<S> >
  {
    typedef mln_fwd_piter(S) adaptee_;
    typedef hexa_fwd_piter_<S> self_;
    typedef internal::piter_adaptor_<adaptee_, S, self_> super_;

  public:

    /// Constructor from a subset of points.
    hexa_fwd_piter_(const box2d_h& subset);

    /// Start an iteration.
    void start();

    /// \cond INTERNAL_API
    /// Go to the next point.
    void next_();
    /// \endcond

    operator point2d_h() const;
  private:

    point2d_h p_;

    //adaptee's box2d.
    box2d box_adaptee_;
  };


  // FIXME:
  template <typename S>
  class hexa_bkd_piter_
    :
    public mln::internal::fixme
  {};



# ifndef MLN_INCLUDE_ONLY


  // hexa_fwd_piter_<I>

  template <typename S>
  inline
  hexa_fwd_piter_<S>::hexa_fwd_piter_(const box2d_h& b)
    :
    super_(adaptee_(box_adaptee_))
  {
    box_adaptee_ = make::box2d(b.pmin()[0] / 2,
			       b.pmin()[1] / 2,
			       b.pmax()[0] / 2 + b.pmax()[0] % 2,
			       b.pmax()[1] / 2);
  }

  template <typename S>
  inline
  void
  hexa_fwd_piter_<S>::start()
  {
    this->pi_.start();
    p_[0] = this->pi_[0];
    p_[1] = this->pi_[1];
  }

  template <typename S>
  inline
  void
  hexa_fwd_piter_<S>::next_()
  {
    this->pi_.next();
    p_[0] = this->pi_[0] * 2;
    p_[1] = this->pi_[1] * 2 + this->pi_[0] % 2;
  }

  template <typename S>
  inline
  hexa_fwd_piter_<S>::operator point2d_h() const
  {
    return p_;
  }

  // FIXME: hexa_bkd_piter_<S>


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_DMORPH_HEXA_PITER_HH
