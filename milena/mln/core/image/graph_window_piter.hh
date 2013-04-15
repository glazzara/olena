// Copyright (C) 2007, 2008, 2009, 2011, 2012, 2013 EPITA Research and
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

#ifndef MLN_CORE_IMAGE_GRAPH_WINDOW_PITER_HH
# define MLN_CORE_IMAGE_GRAPH_WINDOW_PITER_HH

/// \file
///
/// Definition of a graph element iterator on a graph window.
///
/// \todo Split into graph_window_piter, graph_mixed_window_piter and
/// graph_window_piter_base.

# include <mln/core/concept/site_set.hh>
# include <mln/core/concept/window.hh>
# include <mln/core/internal/site_relative_iterator_base.hh>


namespace mln
{

  // Forward declaration.
  template <typename S, typename I> class p_graph_piter;


  namespace internal
  {

    /*!
      \internal
      \brief The window center and the window elements are part of
      different site sets.
    */
    template <typename C, typename P, typename E>
    struct impl_selector
    {
      void do_start_()
      {
	internal::force_exact<E>(*this).iter_.start();
	// Nothing else to do. A pointer to the window element's site
	// set has been already stored in the constructor.
      }

    };

    /*!
      \internal
      \brief The window center and the window elements are part of the same
      site set.
    */
    template <typename C, typename E>
    struct impl_selector<C,C,E>
    {
      void do_start_()
      {
	E& this_ = internal::force_exact<E>(*this);
	this_.iter_.start();

	// We need to store a pointer on the site set of the window
	// center.
	// We cannot do this in the constructor since the center may
	// not be initialized.
	// Here we suppose that if we start this iterator, the
	// potential iterator used as center has been started just
	// before calling this method.
	//
	this_.change_target_site_set(this_.center().site_set());
      }

    };

  } // end of namespace mln::internal


  /*!
    \internal
    \brief Forward iterator on line graph window.

    \tparam S is the site set type.
    \tparam W is the window type.
    \tparam I is the underlying iterator type.
  */
  template <typename S, typename W, typename I>
  class graph_window_piter
    : public internal::site_relative_iterator_base< W,
						    graph_window_piter<S,W,I>,
						    typename W::center_t >,
      public internal::impl_selector<typename W::center_t, mln_psite(W),
				     graph_window_piter<S,W,I> >
  {
    typedef graph_window_piter<S,W,I> self_;
    typedef
    internal::site_relative_iterator_base<W,self_,mln_psite(S)> super_;
    typedef
    internal::impl_selector<typename W::center_t, mln_psite(W),
			    graph_window_piter<S,W,I> > super_impl_;


  public:
    /// Associated types
    /// \{
    /// Type of the window elements.
    typedef mln_result(S::fun_t) P;
    /// Type of the window center.
    typedef typename W::center_t center_t;
    /// Type of the graph element pointed by this iterator.
    typedef typename W::graph_element graph_element;
    /// \}

    /// Construction.
    /// \{
    graph_window_piter();


    // ---------------------------------------------------------
    // FIXME: probably ugly to provide constructors usable for
    //        specific cases only...
    // ---------------------------------------------------------

    /// To be used in case the center and neighbor sites have the same
    /// type and belong to the same site set.
    ///
    /// \param win             The underlying window.
    /// \param p_ref           Window center.
    //
    template <typename Pref>
    graph_window_piter(const Window<W>& win,
		       const Pref& p_ref);

    /// To be used in case center and neighbors sites do not have the
    /// same type and do not belong to the same site set.
    ///
    /// \param win             The underlying window.
    /// \param target_site_set Site set in which neighbor sites are
    ///                        extracted.
    /// \param p_ref           Window center.
    //
    template <typename Pref>
    graph_window_piter(const Window<W>& win,
		       const Site_Set<S>& target_site_set,
		       const Pref& p_ref);
    /// \}

    /// \cond INTERNAL_API
    /// Delayed initialization.
    /// \{
    template <typename Pref>
    void init_(const Window<W>& win, const Pref& p_ref);

    template <typename Pref>
    void init_(const Window<W>& win,
	       const Site_Set<S>& target_site_set,
	       const Pref& p_ref);
    /// \}

    /// Manipulation.
    /// \{
    /// Test if the iterator is valid.
    bool is_valid_() const;
    /// Invalidate the iterator.
    void invalidate_();

    /// Go to the next point.
    void do_next_();

    /// Do some work while setting the reference site.
    template <typename Pref>
    void center_at_(const Pref& c);

    /// Do some work while setting the reference site.
    template <typename S2, typename I2>
    void center_at_(const p_graph_piter<S2, I2>& c);

    /// Compute the current psite.
    mln_psite(W) compute_p_() const;
    /// \endcond

    /// Return the graph element pointed by this iterator.
    const graph_element& element() const;

    /// Return the graph element id.
    /// FIXME: we do not want to have this member since there is an
    /// automatic conversion to the graph element.
    /// C++ does not seem to use this conversion operator.
    unsigned id() const;
    /// \}

    /// Change the target site set.
    /// Window elements different from the center come from the target
    /// site set.
    void change_target_site_set(const S& s);
    /// Return the target site set.
    /// Window elements different from the center come from the target
    /// site set.
    const S& target_site_set() const;

    using super_impl_::do_start_;

    friend struct internal::impl_selector<typename W::center_t, mln_psite(W),
					  graph_window_piter<S,W,I> > ;

  private:
    I iter_;
    const S* s_;
  };


# ifndef MLN_INCLUDE_ONLY


  template <typename S, typename W, typename I>
  inline
  graph_window_piter<S,W,I>::graph_window_piter()
    : s_(0)
  {
  }


  template <typename S, typename W, typename I>
  template <typename Pref>
  inline
  graph_window_piter<S,W,I>::graph_window_piter(const Window<W>& win,
						const Pref& p_ref)
    : s_(0)
  {
    // Center and neighbor sites have the same type and belong to
    // the same site set.
    mlc_is(center_t, mln_psite(W))::check();
    init_(win, p_ref);
  }


  template <typename S, typename W, typename I>
  template <typename Pref>
  inline
  graph_window_piter<S,W,I>::graph_window_piter(const Window<W>& win,
						const Site_Set<S>& target_site_set,
						const Pref& p_ref)
    : s_(0)
  {
    // Center and neighbors sites do not have the same type and do
    // not belong to the same site set.
    mlc_is_not(center_t, mln_psite(W))::check();
    init_(win, target_site_set, p_ref);
  }

  template <typename S, typename W, typename I>
  template <typename Pref>
  inline
  void
  graph_window_piter<S,W,I>::init_(const Window<W>& win,
				   const Pref& p_ref)
  {
    this->center_at(p_ref);
    this->change_target(exact(win));

    mln_postcondition(!this->is_valid());
  }

  template <typename S, typename W, typename I>
  template <typename Pref>
  inline
  void
  graph_window_piter<S,W,I>::init_(const Window<W>& win,
				   const Site_Set<S>& target_site_set,
				   const Pref& p_ref)
  {
    this->center_at(p_ref);
    this->change_target(exact(win));
    change_target_site_set(exact(target_site_set));
    mln_postcondition(!this->is_valid());
  }


  template <typename S, typename W, typename I>
  inline
  bool
  graph_window_piter<S,W,I>::is_valid_() const
  {
    return s_ != 0 && s_->is_valid() && iter_.is_valid();
  }

  template <typename S, typename W, typename I>
  inline
  void
  graph_window_piter<S,W,I>::invalidate_()
  {
    iter_.invalidate();
  }

  template <typename S, typename W, typename I>
  inline
  void
  graph_window_piter<S,W,I>::do_next_()
  {
    iter_.next();
  }

  template <typename S, typename W, typename I>
  template <typename Pref>
  inline
  void
  graph_window_piter<S, W, I>::center_at_(const Pref& c)
  {
    iter_.center_at(c.p_hook_());
  }

  template <typename S, typename W, typename I>
  template <typename S2, typename I2>
  inline
  void
  graph_window_piter<S, W, I>::center_at_(const p_graph_piter<S2, I2>& c)
  {
    iter_.center_at(c.hook_elt_());
  }

  template <typename S, typename W, typename I>
  inline
  mln_psite(W)
  graph_window_piter<S,W,I>::compute_p_() const
  {
    return mln_psite(S)(target_site_set(), iter_.id());
  }

  template <typename S, typename W, typename I>
  inline
  const typename graph_window_piter<S,W,I>::graph_element&
  graph_window_piter<S, W, I>::element() const
  {
    return iter_;
  }

  template <typename S, typename W, typename I>
  inline
  unsigned
  graph_window_piter<S, W, I>::id() const
  {
    return iter_.id();
  }

  template <typename S, typename W, typename I>
  inline
  void
  graph_window_piter<S, W, I>::change_target_site_set(const S& s)
  {
    s_ = & s;
    mln_assertion(s_ != 0);
  }

  template <typename S, typename W, typename I>
  inline
  const S&
  graph_window_piter<S, W, I>::target_site_set() const
  {
    mln_precondition(s_ != 0);
    mln_precondition(s_->is_valid());
    return *s_;
  }


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // ! MLN_CORE_IMAGE_GRAPH_WINDOW_PITER_HH
