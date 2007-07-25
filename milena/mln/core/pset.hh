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

#ifndef MLN_CORE_PSET_HH
# define MLN_CORE_PSET_HH

/*! \file mln/core/pset.hh
 *
 * \brief Definition of a point set class based on std::set.
 */

# include <mln/core/concept/point_set.hh>
# include <mln/core/internal/set_of.hh>
# include <mln/accu/bbox.hh>
# include <mln/core/pvec.hh>


namespace mln
{

  /*! \brief Point set class based on std::set.
   *
   * This is a mathematical set of points (not a multi-set).  The
   * parameter \p P shall be a Point type.
   *
   * \todo Test if \p P being a Point_Site is ok.
   */
  template <typename P>
  class pset : public Point_Set< pset<P> >,
	       private internal::set_of_<P>
  {
    typedef internal::set_of_<P> super_;

  public:

    /// Point associated type.
    typedef mln_point(P) point;

    /// Point_Site associated type.
    typedef P psite;

    /// Forward Point_Iterator associated type.
    typedef pvec_fwd_piter_<P> fwd_piter;

    /// Backward Point_Iterator associated type.
    typedef internal::fixme    bkd_piter;

    /// Constructor.
    pset();

    /// Test is \p p belongs to this point set.
    bool has(const P& p) const;

    /// Return the corresponding std::vector of points.
    using super_::vect;

    /// Give the number of points.
    std::size_t npoints() const;

    /// Insert a point \p p.
    pset<P>& insert(const P& p);

    /// Return the \p i-th point.
    const P& operator[](unsigned i) const;

    /// Clear this set.
    void clear();

    /// Give the exact bounding box.
    const box_<point>& bbox() const;

  protected:

    accu::bbox<P> bb_;
    // FIXME: Add invariant  bb_.is_valid() <=> npoints() != 0
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename P>
  pset<P>::pset()
  {
  }

  template <typename P>
  bool
  pset<P>::has(const P& p) const
  {
    return this->super_::has(p);
  }

  template <typename P>
  std::size_t
  pset<P>::npoints() const
  {
    return this->super_::nelements();
  }

  template <typename P>
  pset<P>&
  pset<P>::insert(const P& p)
  {
    this->super_::insert(p);
    bb_.take(p);
    return *this;
  }

  template <typename P>
  const P&
  pset<P>::operator[](unsigned i) const
  {
    mln_precondition(i < npoints());
    return this->super_::element(i);
  }

  template <typename P>
  void
  pset<P>::clear()
  {
    this->super_::clear();
    bb_.init();
  }

  template <typename P>
  const box_<mln_point(P)>&
  pset<P>::bbox() const
  {
    mln_precondition(npoints() != 0);
    return bb_.to_value();
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_PSET_HH
