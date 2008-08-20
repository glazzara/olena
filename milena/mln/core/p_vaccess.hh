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
 * \todo Factor the implementation classes for site set based on a
 * couple of structures.  Such piece of code shall be used in p_set_of
 * and p_array_of.
 *
 * \todo Fix the FIXMEs.
 */

# include <utility>
# include <mln/core/p_double.hh>
# include <mln/core/internal/site_set_base.hh>
# include <mln/core/internal/pseudo_site_base.hh>
# include <mln/accu/bbox.hh>



namespace mln
{

  // Forward declaration.
  template <typename Sv, typename Sp> class p_vaccess;



  namespace trait
  {

    template <typename Sv, typename Sp>
    struct site_set_< p_vaccess<Sv,Sp> >
    {
      typedef mln_trait_site_set_nsites(Sp)      nsites;
      typedef mln_trait_site_set_bbox(Sp)        bbox;
      typedef trait::site_set::contents::growing contents;
      typedef trait::site_set::arity::multiple   arity;
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
    typedef p_vaccess<Sv,Sp> self_;
  public:

    /// Value associated type.
    typedef mln_value(Sv) value;


    /// Psite associated type.
    typedef p_double_psite<self_, Sp> psite;

    /// Forward Site_Iterator associated type.
    typedef p_double_piter<self_, mln_fwd_viter(Sv), mln_fwd_piter(Sp)> fwd_piter;

    /// Backward Site_Iterator associated type.
    typedef p_double_piter<self_, mln_bkd_viter(Sv), mln_bkd_piter(Sp)> bkd_piter;

    /// Site_Iterator associated type.
    typedef fwd_piter piter;


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

    /// Insert a pair \p v_e (value v, element e).
    void insert(const i_element& v_e);

    /// Insert \p e at value \p v.
    void insert(const value& v, const element& e);


    /// Return the site set at value \p v.
    const Sp& operator()(const value& v) const;

    /// Return the size of this site set in memory.
    std::size_t memory_size() const;


    /// Give the set of values.
    const Sv& values() const;

    // Required by p_double-related classes.
    const Sv& set_1_() const;
    const Sp& set_2_(const value& v) const;
    
  protected:

    Sv vs_;
    std::vector<Sp> ps_;
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
  p_vaccess<Sv,Sp>::has(const psite&) const
  {
    // FIXME
    return true;
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

  template <typename Sv, typename Sp>
  inline
  const Sv&
  p_vaccess<Sv,Sp>::values() const
  {
    return vs_;
  }

  template <typename Sv, typename Sp>
  inline
  const Sv&
  p_vaccess<Sv,Sp>::set_1_() const
  {
    return vs_;
  }

  template <typename Sv, typename Sp>
  inline
  const Sp&
  p_vaccess<Sv,Sp>::set_2_(const value& v) const
  {
    return ps_[vs_.index_of(v)];
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
  

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_P_VACCESS_HH
