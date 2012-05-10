// Copyright (C) 2007, 2008, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_CORE_SITE_SET_P_CENTERED_HH
# define MLN_CORE_SITE_SET_P_CENTERED_HH

/// \file
///
/// This file defines the site set corresponding to a window
/// centered on a site.
///
/// \todo Add the bkd iter.
/// \todo Code is_valid() and change_target() for the site set.

# include <mln/core/concept/window.hh>
# include <mln/core/internal/site_set_base.hh>
# include <mln/core/internal/site_set_iterator_base.hh>


namespace mln
{

  // Fwd decls.
  template <typename W> class p_centered;
  template <typename W> class p_centered_piter;


  namespace trait
  {

    template <typename W>
    struct site_set_< p_centered<W> >
    {
      typedef trait::site_set::nsites::unknown nsites;
      typedef trait::site_set::bbox::unknown   bbox;
      typedef trait::site_set::contents::fixed contents;
      typedef trait::site_set::arity::unique   arity;
    };

//     template <typename W>
//     struct set_precise_unary_< op::ord, p_centered<W> >
//     {
//       typedef set_precise_unary_< op::ord, p_centered<W> > ret; // Itself.
//       bool strict(const p_centered<W>& lhs, const p_centered<W>& rhs) const;
//     };

  } // end of namespace mln::trait


  /// \brief Site set corresponding to a window centered on a site.
  ///
  /// \ingroup modsitesetsparse
  //
  template <typename W>
  class p_centered : public internal::site_set_base_< mln_psite(W), p_centered<W> >,
		     private mlc_is_a(W, Window)::check_t
  {
  public:

    /// Psite associated type.
    typedef mln_psite(W) psite;

    /// Site associated type.
    typedef mln_site(W) site;


    /// Element associated type.
    typedef psite element;


    /// Forward Site_Iterator associated type.
    typedef p_centered_piter<W> fwd_piter;

    /// Backward Site_Iterator associated type.
    typedef p_centered_piter<W> bkd_piter; // FIXME

    /// Site_Iterator associated type.
    typedef fwd_piter piter;


    /// Constructor without argument.
    p_centered();

    /// Constructor from a window \p win and a center \p c.
    p_centered(const W& win, const mln_psite(W)& c);


    /// Test if \p p belongs to the box.
    template <typename P>
    bool has(const P& p) const;

    /// Test if this site set is initialized.
    bool is_valid() const;

    /// Return the size of this site set in memory.
    std::size_t memory_size() const;

    /// Give the center of this site set.
    const mln_psite(W)& center() const;

    /// Give the window this site set is defined upon.
    const W& window() const;

  protected:

    W win_;
    mln_psite(W) c_;
  };


  template <typename W>
  class p_centered_piter : public internal::site_set_iterator_base< p_centered<W>,
								    p_centered_piter<W> >
  {
    typedef p_centered_piter<W> self_;
    typedef internal::site_set_iterator_base< p_centered<W>, self_ > super_;
  public:

    /// Constructor without argument.
    p_centered_piter();

    /// Constructor.
    p_centered_piter(const p_centered<W>& s);

    /// Test the iterator validity.
    bool is_valid_() const;

    /// Invalidate the iterator.
    void invalidate_();

    /// Start an iteration.
    void start_();

    /// Go to the next point.
    void next_();

  protected:
    using super_::p_;
    using super_::s_;

    mln_fwd_qiter(W) q_;
  };



# ifndef MLN_INCLUDE_ONLY

  // p_centered<W>

  template <typename W>
  inline
  bool
  p_centered<W>::is_valid() const
  {
    return true; // FIXME
  }

  template <typename W>
  inline
  p_centered<W>::p_centered()
  {
  }

  template <typename W>
  inline
  p_centered<W>::p_centered(const W& win, const mln_psite(W)& c)
    : win_(win),
      c_(c)
  {
    mln_precondition(is_valid());
  }

  template <typename W>
  template <typename P>
  inline
  bool
  p_centered<W>::has(const P&) const
  {
    mln_precondition(is_valid());
    return true; // FIXME
  }

  template <typename W>
  inline
  std::size_t
  p_centered<W>::memory_size() const
  {
    return sizeof(*this);
  }

  template <typename W>
  inline
  const mln_psite(W)&
  p_centered<W>::center() const
  {
    return c_;
  }

  template <typename W>
  inline
  const W&
  p_centered<W>::window() const
  {
    return win_;
  }

//   namespace trait
//   {

//     template <typename W>
//     inline
//     bool
//     set_precise_unary_< op::ord, p_centered<W> >::strict(const p_centered<W>& lhs, const p_centered<W>& rhs) const
//     {
//       // Lexicographical over "pmin then pmax".
//       return util::ord_lexi_strict(lhs.pmin(), lhs.pmax(),
// 				   rhs.pmin(), rhs.pmax());
//     }

//   } // end of namespace mln::trait


  // p_centered_piter<W>

  template <typename W>
  inline
  p_centered_piter<W>::p_centered_piter()
  {
  }

  template <typename W>
  inline
  p_centered_piter<W>::p_centered_piter(const p_centered<W>& s)
  {
    this->change_target(s);
    q_.center_at(s.center());
    q_.change_target(s.window_());
  }

  template <typename W>
  inline
  bool
  p_centered_piter<W>::is_valid_() const
  {
    return q_.is_valid();
  }

  template <typename W>
  inline
  void
  p_centered_piter<W>::invalidate_()
  {
    q_.invalidate();
  }

  template <typename W>
  inline
  void
  p_centered_piter<W>::start_()
  {
    q_.start();
    if (is_valid_())
      p_ = q_;
  }

  template <typename W>
  inline
  void
  p_centered_piter<W>::next_()
  {
    q_.next();
    if (is_valid_())
      p_ = q_;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_SITE_SET_P_CENTERED_HH
