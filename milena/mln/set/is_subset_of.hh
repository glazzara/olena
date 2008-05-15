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

#ifndef MLN_SET_IS_SUBSET_OF_HH
# define MLN_SET_IS_SUBSET_OF_HH

/*! \file mln/set/is_subset_of.hh
 *
 * \brief Routine to test if a set is a subset of another.
 */

# include <mln/core/concept/point_set.hh>

namespace mln
{

  namespace set
  {
    /*! \brief Test if a point set is a subset of another point set.
     *
     * \relates mln::Site_Set
     */
    template <typename Pl, typename Pr>
    bool
    is_subset_of(const Site_Set<Pl>& lhs, const Site_Set<Pr>& rhs);

# ifndef MLN_INCLUDE_ONL

    template <typename Pl, typename Pr>
    bool
    is_subset_of(const Site_Set<Pl>& lhs_, const Site_Set<Pr>& rhs_)
    {
      trace::entering("set::is_subset_of");
      Pl lhs = exact(lhs_);
      Pr rhs = exact(rhs_);

      if (lhs.npoints() > rhs.npoints())
      {
	trace::exiting("set::is_subset_of");
	return false;
      }

      mln_piter(Pl) p(lhs);
      for_all(p)
	{
	  if (!rhs.has(p))
	  {
	    trace::exiting("set::is_subset_of");
	    return false;
	  }
	}
      trace::exiting("set::is_subset_of");
      return true;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::set

} // end of namespace mln


#endif // ! MLN_SET_SUBSET_HH
