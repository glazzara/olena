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

#ifndef MLN_CORE_INTERNAL_SITE_SET_IMPL_HH
# define MLN_CORE_INTERNAL_SITE_SET_IMPL_HH

/// \file
/// Provide implementation for optional site set methods.

# include <mln/core/concept/site_set.hh>
# include <mln/accu/shape/bbox.hh>



namespace mln
{

  namespace internal
  {

    /// The facade.
    /// Parameter \c Sc is the site set component.
    template <typename Sc>
    struct site_set_impl;



    // For .nsites()

    template <typename trait_nsites, typename Sc>
    struct site_set_impl__nsites
    {
    protected:
      template <typename T>
      void update_nsites_(const T&); // No-op.
    };

    template <typename Sc>
    struct site_set_impl__nsites< trait::site_set::nsites::known, Sc>
    {
      site_set_impl__nsites();
      unsigned nsites() const;
    protected:
      void update_nsites_(const mln_site(Sc)& p);
      template <typename S>
      void update_nsites_(const Site_Set<S>& s);
      unsigned nsites_;
    };


    // For .bbox()

    template <typename trait_bbox, typename Sc>
    struct site_set_impl__bbox
    {
      typedef const mln::box<mln_site(Sc)>& q_box;
      q_box bbox() const;

    protected:
      void update_bbox_(const mln_site(Sc)& p);
      template <typename S>
      void update_bbox_(const Site_Set<S>& s);

      accu::shape::bbox<mln_site(Sc)> bb_;
    };

    template <typename Sc>
    struct site_set_impl__bbox< trait::site_set::nsites::unknown, Sc >
    {
    protected:
      template <typename T>
      void update_bbox_(const T&); // No-op.
    };


    // Facade.

    template <typename Sc>
    struct site_set_impl
      : site_set_impl__nsites< mln_trait_site_set_nsites(Sc), Sc>,
	site_set_impl__bbox  < mln_trait_site_set_bbox(Sc),   Sc>
    {
    };




# ifndef MLN_INCLUDE_ONLY


    // site_set_impl__nsites

    template <typename trait_nsites, typename Sc>
    template <typename T>
    inline
    void
    site_set_impl__nsites<trait_nsites, Sc>::update_nsites_(const T&)
    {
      // No-op.
    }

    template <typename Sc>
    inline
    site_set_impl__nsites<trait::site_set::nsites::known, Sc>::site_set_impl__nsites()
      : nsites_(0)
    {
    }

    template <typename Sc>
    inline
    unsigned
    site_set_impl__nsites<trait::site_set::nsites::known, Sc>::nsites() const
    {
      return nsites_;
    }

    template <typename Sc>
    inline
    void
    site_set_impl__nsites<trait::site_set::nsites::known, Sc>::update_nsites_(const mln_site(Sc)&)
    {
      ++nsites_;
    }

    template <typename Sc>
    template <typename S>
    inline
    void
    site_set_impl__nsites<trait::site_set::nsites::known, Sc>::update_nsites_(const Site_Set<S>& s)
    {
      nsites_ += exact(s).nsites();
    }

    // site_set_impl__bbox

    template <typename trait_bbox, typename Sc>
    inline
    typename site_set_impl__bbox<trait_bbox, Sc>::q_box
    site_set_impl__bbox<trait_bbox, Sc>::bbox() const
    {
      return bb_.to_result();
    }

    template <typename trait_bbox, typename Sc>
    inline
    void
    site_set_impl__bbox<trait_bbox, Sc>::update_bbox_(const mln_site(Sc)& p)
    {
      bb_.take(p);
    }

    template <typename trait_bbox, typename Sc>
    template <typename S>
    inline
    void
    site_set_impl__bbox<trait_bbox, Sc>::update_bbox_(const Site_Set<S>& s)
    {
      bb_.take(exact(s).bbox());
    }

    template <typename Sc>
    template <typename T>
    inline
    void
    site_set_impl__bbox< trait::site_set::nsites::unknown, Sc >::update_bbox_(const T&)
    {
      // No-op.
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_SITE_SET_IMPL_HH
