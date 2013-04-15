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

#ifndef MLN_CORE_SITE_SET_BOX_PITER_HH
# define MLN_CORE_SITE_SET_BOX_PITER_HH

/// \file
///
/// Definition of iterators on points of boxes.

# include <mln/core/internal/site_set_iterator_base.hh>
# include <mln/core/concept/box.hh>
# include <mln/core/site_set/box.hh>


namespace mln
{

  /*!
    \brief A generic forward iterator on points of boxes.

    The parameter \c P is the type of points.

    \see mln::box
  */
  template <typename P>
  class box_fwd_piter_ : public internal::site_set_iterator_base< box<P>,
								  box_fwd_piter_<P> >
  {
    typedef box_fwd_piter_<P> self_;
    typedef internal::site_set_iterator_base< box<P>, self_ > super_;

  public:

    // Make definitions from super class available.
    enum { dim = P::dim };

    /// Constructor without argument.
    box_fwd_piter_();

    /// Constructor.
    ///
    /// \param[in] b A box.
    ///
    box_fwd_piter_(const mln::box<P>& b);

    /// \cond INTERNAL_API
    /// Test the iterator validity.
    bool is_valid_() const;

    /// Invalidate the iterator.
    void invalidate_();

    /// Start an iteration.
    void start_();

    /// Go to the next point.
    void next_();
    /// \endcond

  protected:
    using super_::p_;
    using super_::s_;
  };



  /*!
    \brief A generic backward iterator on points of boxes.

    The parameter \c P is the type of points.

    \see mln::box
  */
  template <typename P>
  class box_bkd_piter_ : public internal::site_set_iterator_base< box<P>,
								  box_bkd_piter_<P> >
  {
    typedef box_bkd_piter_<P> self_;
    typedef internal::site_set_iterator_base< box<P>, self_ > super_;

  public:

    // Make definitions from super class available.
    enum { dim = P::dim };

    /// Constructor without argument.
    box_bkd_piter_();

    /// Constructor.
    ///
    /// \param[in] b A box.
    ///
    box_bkd_piter_(const box<P>& b);

    /// \cond INTERNAL_API
    /// Test the iterator validity.
    bool is_valid_() const;

    /// Invalidate the iterator.
    void invalidate_();

    /// Start an iteration.
    void start_();

    /// Go to the next point.
    void next_();
    /// \endcond

  protected:
    using super_::p_;
    using super_::s_;
  };




# ifndef MLN_INCLUDE_ONLY


  // box_fwd_piter_<P>

  template <typename P>
  inline
  box_fwd_piter_<P>::box_fwd_piter_()
  {
  }

  template <typename P>
  inline
  box_fwd_piter_<P>::box_fwd_piter_(const mln::box<P>& b)
  {
    this->change_target(b);
  }

  template <typename P>
  inline
  bool
  box_fwd_piter_<P>::is_valid_() const
  {
    return p_[0] != s_->pmax()[0] + 1;
  }

  template <typename P>
  inline
  void
  box_fwd_piter_<P>::invalidate_()
  {
    p_[0] = static_cast<def::coord>(s_->pmax()[0] + 1);
  }

  template <typename P>
  inline
  void
  box_fwd_piter_<P>::start_()
  {
    p_ = s_->pmin();
  }

  template <typename P>
  inline
  void
  box_fwd_piter_<P>::next_()
  {
    for (int i = dim - 1; i >= 0; --i)
      if (p_[i] != s_->pmax()[i])
	{
	  ++p_[i];
	  break;
	}
      else
	{
	  p_[i] = s_->pmin()[i];
	  if (i == 0)
	    invalidate_();
	}

    // memo

//     for (int i = dim - 1; i >= 0; --i)
//       if (p_[i] != s_->pmax()[i])
// 	{
// 	  ++p_[i];
// 	  break;
// 	}
//       else
// 	p_[i] = s_->pmin()[i];
//     if (p_ == s_->pmin())
//       invalidate_();
  }


  // box_bkd_piter_<P>

  template <typename P>
  inline
  box_bkd_piter_<P>::box_bkd_piter_()
  {
  }

  template <typename P>
  inline
  box_bkd_piter_<P>::box_bkd_piter_(const box<P>& b)
  {
    this->change_target(b);
  }

  template <typename P>
  inline
  bool
  box_bkd_piter_<P>::is_valid_() const
  {
    return p_[0] != s_->pmin()[0] - 1;
  }

  template <typename P>
  inline
  void
  box_bkd_piter_<P>::invalidate_()
  {
    p_[0] = mln_coord(P)(s_->pmin()[0] - 1);
  }

  template <typename P>
  inline
  void
  box_bkd_piter_<P>::start_()
  {
    p_ = s_->pmax();
  }

  template <typename P>
  inline
  void
  box_bkd_piter_<P>::next_()
  {
    for (int i = dim - 1; i >= 0; --i)
      if (p_[i] == s_->pmin()[i])
	p_[i] = s_->pmax()[i];
      else
	{
	  --p_[i];
	  break;
	}
    if (p_ == s_->pmax())
      invalidate_();
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_SITE_SET_BOX_PITER_HH
