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

#ifndef MLN_CORE_BOX_PITER_HH
# define MLN_CORE_BOX_PITER_HH

/*! \file mln/core/box_piter.hh
 *
 * \brief Definition of iterators on points of boxes.
 */

# include <mln/core/internal/site_iterator_base.hh>
# include <mln/core/concept/box.hh>


namespace mln
{

  /*! \brief A generic forward iterator on points of boxes.
   *
   * The parameter \c P is the type of points.
   *
   * \see mln::box_
   */
  template <typename P>
  class box_fwd_piter_ : public internal::site_iterator_base_< box_<P>,
							       box_fwd_piter_<P> >
  {
    typedef box_fwd_piter_<P> self_;
    typedef internal::site_iterator_base_< box_<P>, self_ > super_;

  public:

    // Make definitions from super class available.
    enum { dim = P::dim };

    /*! \brief Constructor.
     *
     * \param[in] b A box.
     */
    box_fwd_piter_(const box_<P>& b);

    /// Test the iterator validity.
    bool is_valid_() const;

    /// Invalidate the iterator.
    void invalidate_();

    /// Start an iteration.
    void start_();

    /// Go to the next point.
    void next_();

    /// Give the site set target address.
    const box_<P>*& target_();

  private:

    using super_::p_;
    const box_<P>* b_;
  };



  /*! \brief A generic backward iterator on points of boxes.
   *
   * The parameter \c P is the type of points.
   *
   * \see mln::box_
   */
  template <typename P>
  class box_bkd_piter_ : public internal::site_iterator_base_< box_<P>,
							       box_bkd_piter_<P> >
  {
    typedef box_bkd_piter_<P> self_;
    typedef internal::site_iterator_base_< box_<P>, self_ > super_;

  public:

    // Make definitions from super class available.
    enum { dim = P::dim };

    /*! \brief Constructor.
     *
     * \param[in] b A box.
     */
    box_bkd_piter_(const box_<P>& b);

    /// Test the iterator validity.
    bool is_valid_() const;

    /// Invalidate the iterator.
    void invalidate_();

    /// Start an iteration.
    void start_();

    /// Go to the next point.
    void next_();

    /// Give the site set target address.
    const box_<P>*& target_();

  private:

    using super_::p_;
    const box_<P>* b_;
  };




# ifndef MLN_INCLUDE_ONLY


  // box_fwd_piter_<P>

  template <typename P>
  inline
  box_fwd_piter_<P>::box_fwd_piter_(const box_<P>& b)
  {
    change_target(b);
  }

  template <typename P>
  inline
  bool
  box_fwd_piter_<P>::is_valid_() const
  {
    return p_[0] != b_->pmax()[0] + 1;
  }

  template <typename P>
  inline
  void
  box_fwd_piter_<P>::invalidate_()
  {
    p_[0] = b_->pmax()[0] + 1;
  }

  template <typename P>
  inline
  void
  box_fwd_piter_<P>::start_()
  {
    p_ = b_->pmin();
  }

  template <typename P>
  inline
  void
  box_fwd_piter_<P>::next_()
  {
    for (int i = dim - 1; i >= 0; --i)
      if (p_[i] == b_->pmax()[i])
	p_[i] = b_->pmin()[i];
      else
	{
	  ++p_[i];
	  break;
	}
    if (p_ == b_->pmin())
      invalidate_();
  }

  template <typename P>
  inline
  const box_<P>*&
  box_fwd_piter_<P>::target_()
  {
    return b_;
  }


  // box_bkd_piter_<P>

  template <typename P>
  inline
  box_bkd_piter_<P>::box_bkd_piter_(const box_<P>& b)
  {
    change_target(&b);
  }

  template <typename P>
  inline
  bool
  box_bkd_piter_<P>::is_valid_() const
  {
    return p_[0] != b_->pmin()[0] - 1;
  }

  template <typename P>
  inline
  void
  box_bkd_piter_<P>::invalidate_()
  {
    p_[0] = b_->pmin()[0] - 1;
  }

  template <typename P>
  inline
  void
  box_bkd_piter_<P>::start_()
  {
    p_ = b_->pmax();
  }

  template <typename P>
  inline
  void
  box_bkd_piter_<P>::next_()
  {
    for (int i = dim - 1; i >= 0; --i)
      if (p_[i] == b_->pmin()[i])
	p_[i] = b_->pmax()[i];
      else
	{
	  --p_[i];
	  break;
	}
    if (p_ == b_->pmax())
      invalidate_();
  }

  template <typename P>
  inline
  const box_<P>*&
  box_bkd_piter_<P>::target_()
  {
    return b_;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_BOX_PITER_HH
