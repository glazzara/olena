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

#ifndef MLN_CORE_INTERNAL_IMAGE_BASE_HH
# define MLN_CORE_INTERNAL_IMAGE_BASE_HH

/*! \file mln/core/internal/image_base.hh
 *
 * \brief Definition of the common base class for all images.
 */

# include <mln/core/concept/image.hh>
# include <mln/core/grids.hh>
# include <mln/core/trait/qlf_value.hh>
# include <mln/core/internal/check/image_all.hh>
# include <mln/core/internal/data.hh>
# include <mln/core/internal/morpher_lvalue.hh>
# include <mln/util/tracked_ptr.hh>


//              image_base
//                   ^
//                   |
//       ---------------------------
//      |                           |
// image_primary               image_morpher
//                                  ^
//                                  |
//              -----------------------------------------
//             |                    |                    |
//     image_domain_morpher  image_value_morpher   image_identity


namespace mln
{

  namespace internal
  {


    template <typename E>
    struct image_checked_
      :
      public check::image_all_<E>,
      public Image<E>
    {
    };



    /// \internal A base class for images.  Parameter \p S is the
    /// image site set type.
    template <typename S, typename E>
    struct image_base
      :
      public image_checked_<E>

    {
      /// Site_Set associated type.
      typedef S pset;

      /// Point_Site associated type.
      typedef mln_psite(S) psite;

      /// Point associated type.
      typedef mln_site(S) site;


      /// Forward Site_Iterator associated type.
      typedef mln_fwd_piter(S) fwd_piter;

      /// Backward Site_Iterator associated type.
      typedef mln_bkd_piter(S) bkd_piter;


      /// Site_Iterator associated type; default definition is
      /// fwd_piter.
      typedef fwd_piter        piter;


      /// Test if \p p belongs to the image domain.
      bool has(const psite& p) const;

      /// Give the number of sites of the image domain.
      std::size_t nsites() const;

      /// Test if this image has been initialized; default impl.
      bool has_data() const;

      // FIXME: Add void init_data(..);


      /// Assignment operator.
      image_base& operator=(const image_base& rhs);

      /// Copy constructor.
      image_base(const image_base& rhs);

      /// Detach data from an image (free it if nobody else hold it).
      void destroy();

      const util::tracked_ptr< internal::data<E> >& hook_data_() const { return data_; }

      const void* id_() const { return data_.ptr_; }

    protected:
      image_base();

      // Internal data, sharable by several images.
      util::tracked_ptr< internal::data<E> > data_;
    };



# ifndef MLN_INCLUDE_ONLY

    template <typename S, typename E>
    inline
    image_base<S,E>::image_base()
    {
    }

    template <typename S, typename E>
    inline
    image_base<S,E>::image_base(const image_base& rhs)
      : image_checked_<E>()
    {
      mln_precondition(exact(rhs).has_data()); // FIXME: Is-it too restrictive?
      this->data_ = rhs.data_;
    }

    template <typename S, typename E>
    inline
    image_base<S,E>&
    image_base<S,E>::operator=(const image_base<S,E>& rhs)
    {
      mln_precondition(exact(rhs).has_data()); // FIXME: Is-it too restrictive?
      if (& rhs == this) // || ! exact(rhs).has_data())
	return *this;
      this->data_ = rhs.data_;
      return *this;
    }

    template <typename S, typename E>
    inline
    bool
    image_base<S,E>::has_data() const
    {
      return data_ != 0;
    }

    template <typename S, typename E>
    inline
    bool
    image_base<S,E>::has(const psite& p) const
    {
      mln_precondition(exact(this)->has_data());
      return exact(this)->domain().has(p);
    }

    template <typename S, typename E>
    inline
    std::size_t
    image_base<S,E>::nsites() const
    {
      mlc_equal(mln_trait_site_set_nsites(S),
		mln::trait::site_set::nsites::known)::check();
      mln_precondition(exact(this)->has_data());
      return exact(this)->domain().nsites();
    }

    template <typename S, typename E>
    inline
    void
    image_base<S,E>::destroy()
    {
      data_.clean_();
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_IMAGE_BASE_HH
