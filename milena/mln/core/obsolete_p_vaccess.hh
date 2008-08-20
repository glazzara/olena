// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_P_VACCESS_HH
# define MLN_CORE_P_VACCESS_HH

/*! \file mln/core/p_vaccess.hh
 *
 * \todo Factor double-iterators into a class.
 */

# include <utility>
# include <mln/core/internal/site_set_base.hh>
# include <mln/core/internal/pseudo_site_base.hh>
# include <mln/accu/bbox.hh>



namespace mln
{

  // Forward declarations.
  template <typename Sv, typename Sp> class p_vaccess;
  template <typename Sv, typename Sp> class p_vaccess_psite;
  template <typename Sv, typename Sp> class p_vaccess_fwd_piter;
  template <typename Sv, typename Sp> class p_vaccess_bkd_piter;


  namespace trait
  {

    template <typename Sv, typename Sp>
    struct site_set_< p_vaccess<Sv,Sp> >
    {
      typedef mln_trait_site_set_nsites(Sp)    nsites;
      typedef mln_trait_site_set_bbox(Sp)      bbox;
      typedef trait::site_set::contents::fixed contents;
      typedef trait::site_set::arity::multiple arity;
    };

  } // end of namespace trait



  namespace internal
  {

    // For .nsites()

    template <typename trait_nsites, typename Sp>
    struct p_vaccess_impl__nsites
    {
    protected:
      template <typename T>
      void update_nsites_(const T&); // No-op.
    };

    template <typename Sp>
    struct p_vaccess_impl__nsites< trait::site_set::nsites::known, Sp>
    {
      p_vaccess_impl__nsites();
      unsigned nsites() const;
    protected:
      void update_nsites_(const mln_site(Sp)& p);
      template <typename S>
      void update_nsites_(const Site_Set<S>& s);
      unsigned nsites_;
    };

    // For .bbox()

    template <typename trait_bbox, typename Sp>
    struct p_vaccess_impl__bbox
    {
      typedef const mln::box<mln_site(Sp)>& q_box;
      q_box bbox() const;

    protected:
      void update_bbox_(const mln_site(Sp)& p);
      template <typename S>
      void update_bbox_(const Site_Set<S>& s);

      accu::bbox<mln_site(Sp)> bb_;
    };

    template <typename Sp>
    struct p_vaccess_impl__bbox< trait::site_set::nsites::unknown, Sp >
    {
    protected:
      template <typename T>
      void update_bbox_(const T&); // No-op.
    };

    // Facade.

    template <typename Sp>
    struct p_vaccess_impl
      : p_vaccess_impl__nsites< mln_trait_site_set_nsites(Sp), Sp>,
	p_vaccess_impl__bbox  < mln_trait_site_set_bbox(Sp),   Sp>
    {
    };

  } // end of namespace mln::internal



  /*! \brief FIXME
   */
  template <typename Sv, typename Sp>
  class p_vaccess : public internal::site_set_base_< mln_site(Sp),
						     p_vaccess<Sv,Sp> >,
		    public internal::p_vaccess_impl< Sp >
  {
  public:

    /// Value associated type.
    typedef mln_value(Sv) value;

    /// Psite associated type.
    typedef p_vaccess_psite<Sv,Sp> psite;

    /// Site_Iterator associated type.
    typedef p_vaccess_fwd_piter<Sv,Sp>     piter;

    /// Forward Site_Iterator associated type.
    typedef p_vaccess_fwd_piter<Sv,Sp> fwd_piter;

    /// Backward Site_Iterator associated type.
    typedef p_vaccess_bkd_piter<Sv,Sp> bkd_piter;


    /// Constructor.
    p_vaccess();
    p_vaccess(const Sv& vset);


    /// Test if \p p belongs to this site set.
    bool has(const psite& p) const;

    /// Test if the couple (value \p v, psite \p p) belongs to this
    /// site set.
    bool has(const value& v, const mln_psite(Sp)& p) const;

    /// Test if this site set is valid. 
    bool is_valid() const;


    /// Element associated type.
    typedef mln_element(Sp) element;

    /// Insertion element associated type.
    typedef std::pair<value, element> i_element;

    /// Insert a pair (value \p v, element \p e).
    void insert(const i_element& v_e);

    /// Insert \p e at value \p v.
    void insert(const value& v, const element& e);


    /// Return the site set at value \p v.
    const Sp& operator()(const value& v) const;

    /// Return the size of this site set in memory.
    std::size_t memory_size() const;

    const Sv& values() const
    {
      return vs_;
    }

    const Sv& set_1_() const
    {
      return vs_;
    }

    const Sp& set_2_(const value& v) const
    {
      return ps_[vs_.index_of(v)];
    }

    
  protected:

    Sv vs_;
    std::vector<Sp> ps_;
  };



  // p_vaccess_psite<Sv,Sp>

  template <typename Sv, typename Sp>
  class p_vaccess_psite : public internal::pseudo_site_base_< const mln_psite(Sp)&,
							      p_vaccess_psite<Sv,Sp> >
  {
  public:

    typedef mln_value(Sv) value;

    p_vaccess_psite();

    // Target associated type.
    typedef p_vaccess<Sv,Sp> target;

    const p_vaccess<Sv,Sp>* target_() const;

    void change_target(const target& newtarget);

    bool is_valid() const;

    void change_v(const value& v);

    void change_p(const mln_psite(Sp)& p);

    void change_vp(const value& v, const mln_psite(Sp)& p);

    // As a Proxy:
    const mln_psite(Sp)& subj_();

    const mln_value(Sv)& value_() const;
    const mln_psite(Sp)& psite_() const;

  private:

    const target* s_;
    value v_;
    mutable mln_psite(Sp) p_;
  };




  /// Forward iterator.

  template <typename Sv, typename Sp>
  class p_vaccess_fwd_piter
    :
    public internal::site_set_iterator_base< p_vaccess<Sv,Sp>,
					     p_vaccess_fwd_piter<Sv,Sp> >
  {
    typedef p_vaccess_fwd_piter<Sv,Sp> self;
    typedef internal::site_set_iterator_base<p_vaccess<Sv,Sp>, self> super;
  public:

    /// Constructor with no argument.
    p_vaccess_fwd_piter();

    /// Constructor.
    p_vaccess_fwd_piter(const p_vaccess<Sv,Sp>& s);

    /// Test if the iterator is valid.
    bool is_valid_() const;

    /// Invalidate the iterator.
    void invalidate_();

    /// Start an iteration.
    void start_();

    /// Go to the next point.
    void next_();

    mln_value(Sv) value() const;

  protected:
    using super::p_;
    using super::s_;

  private:
    mln_fwd_viter(Sv) vi_;
    mln_fwd_piter(Sp) pi_;

    void progress_();
  };





  /// Backward iterator.

  template <typename Sv, typename Sp>
  class p_vaccess_bkd_piter
    :
    public internal::site_set_iterator_base< p_vaccess<Sv,Sp>,
					     p_vaccess_bkd_piter<Sv,Sp> >
  {
    typedef p_vaccess_bkd_piter<Sv,Sp> self;
    typedef internal::site_set_iterator_base<p_vaccess<Sv,Sp>, self> super;
  public:

    /// Constructor with no argument.
    p_vaccess_bkd_piter();

    /// Constructor.
    p_vaccess_bkd_piter(const p_vaccess<Sv,Sp>& s);

    /// Test if the iterator is valid.
    bool is_valid_() const;

    /// Invalidate the iterator.
    void invalidate_();

    /// Start an iteration.
    void start_();

    /// Go to the next point.
    void next_();

    mln_value(Sv) value() const;

  protected:
    using super::p_;
    using super::s_;

  private:
    mln_bkd_viter(Sv) vi_;
    mln_bkd_piter(Sp) pi_;

    void progress_();
  };





# ifndef MLN_INCLUDE_ONLY


  // p_vaccess<Sv,Sp>

  template <typename Sv, typename Sp>
  inline
  p_vaccess<Sv,Sp>::p_vaccess()
    : vs_(),
      ps_(vs_.nvalues())
  {
  }

  template <typename Sv, typename Sp>
  inline
  p_vaccess<Sv,Sp>::p_vaccess(const Sv& vset)
    : vs_(vset),
      ps_(vs_.nvalues())
  {
  }

  template <typename Sv, typename Sp>
  inline
  bool
  p_vaccess<Sv,Sp>::has(const psite& p) const
  {
    return has(p.value_(), p.psite_());
  }

  template <typename Sv, typename Sp>
  inline
  bool
  p_vaccess<Sv,Sp>::has(const value& v, const mln_psite(Sp)& p) const
  {
    return ps_[vs_.index_of(v)].has(p);
  }

  template <typename Sv, typename Sp>
  inline
  bool
  p_vaccess<Sv,Sp>::is_valid() const
  {
    // FIXME
    return true;
  }

  template <typename Sv, typename Sp>
  inline
  void
  p_vaccess<Sv,Sp>::insert(const value& v, const element& e)
  {
    ps_[vs_.index_of(v)].insert(e);
    this->update_nsites_(e);
    this->update_bbox_(e);
  }

  template <typename Sv, typename Sp>
  inline
  void
  p_vaccess<Sv,Sp>::insert(const i_element& v_e)
  {
    insert(v_e.first, v_e.second);
  }

  template <typename Sv, typename Sp>
  inline
  const Sp&
  p_vaccess<Sv,Sp>::operator()(const value& v) const
  {
    return ps_[vs_.index_of(v)];
  }

  template <typename Sv, typename Sp>
  inline
  std::size_t
  p_vaccess<Sv,Sp>::memory_size() const
  {
    std::size_t mem = 0;
    for (unsigned i = 0; i < ps_.size(); ++i)
      mem += ps_[i].memory_size();
    return sizeof(*this) + mem;
  }

  
  namespace internal
  {

    // p_vaccess_impl__nsites

    template <typename trait_nsites, typename Sp>
    template <typename T>
    inline
    void
    p_vaccess_impl__nsites<trait_nsites, Sp>::update_nsites_(const T&)
    {
      // No-op.
    }

    template <typename Sp>
    inline
    p_vaccess_impl__nsites<trait::site_set::nsites::known, Sp>::p_vaccess_impl__nsites()
      : nsites_(0)
    {
    }

    template <typename Sp>
    inline
    unsigned
    p_vaccess_impl__nsites<trait::site_set::nsites::known, Sp>::nsites() const
    {
      return nsites_;
    }

    template <typename Sp>
    inline
    void
    p_vaccess_impl__nsites<trait::site_set::nsites::known, Sp>::update_nsites_(const mln_site(Sp)&)
    {
      ++nsites_;
    }

    template <typename Sp>
    template <typename S>
    inline
    void
    p_vaccess_impl__nsites<trait::site_set::nsites::known, Sp>::update_nsites_(const Site_Set<S>& s)
    {
      nsites_ += exact(s).nsites();
    }

    // p_vaccess_impl__bbox

    template <typename trait_bbox, typename Sp>
    inline
    typename p_vaccess_impl__bbox<trait_bbox, Sp>::q_box
    p_vaccess_impl__bbox<trait_bbox, Sp>::bbox() const
    {
      return bb_.to_result();
    }

    template <typename trait_bbox, typename Sp>
    inline
    void
    p_vaccess_impl__bbox<trait_bbox, Sp>::update_bbox_(const mln_site(Sp)& p)
    {
      bb_.take(p);
    }

    template <typename trait_bbox, typename Sp>
    template <typename S>
    inline
    void
    p_vaccess_impl__bbox<trait_bbox, Sp>::update_bbox_(const Site_Set<S>& s)
    {
      bb_.take(exact(s).bbox());
    }

    template <typename Sp>
    template <typename T>
    inline
    void
    p_vaccess_impl__bbox< trait::site_set::nsites::unknown, Sp >::update_bbox_(const T&)
    {
      // No-op.
    }

  } // end of namespace mln::internal



  // p_vaccess_psite<Sv,Sp>

  template <typename Sv, typename Sp>
  inline
  p_vaccess_psite<Sv,Sp>::p_vaccess_psite()
  {
  }

  template <typename Sv, typename Sp>
  inline
  const mln_psite(Sp)&
  p_vaccess_psite<Sv,Sp>::subj_()
  {
    return p_;
  }
  
  template <typename Sv, typename Sp>
  inline
  const p_vaccess<Sv,Sp>*
  p_vaccess_psite<Sv,Sp>::target_() const
  {
    return s_;
  }

  template <typename Sv, typename Sp>
  inline
  void
  p_vaccess_psite<Sv,Sp>::change_target(const target& newtarget)
  {
    s_ = & newtarget;
  }

  template <typename Sv, typename Sp>
  inline
  bool
  p_vaccess_psite<Sv,Sp>::is_valid() const
  {
    return s_ != 0 && s_->has(v_, p_);
  }

  template <typename Sv, typename Sp>
  inline
  void
  p_vaccess_psite<Sv,Sp>::change_v(const value& v)
  {
    v_ = v;
  }
  
  template <typename Sv, typename Sp>
  inline
  void
  p_vaccess_psite<Sv,Sp>::change_p(const mln_psite(Sp)& p)
  {
    p_ = p;
  }

  template <typename Sv, typename Sp>
  inline
  void
  p_vaccess_psite<Sv,Sp>::change_vp(const value& v, const mln_psite(Sp)& p)
  {
    change_v(v);
    change_p(p);
  }

  template <typename Sv, typename Sp>
  inline
  const mln_value(Sv)& 
  p_vaccess_psite<Sv,Sp>::value_() const
  {
    return v_;
  }

  template <typename Sv, typename Sp>
  inline
  const mln_psite(Sp)&
  p_vaccess_psite<Sv,Sp>::psite_() const
  {
    return p_;
  }


  // p_vaccess_fwd_piter<Sv,Sp>

  template <typename Sv, typename Sp>
  inline
  p_vaccess_fwd_piter<Sv,Sp>::p_vaccess_fwd_piter()
  {
  }

  template <typename Sv, typename Sp>
  inline
  p_vaccess_fwd_piter<Sv,Sp>::p_vaccess_fwd_piter(const p_vaccess<Sv,Sp>& s)
  {
    this->change_target(s);
    vi_.change_target(s.values());
    invalidate_();
  }

  template <typename Sv, typename Sp>
  inline
  bool
  p_vaccess_fwd_piter<Sv,Sp>::is_valid_() const
  {
    return pi_.is_valid();
  }
  
  template <typename Sv, typename Sp>
  inline
  void
  p_vaccess_fwd_piter<Sv,Sp>::invalidate_()
  {
    pi_.invalidate();
  }
  
  template <typename Sv, typename Sp>
  inline
  void
  p_vaccess_fwd_piter<Sv,Sp>::start_()
  {
    vi_.start();
    if (vi_.is_valid())
      p_.change_v(vi_);
    pi_.change_target((*s_)(vi_));
    pi_.start();
    if (! pi_.is_valid())
      progress_();
    else
      p_.change_p(pi_);
    mln_postcondition(implies(pi_.is_valid(), vi_.is_valid()));
  }

  template <typename Sv, typename Sp>
  inline
  void
  p_vaccess_fwd_piter<Sv,Sp>::next_()
  {
    pi_.next();
    if (! pi_.is_valid())
      progress_();
    else
      p_.change_p(pi_);
    mln_postcondition(implies(pi_.is_valid(), vi_.is_valid()));
  }

  template <typename Sv, typename Sp>
  inline
  mln_value(Sv)
    p_vaccess_fwd_piter<Sv,Sp>::value() const
  {
    mln_precondition(vi_.is_valid());
    return vi_;
  } 

  template <typename Sv, typename Sp>
  inline
  void
  p_vaccess_fwd_piter<Sv,Sp>::progress_()
  {
    // This routine is general; it does not make the assumption that
    // the site set type features is_empty().
    while (! pi_.is_valid() && vi_.is_valid())
      {
	vi_.next();
	if (! vi_.is_valid())
	  {
	    // End of iterations.
	    pi_.invalidate(); // Safety.
	    return;
	  }
	p_.change_v(vi_);
	pi_.change_target((*s_)(vi_));
	pi_.start();
      }
    if (pi_.is_valid())
      p_.change_p(pi_);
  }


  // p_vaccess_bkd_piter<Sv,Sp>

  template <typename Sv, typename Sp>
  inline
  p_vaccess_bkd_piter<Sv,Sp>::p_vaccess_bkd_piter()
  {
  }

  template <typename Sv, typename Sp>
  inline
  p_vaccess_bkd_piter<Sv,Sp>::p_vaccess_bkd_piter(const p_vaccess<Sv,Sp>& s)
  {
    this->change_target(s);
    vi_.change_target(s.values());
    invalidate_();
  }

  template <typename Sv, typename Sp>
  inline
  bool
  p_vaccess_bkd_piter<Sv,Sp>::is_valid_() const
  {
    return pi_.is_valid();
  }

  template <typename Sv, typename Sp>
  inline
  void
  p_vaccess_bkd_piter<Sv,Sp>::invalidate_()
  {
    pi_.invalidate();
  }

  template <typename Sv, typename Sp>
  inline
  void
  p_vaccess_bkd_piter<Sv,Sp>::start_()
  {
    vi_.start();
    if (vi_.is_valid())
      p_.change_v(vi_);
    pi_.change_target((*s_)(vi_));
    pi_.start();
    if (! pi_.is_valid())
      progress_();
    else
      p_.change_p(pi_);
    mln_postcondition(implies(pi_.is_valid(), vi_.is_valid()));
  }

  template <typename Sv, typename Sp>
  inline
  void
  p_vaccess_bkd_piter<Sv,Sp>::next_()
  {
    pi_.next();
    if (! pi_.is_valid())
      progress_();
    else
      p_.change_p(pi_);
    mln_postcondition(implies(pi_.is_valid(), vi_.is_valid()));
  }

  template <typename Sv, typename Sp>
  inline
  mln_value(Sv)
  p_vaccess_bkd_piter<Sv,Sp>::value() const
  {
    mln_precondition(vi_.is_valid());
    return vi_;
  } 

  template <typename Sv, typename Sp>
  inline
  void
  p_vaccess_bkd_piter<Sv,Sp>::progress_()
  {
    // This routine is general; it does not make the assumption that
    // the site set type features is_empty().
    while (! pi_.is_valid() && vi_.is_valid())
      {
	vi_.next();
	if (! vi_.is_valid())
	  {
	    // End of iterations.
	    pi_.invalidate(); // Safety.
	    return;
	  }
	p_.change_v(vi_);
	pi_.change_target((*s_)(vi_));
	pi_.start();
      }
    if (pi_.is_valid())
      p_.change_p(pi_);
  }
  

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_P_VACCESS_HH
