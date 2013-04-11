// Copyright (C) 2007, 2008, 2009, 2013 EPITA Research and Development
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

#ifndef MLN_CORE_SITE_SET_P_IMAGE_HH
# define MLN_CORE_SITE_SET_P_IMAGE_HH

/// \file
///
/// \brief Definition of a site set class based on an image of
/// Booleans.
///
/// \todo Add an init method (deferred initialization).


# include <mln/core/site_set/p_if.hh>
# include <mln/fun/ops.hh>
# include <mln/pw/value.hh>
# include <mln/pw/cst.hh>
# include <mln/data/fill_with_value.hh>



namespace mln
{

  // Forward declaration.
  template <typename I> class p_image;


  namespace trait
  {

    template <typename I>
    struct site_set_< p_image<I> >
    {
      typedef trait::site_set::nsites::known  nsites;
      typedef trait::site_set::bbox::unknown  bbox; // FIXME
      typedef trait::site_set::contents::free contents;
      typedef trait::site_set::arity::unique  arity;
    };

  } // end of namespace trait


  namespace internal
  {

    template <typename I>
    struct p_image_site_set // Hack to help g++-2.95.
    {
    private:
      typedef mln_domain(I) S_;
      typedef fun::eq_v2b_expr_< pw::value_<I>, pw::cst_<bool> > F_;
    public:
     typedef p_if<S_, F_> ret;
    };


  } // end of namespace internal


  /// \brief Site set based on an image of Booleans.
  ///
  /// \ingroup modsitesetsparse
  template <typename I>
  class p_image : public internal::site_set_base_< mln_psite(I), p_image<I> >
  {
  public:

    /// Equivalent site_set type.
    typedef typename internal::p_image_site_set<I>::ret S;

    /// Conversion towards the equivalent site set.
    operator typename internal::p_image_site_set<I>::ret () const;


    /// Element associated type.
    typedef mln_psite(I) element;


    /// Psite associated type.
    typedef mln_psite(I) psite;

    /// Forward Site_Iterator associated type.
    typedef mln_fwd_piter(S) fwd_piter;

    /// Backward Site_Iterator associated type.
    typedef mln_bkd_piter(S) bkd_piter;

    /// Site_Iterator associated type.
    typedef mln_piter(S) piter;


    /// Constructor without argument.
    p_image();

    /// Constructor.
    p_image(const I& ima);



    /// Test is the psite \p p belongs to this site set.
    bool has(const psite&) const;


    /// Test if this site set is valid, i.e., initialized.
    bool is_valid() const;

    /// Give the number of sites.
    unsigned nsites() const;


    /// Insertion element associated type.
    typedef psite i_element;

    /// Insert a site \p p.
    void insert(const psite& p);

    /// Removal element associated type.
    typedef psite r_element;

    /// Remove a site \p p.
    void remove(const psite& p);


    /// Change the status in/out of a site \p p.
    void toggle(const psite& p);


    /// Return the size of this site set in memory.
    std::size_t memory_size() const;


    /// Clear this set.
    void clear();


    /// \cond INTERNAL_API
    /// Hook to the image
    const I& image_hook_() const;
    /// \endcond

  private:

    I ima_;
    unsigned nsites_;
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename I>
  inline
  p_image<I>::operator typename internal::p_image_site_set<I>::ret () const
  {
    S tmp(ima_.domain(), pw::value(ima_) == pw::cst(true));
    return tmp;
  }

  template <typename I>
  inline
  p_image<I>::p_image()
  {
    nsites_ = 0;
  }

  template <typename I>
  inline
  p_image<I>::p_image(const I& ima)
  {
    mln_precondition(ima.is_valid());
    ima_ = ima;
    clear();
  }

  template <typename I>
  inline
  bool
  p_image<I>::has(const psite& p) const
  {
    mln_precondition(is_valid());
    return ima_.domain().has(p) && ima_(p) == true;
  }

  template <typename I>
  inline
  bool
  p_image<I>::is_valid() const
  {
    return ima_.is_valid();
  }

  template <typename I>
  inline
  unsigned
  p_image<I>::nsites() const
  {
    return nsites_;
  }

  template <typename I>
  inline
  void
  p_image<I>::insert(const psite& p)
  {
    mln_precondition(is_valid());
    mln_precondition(ima_.domain().has(p));
    if (ima_(p) == true)
      return; // No-op.
    ima_(p) = true;
    ++nsites_;
  }

  template <typename I>
  inline
  void
  p_image<I>::remove(const psite& p)
  {
    mln_precondition(is_valid());
    mln_precondition(ima_.domain().has(p));
    if (ima_(p) == false)
      return; // No-op.
    ima_(p) = false;
    mln_assertion(nsites_ > 0);
    --nsites_;
  }

  template <typename I>
  inline
  void
  p_image<I>::toggle(const psite& p)
  {
    mln_precondition(is_valid());
    mln_precondition(ima_.domain().has(p));
    if (ima_(p) == true)
      {
	// Removal.
	ima_(p) = false;
	mln_assertion(nsites_ > 0);
	--nsites_;
      }
    else
      {
	// Insertion.
	ima_(p) = true;
	++nsites_;
      }
  }

  template <typename I>
  inline
  std::size_t
  p_image<I>::memory_size() const
  {
    if (! is_valid())
      return sizeof(*this);
    return 0; // FIXME
  }

  template <typename I>
  inline
  void
  p_image<I>::clear()
  {
    if (! is_valid())
      return; // No-op.
    nsites_ = 0;
    data::fill_with_value(ima_, false);
  }

  template <typename I>
  inline
  const I&
  p_image<I>::image_hook_() const
  {
    mln_precondition(is_valid());
    return ima_;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_SITE_SET_P_IMAGE_HH
