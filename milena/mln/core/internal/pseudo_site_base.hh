// Copyright (C) 2008, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_CORE_INTERNAL_PSEUDO_SITE_BASE_HH
# define MLN_CORE_INTERNAL_PSEUDO_SITE_BASE_HH

/*! \file
 *
 * \brief Base class to factor code for pseudo site classes.
 */

# include <mln/core/concept/pseudo_site.hh>


namespace mln
{

  namespace internal
  {

    /*!
      \internal
      \brief A base class for pseudo sites.

      Parameter \c P is FIXME: a point site type.
    */
    template <typename P, typename E>
    struct pseudo_site_base_ : Pseudo_Site<E>,
                               proxy_impl<P, E>
    {
    private:
      typedef typename proxy_impl<P, E>::HOT_actual_subject q_site_;

    public:

      // The associated site type.
      typedef mlc_unqualif(q_site_) site;

      // Direct access to the site.
      const site& to_site() const;

    protected:
      pseudo_site_base_();
    };


#ifndef MLN_INCLUDE_ONLY

    template <typename P, typename E>
    inline
    pseudo_site_base_<P, E>::pseudo_site_base_()
    {
    }

    template <typename P, typename E>
    inline
    const typename pseudo_site_base_<P, E>::site&
    pseudo_site_base_<P, E>::to_site() const
    {
      return this->get_subject();
    }

#endif // ! MLN_INCLUDE_ONLY

  } // end of namespace internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_PSEUDO_SITE_BASE_HH
