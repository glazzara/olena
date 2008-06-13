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

#ifndef MLN_CORE_NEIGHB_HH
# define MLN_CORE_NEIGHB_HH

/*! \file mln/core/neighb.hh
 *
 * \brief Definition of the generic neighborhood class mln::neighb.
 */

# include <mln/core/internal/neighborhood_base.hh>
# include <mln/core/internal/basic_window_impl.hh>
# include <mln/core/internal/neighborhood_impl_mixin.hh>


namespace mln
{
 

  /*! \brief Generic neighborhood class.
   *
   * This neighborhood of window is just like a set of delta-points.
   * The parameter is \c D, type of delta-point.
   */
  template <typename D>
  struct neighb : internal::neighborhood_base< D, neighb<D> >,
		  internal::neighborhood_impl_mixin< internal::basic_window_impl< D, neighb<D> >,
						     neighb<D> >
  {
    /*! \brief Constructor without argument.
     *
     * The constructed neighborhood is empty. You have to use insert()
     * to proceed to the neighborhood definition.
     */
    neighb();

    // Overridden from internal::basic_window_impl so that it also
    // inserts \a -dp.
    neighb<D>& insert_(const D& dp);
  };
 

# ifndef MLN_INCLUDE_ONLY

  template <typename D>
  inline
  neighb<D>::neighb()
  {
  }

  template <typename D>
  inline
  neighb<D>&
  neighb<D>::insert_(const D& dp)
  {
    typedef neighb<D> self;
    typedef internal::basic_window_impl< D, neighb<D> > win_impl;
    typedef internal::neighborhood_impl_mixin< win_impl, neighb<D> > super_;
    this->super_::insert_( dp);
    this->super_::insert_(-dp);
    return *this;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_NEIGHB_HH
