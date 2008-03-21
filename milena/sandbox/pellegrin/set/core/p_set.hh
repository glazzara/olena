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

#ifndef MLN_CORE_P_SET_HH
# define MLN_CORE_P_SET_HH

/*! \file mln/core/p_set.hh
 *
 * \brief Definition of a point set class based on std::set.
 */

# include <mln/core/internal/point_set_base.hh>
# include <mln/core/internal/set_of.hh>
# include <mln/accu/bbox.hh>
# include <mln/core/p_array.hh>
# include <trait/point_set.hh>


namespace mln
{

  // Fwd decls.
  template <typename P> struct p_set;

  namespace trait
  {

    template <typename P>
    struct point_set_< p_set<P> > : public default_point_set_< p_set<P> >
    {
      typedef trait::point_set::arity::unique   arity;
      typedef trait::point_set::has_speed::fast has_speed;
    };

  }

  /*! \brief Point set class based on std::set.
   *
   * This is a mathematical set of points (not a multi-set).  The
   * parameter \p P shall be a Point type.
   *
   * \todo Test if \p P being a Point_Site is ok.
   */
  template <typename P>
  class p_set : public internal::point_set_base_< P, p_set<P> >,
		private internal::set_of_<P>
  {
    typedef internal::set_of_<P> super_;

  public:

    /// Forward Point_Iterator associated type.
    typedef p_array_fwd_piter_<P> fwd_piter;

    /// Backward Point_Iterator associated type.
    typedef p_array_bkd_piter_<P> bkd_piter;

    /// Constructor.
    p_set();

    /// Test is \p p belongs to this point set.
    bool has(const P& p) const;

    /// Return the corresponding std::vector of points.
    using super_::vect;

    /// Give the number of points.
    std::size_t npoints() const;

    /// Insert a point \p p.
    p_set<P>& insert(const P& p);

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

    accu::bbox<P> bb_;
    // FIXME: Add invariant  bb_.is_valid() <=> npoints() != 0
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename P>
  inline
  p_set<P>::p_set()
  {
  }

  template <typename P>
  inline
  bool
  p_set<P>::has(const P& p) const
  {
    return this->super_::has(p);
  }

  template <typename P>
  inline
  std::size_t
  p_set<P>::npoints() const
  {
    return this->super_::nelements();
  }

  template <typename P>
  inline
  p_set<P>&
  p_set<P>::insert(const P& p)
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
  p_set<P>::operator[](unsigned i) const
  {
    mln_precondition(i < npoints());
    return this->super_::element(i);
  }

  template <typename P>
  inline
  void
  p_set<P>::clear()
  {
    this->super_::clear();
    bb_.init();
  }

  template <typename P>
  inline
  const box_<mln_point(P)>&
  p_set<P>::bbox() const
  {
    mln_precondition(npoints() != 0);
    return bb_.to_result();
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_P_SET_HH
