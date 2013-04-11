// Copyright (C) 2008, 2009, 2013 EPITA Research and Development
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

#ifndef MLN_CORE_SITE_SET_P_DOUBLE_HH
# define MLN_CORE_SITE_SET_P_DOUBLE_HH

/*! \file
 *
 * FIXME
 *
 * \todo Document!
 *
 * \todo Add tests about validity of compounded site sets (?)
 */

# include <mln/core/internal/pseudo_site_base.hh>
# include <mln/core/internal/site_set_iterator_base.hh>


namespace mln
{

  // p_double_psite<S,Sp>

  template <typename S, typename Sp>
  class p_double_psite : public internal::pseudo_site_base_< const mln_psite(Sp)&,
							     p_double_psite<S,Sp> >
  {
  public:

    p_double_psite();

    // Target associated type.
    typedef S target;

    /// \cond INTERNAL_API
    const S* target_() const;
    /// \endcond

    void change_target(const S& newtarget);

    bool is_valid() const;

    unsigned index() const;
    const mln_psite(Sp)& p() const;

    void change_i(unsigned i);

    void change_p(const mln_psite(Sp)& p);

    /// \cond INTERNAL_API
    // As a Proxy:
    const mln_psite(Sp)& subj_();
    /// \endcond

  private:

    const target* s_;
    mutable unsigned i_;
    mutable mln_psite(Sp) p_;
  };



  // p_double_piter<S,I1,I2>

  template <typename S, typename I1, typename I2>
  class p_double_piter
    :
    public internal::site_set_iterator_base< S,
					     p_double_piter<S,I1,I2> >
  {
    typedef p_double_piter<S,I1,I2> self_;
    typedef internal::site_set_iterator_base<S,self_> super_;
  public:

    /// Constructor with no argument.
    p_double_piter();

    /// Constructor.
    p_double_piter(const S& s);

    /// Change the site set targeted by this iterator.
    void change_target(const S& newtarget);

    /// Test if the iterator is valid.
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

  private:
    I1 i1_;
    I2 i2_;

    // Progress to the next valid state if it exists.
    void progress_();

  };



# ifndef MLN_INCLUDE_ONLY


  // p_double_psite<S,Sp>

  template <typename S, typename Sp>
  inline
  p_double_psite<S,Sp>::p_double_psite()
  {
  }

  template <typename S, typename Sp>
  inline
  const mln_psite(Sp)&
  p_double_psite<S,Sp>::subj_()
  {
    return p_;
  }

  template <typename S, typename Sp>
  inline
  const S*
  p_double_psite<S,Sp>::target_() const
  {
    return s_;
  }

  template <typename S, typename Sp>
  inline
  void
  p_double_psite<S,Sp>::change_target(const S& s)
  {
    s_ = & s;
  }

  template <typename S, typename Sp>
  inline
  bool
  p_double_psite<S,Sp>::is_valid() const
  {
    return s_ != 0 && p_.is_valid();
  }

  template <typename S, typename Sp>
  inline
  unsigned
  p_double_psite<S,Sp>::index() const
  {
    return i_;
  }

  template <typename S, typename Sp>
  inline
  const mln_psite(Sp)&
  p_double_psite<S,Sp>::p() const
  {
    return p_;
  }

  template <typename S, typename Sp>
  inline
  void
  p_double_psite<S,Sp>::change_i(unsigned i)
  {
    i_ = i;
  }

  template <typename S, typename Sp>
  inline
  void
  p_double_psite<S,Sp>::change_p(const mln_psite(Sp)& p)
  {
    p_ = p;
  }


  // p_double_piter<S,I1,I2>

  template <typename S, typename I1, typename I2>
  inline
  p_double_piter<S,I1,I2>::p_double_piter()
  {
  }

  template <typename S, typename I1, typename I2>
  inline
  p_double_piter<S,I1,I2>::p_double_piter(const S& s)
  {
    this->change_target(s);
  }

  template <typename S, typename I1, typename I2>
  inline
  void
  p_double_piter<S,I1,I2>::change_target(const S& newtarget)
  {
    this->super_::change_target(newtarget);
    i1_.change_target(newtarget.set_1_());
    invalidate_();
  }

  template <typename S, typename I1, typename I2>
  inline
  bool
  p_double_piter<S,I1,I2>::is_valid_() const
  {
    return i2_.is_valid();
  }

  template <typename S, typename I1, typename I2>
  inline
  void
  p_double_piter<S,I1,I2>::invalidate_()
  {
    i2_.invalidate();
  }

  template <typename S, typename I1, typename I2>
  inline
  void
  p_double_piter<S,I1,I2>::start_()
  {
    i1_.start();
    if (i1_.is_valid())
      {
	i2_.change_target(s_->set_2_(i1_));
	i2_.start();
	if (! i2_.is_valid())
	  progress_();
	else
	  {
	    p_.change_i(i1_.index_());
	    p_.change_p(i2_);
	  }
      }
    else
      i2_.invalidate();
    mln_postcondition(implies(i2_.is_valid(), i1_.is_valid()));
  }

  template <typename S, typename I1, typename I2>
  inline
  void
  p_double_piter<S,I1,I2>::next_()
  {
    i2_.next();
    if (! i2_.is_valid())
      progress_();
    else
      p_.change_p(i2_);
    mln_postcondition(implies(i2_.is_valid(), i1_.is_valid()));
  }

  template <typename S, typename I1, typename I2>
  inline
  void
  p_double_piter<S,I1,I2>::progress_()
  {
    // This routine is general; it does not make the assumption that
    // the site set type features is_empty().
    while (! i2_.is_valid() && i1_.is_valid())
      {
	i1_.next();
	if (! i1_.is_valid())
	  {
	    // End of iterations.
	    i2_.invalidate(); // Safety.
	    return;
	  }
	i2_.change_target(s_->set_2_(i1_));
	i2_.start();
      }
    if (i2_.is_valid())
      {
	p_.change_i(i1_.index_());
	p_.change_p(i2_);
      }
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_SITE_SET_P_DOUBLE_HH
