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

#ifndef MLN_CORE_IMAGE_GRAPH_WINDOW_IF_PITER_HH
# define MLN_CORE_IMAGE_GRAPH_WINDOW_IF_PITER_HH

/// \file
///
/// \brief Definition of a site iterator on a custom graph window.

# include <mln/core/concept/window.hh>
# include <mln/core/internal/is_masked_impl_selector.hh>
# include <mln/core/internal/site_relative_iterator_base.hh>


namespace mln
{

  // Forward declaration.
  template <typename S, typename I> class p_graph_piter;



  /// Forward iterator on line graph window.
  template <typename S, typename W, typename I>
  class graph_window_if_piter
    : public internal::site_relative_iterator_base< W,
						    graph_window_if_piter<S,W,I> >,
      public internal::is_masked_impl_selector< S,
						typename W::mask_t::domain_t,
						graph_window_if_piter<S,W,I> >
  {
    typedef graph_window_if_piter<S,W,I> self_;
    typedef internal::site_relative_iterator_base<W,self_> super_;

  public:
    /// Associated types
    /// \{
    typedef mln_result(S::fun_t) P;
    /// \}

    /// Construction.
    /// \{
    graph_window_if_piter();
    template <typename Pref>
    graph_window_if_piter(const Window<W>& win,
			  const Pref& p_ref);
    /// \}

    /// Delayed initialization.
    template <typename Pref>
    void init_(const Window<W>& win, const Pref& p_ref);

    /// Manipulation.
    /// \{
    /// Test if the iterator is valid.
    bool is_valid_() const;
    /// Invalidate the iterator.
    void invalidate_();

    /// Start an iteration.
    void do_start_();
    /// Go to the next point.
    void do_next_();

    /// Do some work while setting the reference site.
    template <typename Pref>
    void center_at_(const Pref& c);

    /// Do some work while setting the reference site.
    template <typename I2>
    void center_at_(const p_graph_piter<S, I2>& c);

    /// Return the graph element pointed by this iterator.
    const mln_graph_element(S)& element() const;

    /// Compute the current psite.
    mln_psite(W) compute_p_() const;

    /// Return the graph element id.
    /// FIXME: we do not want to have this member since there is an
    /// automatic conversion to the graph element.
    /// C++ does not seem to use this conversion operator.
    unsigned id() const;
    /// \}

  private:
    I iter_;

  protected:
    /// The underlying window.
    using super_::s_;
  };



# ifndef MLN_INCLUDE_ONLY

  template <typename S, typename W, typename I>
  inline
  graph_window_if_piter<S,W,I>::graph_window_if_piter()
  {
  }

  template <typename S, typename W, typename I>
  template <typename Pref>
  inline
  graph_window_if_piter<S,W,I>::graph_window_if_piter(const Window<W>& win,
						      const Pref& p_ref)
  {
    init_(win, p_ref);
  }

  template <typename S, typename W, typename I>
  template <typename Pref>
  inline
  void
  graph_window_if_piter<S,W,I>::init_(const Window<W>& win,
				      const Pref& p_ref)
  {
    this->center_at(p_ref);
    this->change_target(exact(win));

    mln_postcondition(!this->is_valid());
  }

  template <typename S, typename W, typename I>
  inline
  bool
  graph_window_if_piter<S,W,I>::is_valid_() const
  {
    return iter_.is_valid();
  }

  template <typename S, typename W, typename I>
  inline
  void
  graph_window_if_piter<S,W,I>::invalidate_()
  {
    iter_.invalidate();
  }

  template <typename S, typename W, typename I>
  inline
  void
  graph_window_if_piter<S,W,I>::do_start_()
  {
    iter_.start();
    while (iter_.is_valid() && this->is_masked(this->c_->element(), iter_))
      iter_.next();
  }

  template <typename S, typename W, typename I>
  inline
  void
  graph_window_if_piter<S,W,I>::do_next_()
  {
    iter_.next();
    while (iter_.is_valid() && this->is_masked(this->c_->element(), iter_))
      iter_.next();
  }

  template <typename S, typename W, typename I>
  template <typename Pref>
  inline
  void
  graph_window_if_piter<S,W,I>::center_at_(const Pref& c)
  {
    iter_.center_at(c.p_hook_());
  }

  template <typename S, typename W, typename I>
  template <typename I2>
  inline
  void
  graph_window_if_piter<S,W,I>::center_at_(const p_graph_piter<S, I2>& c)
  {
    iter_.center_at(c.hook_elt_());
  }

  template <typename S, typename W, typename I>
  inline
  mln_psite(W)
  graph_window_if_piter<S,W,I>::compute_p_() const
  {
    return mln_psite(S)(this->center().site_set(), iter_.id());
  }

  template <typename S, typename W, typename I>
  inline
  const mln_graph_element(S)&
  graph_window_if_piter<S,W,I>::element() const
  {
    return iter_;
  }

  template <typename S, typename W, typename I>
  inline
  unsigned
  graph_window_if_piter<S,W,I>::id() const
  {
    return iter_.id();
  }


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // ! MLN_CORE_IMAGE_GRAPH_WINDOW_IF_PITER_HH
