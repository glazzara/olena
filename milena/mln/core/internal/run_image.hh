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

#ifndef MLN_CORE_INTERNAL_RUN_IMAGE_HH
# define MLN_CORE_INTERNAL_RUN_IMAGE_HH

/*! \file mln/core/internal/run_image.hh
 *
 * \brief Definition of mln::internal::run_image_ class for internal use only
 */

# include <mln/core/internal/image_base.hh>
# include <mln/core/internal/run_pset.hh>
# include <mln/core/internal/run_psite.hh>
# include <mln/value/set.hh>

namespace mln
{

  namespace internal
  {

    /*! \brief Factorization class for run_image.
     *
     * Parameter \c P is the type of the image point.
     * Parameter \c E is the Exact type of the image.
     */
    template <typename P, typename E>
    class run_image_ : public image_base_<run_pset_<P>, E>
    {
    public:
      typedef run_pset_<P> pset;
      typedef mln_psite(pset) psite;

      /// Give the definition domain.
      const pset& domain() const;
      /// Test if \p p is valid.
      bool owns_(const psite& site) const;
    protected:
      /// domain of the image
      pset domain_;

      run_image_();
    };

# ifndef MLN_INCLUDE_ONLY

    template <typename P, typename E>
    run_image_<P, E>::run_image_()
    {
    }

    template <typename P, typename E>
    const typename run_image_<P, E>::pset&
    run_image_<P, E>::domain() const
    {
      return domain_;
    }

    template <typename P, typename E>
    bool
    run_image_<P, E>::owns_(const typename run_image_<P, E>::psite& site) const
    {
      return domain_.has(site);
    }
# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_RUN_IMAGE_HH
