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

#ifndef MLN_CORE_CLOCK_NEIGHB_HH
# define MLN_CORE_CLOCK_NEIGHB_HH

/*! \file mln/core/clock_neighb.hh
 *
 * \brief Definition of the generic neighborhood class mln::clock_neighb.
 */

# include <mln/core/concept/neighborhood.hh>
# include <mln/core/dpoint.hh>
# include <mln/core/site_set/p_array.hh>

namespace mln
{

  // fwd decls
  template <typename D> class dpsites_fwd_piter;
  template <typename D> class dpsites_bkd_piter;


  /*! \brief Generic neighborhood class.
   *
   * This neighborhood of window is just like a set of delta-psites.
   * The parameter is \c D, type of delta-psite.
   */
  template <typename D>
  struct clock_neighb : public Neighborhood< clock_neighb <D> >
  {
    /// Dpsite associated type.
    typedef D dpsite;

    /// Site associated type.
    typedef mln_psite(D) point;

    /*! \brief Site_Iterator type to browse the psites of a generic
     * neighborhood w.r.t. the ordering of delta-psites.
     */
    typedef dpsites_fwd_piter<D> fwd_niter;

    /*! \brief Site_Iterator type to browse the psites of a generic
     * neighborhood w.r.t. the reverse ordering of delta-psites.
     *
     * !!! Be careful the start delta psite become the last now.
     */
    typedef dpsites_bkd_piter<D> bkd_niter;

    /*! \brief Same as fwd_niter.
     */
    typedef fwd_niter niter;

    /*! \brief Constructor without argument.
     *
     * The constructed neighborhood is empty. You have to use insert()
     * to proceed to the neighborhood definition.
     */
    clock_neighb();

    /*! \brief Insert a delta-psite \p dp in the neighborhood
     *  definition.
     *
     * \param[in] dp The delta-psite to insert.
     *
     * This method also insert the symmetrical delta-psite, - \p dp,
     * in the neighborhood definition; thus the client has not to
     * ensure the symmetry property; that is automatic.
     */
    clock_neighb<D>& append(const D& dp);
    /// \}
    const std::vector<D>& vect() const
    {
      return vec_;
    }

    std::vector<D>	vec_;
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename D>
  inline
  clock_neighb<D>::clock_neighb()
  {
  }

  template <typename D>
  inline
  clock_neighb<D>&
  clock_neighb<D>::append(const D& dp)
  {
    vec_.push_back(dp);
    return *this;
  }


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


# include <mln/core/dpsites_piter.hh>


#endif // ! MLN_CORE_CLOCK_NEIGHB_HH
