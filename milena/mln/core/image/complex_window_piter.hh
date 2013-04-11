// Copyright (C) 2008, 2009, 2010, 2011, 2012, 2013 EPITA Research and
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

#ifndef MLN_CORE_IMAGE_COMPLEX_WINDOW_PITER_HH
# define MLN_CORE_IMAGE_COMPLEX_WINDOW_PITER_HH

/// \file
/// \brief  Definition of a site iterator on a complex window.

# include <mln/core/concept/window.hh>
# include <mln/core/internal/site_relative_iterator_base.hh>

// FIXME: These might be factor-able, both between fwd/bkd and nbh/win.

/* FIXME: Do we really want to inherit from
   internal::site_relative_iterator_base?  It might duplicate things,
   since most of the implementation of this iterator is delegated to
   the underlying complex iter.  Think about introducing base class
   replacement.  */


namespace mln
{

  /*------------------------------------.
  | complex_window_fwd_piter<I, G, W>.  |
  `------------------------------------*/

  /*!
    \internal
    \brief Forward iterator on complex window.
  */
  template <typename I, typename G, typename W>
  class complex_window_fwd_piter
    : public internal::site_relative_iterator_base< W,
						    complex_window_fwd_piter<I, G, W> >
  {
    typedef complex_window_fwd_piter<I, G, W> self_;
    typedef internal::site_relative_iterator_base< W, self_ > super_;

  public:
    /// The Pseudo_Site type.
    typedef mln_psite(W) psite;
    /// The type of the underlying complex iterator.
    typedef typename W::complex_fwd_iter iter_type;

  public:
    /// Construction.
    /// \{
    complex_window_fwd_piter();
    template <typename Pref>
    complex_window_fwd_piter(const Window<W>& win, const Pref& p_ref);
    /// \}

    /// \cond INTERNAL_API
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
    /// Go to the next site.
    void do_next_();

    /// Additional work while setting the reference site.
    template <typename Pref>
    void center_at_(const Pref& c);

    /// Compute the current psite.
    psite compute_p_() const;
    /// \}
    /// \endcond

    /// \brief Accessors.
    /// \{
    const iter_type& iter() const;
    iter_type& iter();
    /// \}

  private:
    /// The underlying complex iterator.
    iter_type iter_;
  };


  /// Print an mln::complex_window_fwd_piter.
  template <typename I, typename G, typename W>
  std::ostream&
  operator<<(std::ostream& ostr,
	     const complex_window_fwd_piter<I, G, W>& p);


  /*------------------------------------.
  | complex_window_bkd_piter<I, G, W>.  |
  `------------------------------------*/

  /*!
    \internal
    \brief Backward iterator on complex window.
  */
  template <typename I, typename G, typename W>
  class complex_window_bkd_piter
    : public internal::site_relative_iterator_base< W,
						    complex_window_bkd_piter<I, G, W> >
  {
    typedef complex_window_bkd_piter<I, G, W> self_;
    typedef internal::site_relative_iterator_base< W, self_ > super_;

  public:
    /// The Pseudo_Site type.
    typedef mln_psite(W) psite;
    /// The type of the underlying complex iterator.
    typedef typename W::complex_bkd_iter iter_type;

  public:
    /// Construction.
    /// \{
    complex_window_bkd_piter();
    template <typename Pref>
    complex_window_bkd_piter(const Window<W>& win, const Pref& p_ref);
    /// \}

    /// \cond INTERNAL_API
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
    /// Go to the next site.
    void do_next_();

    /// Additional work while setting the reference site.
    template <typename Pref>
    void center_at_(const Pref& c);

    /// Compute the current psite.
    psite compute_p_() const;
    /// \}
    /// \endcond

    /// \brief Accessors.
    /// \{
    const iter_type& iter() const;
    iter_type& iter();
    /// \}

  private:
    /// The underlying complex iterator.
    iter_type iter_;
  };


  /// Print an mln::complex_window_bkd_piter.
  template <typename I, typename G, typename W>
  std::ostream&
  operator<<(std::ostream& ostr,
	     const complex_window_bkd_piter<I, G, W>& p);



# ifndef MLN_INCLUDE_ONLY

  /*------------------------------------.
  | complex_window_fwd_piter<I, G, W>.  |
  `------------------------------------*/

  template <typename I, typename G, typename W>
  inline
  complex_window_fwd_piter<I, G, W>::complex_window_fwd_piter()
  {
  }

  template <typename I, typename G, typename W>
  template <typename Pref>
  inline
  complex_window_fwd_piter<I, G, W>::complex_window_fwd_piter(const Window<W>& win,
							      const Pref& p_ref)
  {
    init_(win, p_ref);
  }

  template <typename I, typename G, typename W>
  template <typename Pref>
  inline
  void
  complex_window_fwd_piter<I, G, W>::init_(const Window<W>& win,
					   const Pref& p_ref)
  {
    this->change_target(exact(win));
    this->center_at(p_ref);
    mln_postcondition(!this->is_valid());
  }


  template <typename I, typename G, typename W>
  inline
  bool
  complex_window_fwd_piter<I, G, W>::is_valid_() const
  {
    return iter_.is_valid();
  }

  template <typename I, typename G, typename W>
  inline
  void
  complex_window_fwd_piter<I, G, W>::invalidate_()
  {
    iter_.invalidate();
  }

  template <typename I, typename G, typename W>
  inline
  void
  complex_window_fwd_piter<I, G, W>::do_start_()
  {
    iter_.start();
  }

  template <typename I, typename G, typename W>
  inline
  void
  complex_window_fwd_piter<I, G, W>::do_next_()
  {
    iter_.next();
  }

  template <typename I, typename G, typename W>
  template <typename Pref>
  inline
  void
  complex_window_fwd_piter<I, G, W>::center_at_(const Pref&)
  {
    iter_.center_at(this->center().face());
  }

  template <typename I, typename G, typename W>
  inline
  mln_psite(W)
  complex_window_fwd_piter<I, G, W>::compute_p_() const
  {
    return psite(this->center().site_set(), iter_);
  }

  template <typename I, typename G, typename W>
  inline
  const typename W::complex_fwd_iter&
  complex_window_fwd_piter<I, G, W>::iter() const
  {
    return iter_;
  }

  template <typename I, typename G, typename W>
  inline
  typename W::complex_fwd_iter&
  complex_window_fwd_piter<I, G, W>::iter()
  {
    return iter_;
  }


  template <typename I, typename G, typename W>
  inline
  std::ostream&
  operator<<(std::ostream& ostr,
	     const complex_window_fwd_piter<I, G, W>& p)
  {
    return ostr << p.unproxy_();
  }


  /*------------------------------------.
  | complex_window_bkd_piter<I, G, W>.  |
  `------------------------------------*/

  template <typename I, typename G, typename W>
  inline
  complex_window_bkd_piter<I, G, W>::complex_window_bkd_piter()
  {
  }

  template <typename I, typename G, typename W>
  template <typename Pref>
  inline
  complex_window_bkd_piter<I, G, W>::complex_window_bkd_piter(const Window<W>& win,
							      const Pref& p_ref)
  {
    init_(win, p_ref);
  }

  template <typename I, typename G, typename W>
  template <typename Pref>
  inline
  void
  complex_window_bkd_piter<I, G, W>::init_(const Window<W>& win,
					   const Pref& p_ref)
  {
    this->change_target(exact(win));
    this->center_at(p_ref);
    mln_postcondition(!this->is_valid());
  }


  template <typename I, typename G, typename W>
  inline
  bool
  complex_window_bkd_piter<I, G, W>::is_valid_() const
  {
    return iter_.is_valid();
  }

  template <typename I, typename G, typename W>
  inline
  void
  complex_window_bkd_piter<I, G, W>::invalidate_()
  {
    iter_.invalidate();
  }

  template <typename I, typename G, typename W>
  inline
  void
  complex_window_bkd_piter<I, G, W>::do_start_()
  {
    iter_.start();
  }

  template <typename I, typename G, typename W>
  inline
  void
  complex_window_bkd_piter<I, G, W>::do_next_()
  {
    iter_.next();
  }

  template <typename I, typename G, typename W>
  template <typename Pref>
  inline
  void
  complex_window_bkd_piter<I, G, W>::center_at_(const Pref& c)
  {
    // FIXME: Argument C is not used here... Is this normal?
    (void) c;
    iter_.center_at(this->center().face());
  }

  template <typename I, typename G, typename W>
  inline
  mln_psite(W)
  complex_window_bkd_piter<I, G, W>::compute_p_() const
  {
    return psite(this->center().site_set(), iter_);
  }

  template <typename I, typename G, typename W>
  inline
  const typename W::complex_bkd_iter&
  complex_window_bkd_piter<I, G, W>::iter() const
  {
    return iter_;
  }

  template <typename I, typename G, typename W>
  inline
  typename W::complex_bkd_iter&
  complex_window_bkd_piter<I, G, W>::iter()
  {
    return iter_;
  }


  template <typename I, typename G, typename W>
  inline
  std::ostream&
  operator<<(std::ostream& ostr,
	     const complex_window_bkd_piter<I, G, W>& p)
  {
    return ostr << p.unproxy_();
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // ! MLN_CORE_IMAGE_COMPLEX_WINDOW_PITER_HH
