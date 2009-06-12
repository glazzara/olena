// Copyright (C) 2008 EPITA Research and Development Laboratory
//
// This file is part of the Milena Library.  This library is free
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


#ifndef MULTI_SET_HH
# define MULTI_SET_HH

/*! \file sandbox/pellegrin/set/multi_set.hh
 *
 * \brief Definition of a point multi-set class.
 */

# include <mln/core/internal/point_set_base.hh>
# include <mln/core/internal/set_of.hh>


namespace mln
{

  /*! \brief Point set class.
   *
   * This is a mathematical multi-set of points.  The
   * parameter \p P shall be a Point type.
   *
   * \todo All.
   */
  template <typename P>
  class multi_set : public internal::point_set_base_< P, p_set<P> >,
		    private internal::set_of_<P>
  {
    typedef internal::set_of_<P> super_;

  public:
    /// Forward Site_Iterator associated type.
    typedef multi_set_fwd_piter_<P> fwd_piter;
    /// Backward Site_Iterator associated type.
    typedef multi_set_bkd_piter_<P> bkd_piter;

    /// Constructor.
    multi_set();

    /// Test is \p p belongs to this point set.
    bool has(const P& p) const;

    /// Return the corresponding std::vector of points.
    using super_::vect;

    /// Give the number of points.
    std::size_t nsites() const;

    /// Insert a point \p p.
    multi_set<P>& insert(const P& p);

    // FIXME : doesn't compile
    //     /// Remove a point \p p.
    //     p_set<P>& remove(P& p);

    /// Return the \p i-th point.
    const P& operator[](unsigned i) const;

    /// Clear this set.
    void clear();

    /// Give the exact bounding box.
    const box_<mln_point(P)>& bbox() const;

  protected:

    accu::shape::bbox<P> bb_;
    // FIXME: Add invariant  bb_.is_valid() <=> nsites() != 0
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename P>
  inline
  multi_set<P>::multi_set()
  {
  }

  template <typename P>
  inline
  bool
  multi_set<P>::has(const P& p) const
  {
    return this->super_::has(p);
  }

  template <typename P>
  inline
  std::size_t
  multi_set<P>::nsites() const
  {
    return this->super_::nelements();
  }

  template <typename P>
  inline
  multi_set<P>&
  multi_set<P>::insert(const P& p)
  {
    this->super_::insert(p);
    bb_.take(p);
    return *this;
  }


  // FIXME : finish it.
  //   template <typename P>
  //   p_set<P>&
  //   p_set<P>::remove(P& p)
  //   {
  //     this->super_::remove(p);
  //     // FIXME: need to rebuild bb_ ?
  //     //bb_.untake(p);
  //     return *this;
  //   }

  template <typename P>
  inline
  const P&
  multi_set<P>::operator[](unsigned i) const
  {
    mln_precondition(i < nsites());
    return this->super_::element(i);
  }

  template <typename P>
  inline
  void
  multi_set<P>::clear()
  {
    this->super_::clear();
    bb_.init();
  }

  template <typename P>
  inline
  const box_<mln_point(P)>&
  multi_set<P>::bbox() const
  {
    mln_precondition(nsites() != 0);
    return bb_.to_result();
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MULTI_SET_HH
