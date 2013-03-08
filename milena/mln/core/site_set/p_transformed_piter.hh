// Copyright (C) 2009, 2012 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_CORE_SITE_SET_P_TRANSFORMED_PITER_HH
# define MLN_CORE_SITE_SET_P_TRANSFORMED_PITER_HH

/// \file
///
/// Definition of iterators on p_transformed<S,F>.

# include <mln/core/internal/site_set_iterator_base.hh>
# include <mln/core/site_set/p_transformed.hh>
# include <mln/convert/to.hh>


namespace mln
{


  /*!
    \internal
    \brief Iterator on p_transformed<S,F>.

    Parameter \c S is a site set type; parameter F is a function
    from point to Boolean.

    \see mln::p_transformed
  */
  template <typename Pi, typename S, typename F>
  struct p_transformed_piter
    : public internal::site_set_iterator_base< p_transformed<S,F>,            // Site_Set.
					       p_transformed_piter<Pi,S,F> >  // Exact.
  {
    /// Constructor without argument.
    p_transformed_piter();

    /// Constructor from a site set.
    p_transformed_piter(const p_transformed<S,F>& s);

    /// Test the iterator validity.
    bool is_valid_() const;

    /// Invalidate the iterator.
    void invalidate_();

    /// Start an iteration.
    void start_();

    /// Go to the next point.
    void next_();

    /// Change the set site targeted by this iterator.
    void change_target(const p_transformed<S,F>& s);

  private:
    typedef p_transformed_piter<Pi,S,F> self_;
    typedef internal::site_set_iterator_base<p_transformed<S,F>, self_> super_;

  protected:
    using super_::s_;
    using super_::p_;

    // The underlying site iterator.
    Pi pi_;
  };



# ifndef MLN_INCLUDE_ONLY

  template <typename Pi, typename S, typename F>
  inline
  p_transformed_piter<Pi,S,F>::p_transformed_piter()
  {
  }

  template <typename Pi, typename S, typename F>
  inline
  p_transformed_piter<Pi,S,F>::p_transformed_piter(const p_transformed<S,F>& s)
  {
    this->change_target(s);
  }

  template <typename Pi, typename S, typename F>
  inline
  bool
  p_transformed_piter<Pi,S,F>::is_valid_() const
  {
    return pi_.is_valid();
  }

  template <typename Pi, typename S, typename F>
  inline
  void
  p_transformed_piter<Pi,S,F>::invalidate_()
  {
    pi_.invalidate();
  }

  template <typename Pi, typename S, typename F>
  inline
  void
  p_transformed_piter<Pi,S,F>::start_()
  {
    pi_.start();
    if (pi_.is_valid())
      p_ = s_->function()(convert::to<mln_argument(F)>(pi_));
  }

  template <typename Pi, typename S, typename F>
  inline
  void
  p_transformed_piter<Pi,S,F>::next_()
  {
    pi_.next();
    if (pi_.is_valid())
      p_ = s_->function()(convert::to<mln_argument(F)>(pi_));
  }

  template <typename Pi, typename S, typename F>
  inline
  void
  p_transformed_piter<Pi,S,F>::change_target(const p_transformed<S,F>& s)
  {
    s_ = & s;
    // p might be also updated since it can hold a pointer towards
    // the set it designates, so:
    pi_.change_target(s.primary_set());
    // Last:
    this->invalidate();
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_SITE_SET_P_TRANSFORMED_PITER_HH
