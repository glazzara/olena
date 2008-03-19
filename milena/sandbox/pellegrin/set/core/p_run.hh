// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_P_RUN_HH
# define MLN_CORE_P_RUN_HH

/*! \file mln/core/p_run.hh
 *
 * \brief Definition of a point set class based on std::set.
 */

# include <mln/core/internal/point_set_base.hh>
# include <mln/core/internal/set_of.hh>
# include <mln/accu/bbox.hh>
# include <mln/trait/point_set.hh>


namespace mln
{

  // Fwd decls.
  template <typename P> struct p_run;
  template <typename P> struct p_run_fwd_piter_;
  template <typename P> struct p_run_bkd_piter_;

  namespace trait
  {

    template <typename P>
    struct point_set_< p_run<P> > : public default_point_set_< p_run<P> >
    {
      typedef trait::point_set::arity::unique   arity;
      typedef trait::point_set::has_speed::fast has_speed;
    }

  }

  /*! \brief Point set class in run.
   *
   * This is a mathematical set of points (not a multi-set).  The
   * parameter \p P shall be a Point type.
   *
   * \todo Test if \p P being a Point_Site is ok.
   */
  template <typename P>
  class p_run : public internal::point_set_base_< P, p_run<P> >
  {
  public:

    /// Forward Point_Iterator associated type.
    typedef p_run_fwd_piter_<P> fwd_piter;

    /// Backward Point_Iterator associated type.
    typedef p_run_bkd_piter_<P> bkd_piter;

    /// Constructor without argument.
    p_run();

    /// Constructor.
    p_run(const P& start, std::size_t len);

    /// Set the starting point.
    void set_run(const P& start, std::size_t len);

    /// Test is \p p belongs to this point set.
    bool has(const P& p) const;

    /// Give the number of points.
    std::size_t npoints() const;

    /// Give the length of the run.
    std::size_t length() const;

    /// Return the \p i-th point.
    P operator[](unsigned i) const;

    /// Return the first point.
    const P& first() const;

    /// Give the exact bounding box.
    const box_<mln_point(P)>& bbox() const;

    /// Set a relation order to p_run.
    bool operator<(const p_run<P>& rhs) const;

  protected:

    accu::bbox<P> bb_;
    // FIXME: Add invariant  bb_.is_valid() <=> npoints() != 0

    /// The first point of the run.
    P p_;

    /// The length of the run.
    std::size_t len_;

    /// For internal use.
    bool is_valid_;
  };

  template <typename P>
  std::ostream& operator<<(std::ostream& out, const p_run<P>& pr)
  {
    out << "Run: (" << pr.first() << ", " << pr.length() << ")";
    return out;
  }

# ifndef MLN_INCLUDE_ONLY

  template <typename P>
  inline
  p_run<P>::p_run()
  {
    is_valid_ = false;
  }

  template <typename P>
  inline
  p_run<P>::p_run(const P& start, std::size_t len)
    : p_(start),
      len_(len)
  {
    mln_precondition(len != 0);
    P p = start;
    bb_.init();
    bb_.take(p);
    p[P::dim - 1] += len - 1;
    bb_.take(p);
    is_valid_ = true;
  }

  template <typename P>
  inline
  void
  p_run<P>::set_run(const P& start, std::size_t len)
  {
    mln_precondition(len != 0);
    p_ = start;
    len_ = len;
    P p = start;
    bb_.init();
    bb_.take(p);
    p[P::dim - 1] += len - 1;
    bb_.take(p);
    is_valid_ = true;
  }

  template <typename P>
  inline
  bool
  p_run<P>::has(const P& p) const
  {
    mln_precondition(is_valid_);
    bool res = true;
    for (int i = P::dim - 2; i >= 0; --i)
      if (!(res = (res && p[i] == p_[i])))
	return false;
    return (p[P::dim - 1] >= p_[P::dim - 1]
	    && p[P::dim - 1] < p_[P::dim - 1] + (signed)len_);
  }

  template <typename P>
  inline
  std::size_t
  p_run<P>::npoints() const
  {
    mln_precondition(is_valid_);
    return len_;
  }

  template <typename P>
  inline
  std::size_t
  p_run<P>::length() const
  {
    mln_precondition(is_valid_);
    return len_;
  }

  template <typename P>
  inline
  P
  p_run<P>::operator[](unsigned i) const
  {
    mln_precondition(is_valid_);
    mln_precondition(i < npoints());
    P p = p_;
    p[P::dim - 1] += i;
    return p;
  }

  template <typename P>
  inline
  const P&
  p_run<P>::first() const
  {
    return p_;
  }

  template <typename P>
  inline
  const box_<mln_point(P)>&
  p_run<P>::bbox() const
  {
    mln_precondition(is_valid_);
    mln_precondition(npoints() != 0);
    return bb_.to_result();
  }

  template <typename P>
  inline
  bool
  p_run<P>::operator<(const p_run<P>& rhs) const
  {
    return (this->p_ < rhs.p_)
      || (this->p_ == rhs.p_ && this->len_ < rhs.len_);
  }


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

# include <mln/core/p_run_piter.hh>

#endif // ! MLN_CORE_P_RUN_HH
