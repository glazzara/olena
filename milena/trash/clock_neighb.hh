// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
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

#ifndef MLN_CORE_CLOCK_NEIGHB_HH
# define MLN_CORE_CLOCK_NEIGHB_HH

/// \file mln/core/clock_neighb.hh
///
/// Definition of the generic neighborhood class mln::clock_neighb.


# include <mln/core/concept/neighborhood.hh>
# include <mln/core/window.hh>
# include <mln/core/dpoint.hh>
# include <mln/core/site_set/p_array.hh>

namespace mln
{

  // forward declarations
  template <typename D> class dpsites_fwd_piter;
  template <typename D> class dpsites_bkd_piter;


  /// Generic neighborhood class.
  ///
  /// This neighborhood of window is just like a set of delta-psites.
  /// The parameter is \c D, type of delta-psite.
  ///
  template <typename D>
  struct clock_neighb : public Neighborhood< clock_neighb <D> >
  {
    /// Dpsite associated type.
    typedef D dpsite;

    /// Site associated type.
    typedef mln_psite(D) site;

    /// Window associated type.
    typedef mln::window<dpsite> window;

    /// Site_Iterator type to browse the psites of a generic
    /// neighborhood w.r.t. the ordering of delta-psites.
    ///
    typedef dpsites_fwd_piter<D> fwd_niter;

    /// Site_Iterator type to browse the psites of a generic
    /// neighborhood w.r.t. the reverse ordering of delta-psites.
    ///
    /// !!! Be careful the start delta psite become the last now.
    ///
    typedef dpsites_bkd_piter<D> bkd_niter;

    /// Same as fwd_niter.
    typedef fwd_niter niter;

    /// Constructor without argument.
    ///
    /// The constructed neighborhood is empty. You have to use insert()
    /// to proceed to the neighborhood definition.
    ///
    clock_neighb();

    /// Insert a delta-psite \p dp in the neighborhood
    ///  definition.
    ///
    /// \param[in] dp The delta-psite to insert.
    ///
    /// This method also insert the symmetrical delta-psite, - \p dp,
    /// in the neighborhood definition; thus the client has not to
    /// ensure the symmetry property; that is automatic.
    ///
    clock_neighb<D>& append(const D& dp);
    /// \}

    /// Return the dpsites of this neighborhood.
    const std::vector<D>& vect() const;

    /// Return the corresponding window.
    /// FIXME: not in constant time!
    mln::window<D> win() const;

    /// Return whether this neighborhood is valid.
    bool is_valid() const;

  private:
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

  template <typename D>
  inline
  const std::vector<D>&
  clock_neighb<D>::vect() const
  {
    return vec_;
  }

  template <typename D>
  inline
  mln::window<D>
  clock_neighb<D>::win() const
  {
    mln::window<D> result;
    for (unsigned i = 0; i < vec_.size(); ++i)
      result.insert(vec_[i]);
    return result;
  }

  template <typename D>
  inline
  bool
  clock_neighb<D>::is_valid() const
  {
    //FIXME: correct?
    return true;
  }


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


# include <mln/core/dpsites_piter.hh>


#endif // ! MLN_CORE_CLOCK_NEIGHB_HH
