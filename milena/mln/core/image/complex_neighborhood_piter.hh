// Copyright (C) 2008, 2009, 2012, 2013 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_CORE_IMAGE_COMPLEX_NEIGHBORHOOD_PITER_HH
# define MLN_CORE_IMAGE_COMPLEX_NEIGHBORHOOD_PITER_HH

/// \file
/// Definition of a site iterator on a complex neighborhood.

# include <mln/core/concept/neighborhood.hh>
# include <mln/core/internal/site_relative_iterator_base.hh>

// FIXME: These might be factor-able, both between fwd/bkd and nbh/win.

/* FIXME: Do we really want to inherit from
   internal::site_relative_iterator_base?  It might duplicate things,
   since most of the implementation of this iterator is delegated to
   the underlying complex iter.  Think about introducing base class
   replacement.  */


namespace mln
{

  /*------------------------------------------.
  | complex_neighborhood_fwd_piter<I, G, N>.  |
  `------------------------------------------*/

  /*!
    \internal
    \brief Forward iterator on complex neighborhood.
  */
  template <typename I, typename G, typename N>
  class complex_neighborhood_fwd_piter
    : public internal::site_relative_iterator_base< N,
						    complex_neighborhood_fwd_piter<I, G, N> >
  {
    typedef complex_neighborhood_fwd_piter<I, G, N> self_;
    typedef internal::site_relative_iterator_base< N, self_ > super_;

  public:
    /// The Pseudo_Site type.
    typedef mln_psite(N) psite;
    /// The type of the underlying complex iterator.
    typedef typename N::complex_fwd_iter iter_type;

  public:
    /// Construction.
    /// \{
    complex_neighborhood_fwd_piter();
    template <typename Pref>
    complex_neighborhood_fwd_piter(const Neighborhood<N>& nbh,
				   const Pref& p_ref);
    /// \}

    /// \cond INTERNAL_API
    /// Delayed initialization.
    template <typename Pref>
    void init_(const Neighborhood<N>& nbh, const Pref& p_ref);

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


  /// Print an mln::complex_neighborhood_fwd_piter.
  template <typename I, typename G, typename N>
  std::ostream&
  operator<<(std::ostream& ostr,
	     const complex_neighborhood_fwd_piter<I, G, N>& p);


  /*------------------------------------------.
  | complex_neighborhood_bkd_piter<I, G, N>.  |
  `------------------------------------------*/

  /*!
    \internal
    \brief Backward iterator on complex neighborhood.
  */
  template <typename I, typename G, typename N>
  class complex_neighborhood_bkd_piter
    : public internal::site_relative_iterator_base< N,
						    complex_neighborhood_bkd_piter<I, G, N> >
  {
    typedef complex_neighborhood_bkd_piter<I, G, N> self_;
    typedef internal::site_relative_iterator_base< N, self_ > super_;

  public:
    /// The Pseudo_Site type.
    typedef mln_psite(N) psite;
    /// The type of the underlying complex iterator.
    typedef typename N::complex_bkd_iter iter_type;

  public:
    /// Construction.
    /// \{
    complex_neighborhood_bkd_piter();
    template <typename Pref>
    complex_neighborhood_bkd_piter(const Neighborhood<N>& nbh,
				   const Pref& p_ref);
    /// \}

    /// \cond INTERNAL_API
    /// Delayed initialization.
    template <typename Pref>
    void init_(const Neighborhood<N>& nbh, const Pref& p_ref);

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


  /// Print an mln::complex_neighborhood_bkd_piter.
  template <typename I, typename G, typename N>
  std::ostream&
  operator<<(std::ostream& ostr,
	     const complex_neighborhood_bkd_piter<I, G, N>& p);



# ifndef MLN_INCLUDE_ONLY

  /*------------------------------------------.
  | complex_neighborhood_fwd_piter<I, G, N>.  |
  `------------------------------------------*/

  template <typename I, typename G, typename N>
  inline
  complex_neighborhood_fwd_piter<I, G, N>::complex_neighborhood_fwd_piter()
  {
  }

  template <typename I, typename G, typename N>
  template <typename Pref>
  inline
  complex_neighborhood_fwd_piter<I, G, N>::complex_neighborhood_fwd_piter(const Neighborhood<N>& nbh,
									  const Pref& p_ref)
  {
    init_(nbh, p_ref);
  }

  template <typename I, typename G, typename N>
  template <typename Pref>
  inline
  void
  complex_neighborhood_fwd_piter<I, G, N>::init_(const Neighborhood<N>& nbh,
						 const Pref& p_ref)
  {
    this->change_target(exact(nbh));
    this->center_at(p_ref);
    mln_postcondition(!this->is_valid());
  }

  template <typename I, typename G, typename N>
  inline
  bool
  complex_neighborhood_fwd_piter<I, G, N>::is_valid_() const
  {
    return iter_.is_valid();
  }

  template <typename I, typename G, typename N>
  inline
  void
  complex_neighborhood_fwd_piter<I, G, N>::invalidate_()
  {
    iter_.invalidate();
  }

  template <typename I, typename G, typename N>
  inline
  void
  complex_neighborhood_fwd_piter<I, G, N>::do_start_()
  {
    iter_.start();
  }

  template <typename I, typename G, typename N>
  inline
  void
  complex_neighborhood_fwd_piter<I, G, N>::do_next_()
  {
    iter_.next();
  }

  template <typename I, typename G, typename N>
  template <typename Pref>
  inline
  void
  complex_neighborhood_fwd_piter<I, G, N>::center_at_(const Pref&)
  {
    iter_.center_at(this->center().face());
  }

  template <typename I, typename G, typename N>
  inline
  mln_psite(N)
  complex_neighborhood_fwd_piter<I, G, N>::compute_p_() const
  {
    return psite(this->center().site_set(), iter_);
  }

  template <typename I, typename G, typename N>
  inline
  const typename N::complex_fwd_iter&
  complex_neighborhood_fwd_piter<I, G, N>::iter() const
  {
    return iter_;
  }

  template <typename I, typename G, typename N>
  inline
  typename N::complex_fwd_iter&
  complex_neighborhood_fwd_piter<I, G, N>::iter()
  {
    return iter_;
  }


  template <typename I, typename G, typename N>
  inline
  std::ostream&
  operator<<(std::ostream& ostr,
	     const complex_neighborhood_fwd_piter<I, G, N>& p)
  {
    return ostr << p.unproxy_();
  }


  /*------------------------------------------.
  | complex_neighborhood_bkd_piter<I, G, N>.  |
  `------------------------------------------*/

  template <typename I, typename G, typename N>
  inline
  complex_neighborhood_bkd_piter<I, G, N>::complex_neighborhood_bkd_piter()
  {
  }

  template <typename I, typename G, typename N>
  template <typename Pref>
  inline
  complex_neighborhood_bkd_piter<I, G, N>::complex_neighborhood_bkd_piter(const Neighborhood<N>& nbh,
									  const Pref& p_ref)
  {
    init_(nbh, p_ref);
  }

  template <typename I, typename G, typename N>
  template <typename Pref>
  inline
  void
  complex_neighborhood_bkd_piter<I, G, N>::init_(const Neighborhood<N>& nbh,
						 const Pref& p_ref)
  {
    this->change_target(exact(nbh));
    this->center_at(p_ref);
    mln_postcondition(!this->is_valid());
  }

  template <typename I, typename G, typename N>
  inline
  bool
  complex_neighborhood_bkd_piter<I, G, N>::is_valid_() const
  {
    return iter_.is_valid();
  }

  template <typename I, typename G, typename N>
  inline
  void
  complex_neighborhood_bkd_piter<I, G, N>::invalidate_()
  {
    iter_.invalidate();
  }

  template <typename I, typename G, typename N>
  inline
  void
  complex_neighborhood_bkd_piter<I, G, N>::do_start_()
  {
    iter_.start();
  }

  template <typename I, typename G, typename N>
  inline
  void
  complex_neighborhood_bkd_piter<I, G, N>::do_next_()
  {
    iter_.next();
  }

  template <typename I, typename G, typename N>
  template <typename Pref>
  inline
  void
  complex_neighborhood_bkd_piter<I, G, N>::center_at_(const Pref&)
  {
    iter_.center_at(this->center().face());
  }

  template <typename I, typename G, typename N>
  inline
  mln_psite(N)
  complex_neighborhood_bkd_piter<I, G, N>::compute_p_() const
  {
    return psite(this->center().site_set(), iter_);
  }

  template <typename I, typename G, typename N>
  inline
  const typename N::complex_bkd_iter&
  complex_neighborhood_bkd_piter<I, G, N>::iter() const
  {
    return iter_;
  }

  template <typename I, typename G, typename N>
  inline
  typename N::complex_bkd_iter&
  complex_neighborhood_bkd_piter<I, G, N>::iter()
  {
    return iter_;
  }


  template <typename I, typename G, typename N>
  inline
  std::ostream&
  operator<<(std::ostream& ostr,
	     const complex_neighborhood_bkd_piter<I, G, N>& p)
  {
    return ostr << p.unproxy_();
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // ! MLN_CORE_IMAGE_COMPLEX_NEIGHBORHOOD_PITER_HH
