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

#ifndef MLN_CORE_RUNS_PSITE_HH
# define MLN_CORE_RUNS_PSITE_HH

/*! \file mln/core/runs_psite.hh
 *
 * \brief Definition of class mln::runs_psite_ for internal use only
 */

# include <mln/core/concept/point_site.hh>


namespace mln
{
  // Fwd decl.
  template <typename P> class p_runs_;

  /*! \brief Psite class used in run_image_.
   *
   * Parameter \c P is the type of the image point.
   */
  template <typename P>
  class runs_psite : public Point_Site< runs_psite<P> >
  {
  public:

    typedef mln_mesh(P) mesh;
    enum { dim = P::dim };
    typedef P point;
    typedef mln_dpoint(P) dpoint;
    typedef mln_coord(P) coord;

    runs_psite();
    runs_psite(const p_runs_<P>& pr, const P& p);
    runs_psite(P point, unsigned index, unsigned pset_pos);

    operator P () const;

    /// Return the point at the start of the current run.
    P& range_start_();

    /// Return the point at the start of the current run.
    const P& range_start_() const;

    /// Return the position of this psite in the point set.
    unsigned pset_pos_() const;

    /// Return the position of this psite in the point set.
    unsigned& pset_pos_();

    /// Return the position of this psite in the current range.
    unsigned index_() const;

    /// Return the position of this psite in the current range.
    unsigned& index_();

    /// Reference to the corresponding point.
    const P& to_point() const;

    /// Give the i-th coordinate of the corresponding point.
    mln_coord(P) operator[](unsigned i) const;

  protected:

    /// Start of the psite range.
    P p_; // FIXME: Rename as p_ (like everywhere else!)

    /// Position in the psite range.
    unsigned range_index_;

    /// Position of the psite in the point set.
    unsigned pset_position_;
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename P>
  inline
  runs_psite<P>::runs_psite(const p_runs_<P>& pr, const P& p)
  {
    unsigned i = 0;
    while (i < pr.nruns() && p < pr[i].first())
      ++i;
    mln_assertion(i != 0);

    range_index_ = p[P::dim - 1] - pr[i].first()[P::dim - 1];
      
    mln_assertion(pr[i - 1].npoints() > range_index_);

    pset_position_ = i - 1;
    p_ = pr[pset_position_].first();
  }

  template <typename P>
  inline
  runs_psite<P>::runs_psite(P point, unsigned index, unsigned pset_pos) :
    p_(point),
    range_index_(index),
    pset_position_(pset_pos)
  {
  }

  template <typename P>
  inline
  runs_psite<P>::operator P() const
  {
    P p = p_;
    p[dim - 1] += range_index_;
    return p;
  }

  template <typename P>
  inline
  const P&
  runs_psite<P>::range_start_() const
  {
    return p_;
  }

  template <typename P>
  inline
  P&
  runs_psite<P>::range_start_()
  {
    return p_;
  }

  template <typename P>
  inline
  unsigned
  runs_psite<P>::pset_pos_() const
  {
    return pset_position_;
  }

  template <typename P>
  inline
  unsigned&
  runs_psite<P>::pset_pos_()
  {
    return pset_position_;
  }

  template <typename P>
  inline
  unsigned
  runs_psite<P>::index_() const
  {
    return range_index_;
  }

  template <typename P>
  inline
  unsigned&
  runs_psite<P>::index_()
  {
    return range_index_;
  }

  template <typename P>
  inline
  const P&
  runs_psite<P>::to_point() const
  {
    static P p = p_;
    p[dim - 1] += range_index_;
    return p;
  }

  template <typename P>
  inline
  mln_coord(P)
  runs_psite<P>::operator[](unsigned i) const
  {
    mln_precondition(i < dim);
    if (i == dim - 1)
      return p_[i] + range_index_;
    else
      return p_[i];
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_RUNS_PSITE_HH
