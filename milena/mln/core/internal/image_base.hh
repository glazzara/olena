// Copyright (C) 2007, 2008, 2009, 2012, 2013 EPITA Research and
// Development Laboratory (LRDE)
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

#ifndef MLN_CORE_INTERNAL_IMAGE_BASE_HH
# define MLN_CORE_INTERNAL_IMAGE_BASE_HH

/// \file
///
/// Definition of the common base class for all images.
///
/// \todo Provide coord iff the site is a grid point.


# include <mln/core/concept/image.hh>
# include <mln/core/grids.hh>
# include <mln/core/trait/qlf_value.hh>
# include <mln/core/internal/check/image_all.hh>
# include <mln/core/internal/data.hh>
# include <mln/core/internal/morpher_lvalue.hh>
# include <mln/util/tracked_ptr.hh>
# include <mln/value/set.hh>
# include <mln/value/super_value.hh>


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


    /*!
      \brief A base class for images.

      Parameter \p T is the image value type.
      Parameter \p S is the image site set type.

      \verbatim
                  image_base
                       ^
                       |
           ---------------------------
          |                           |
     image_primary               image_morpher
          ^                            ^
          |                            |
          |		     -----------------------------------------
      	  | 		    |                    |                    |
     pw_image_base    image_domain_morpher  image_value_morpher   image_identity

     \endverbatim
    */
    template <typename T, typename S, typename E>
    struct image_base
      :
      public image_checked_<E>

    {
      /// Value associated type.
      typedef T value;

      /// Eligible-value-set associated type.
      typedef mln::value::set<T> t_eligible_values_set;

      // Return the set of the image eligigle values
      const t_eligible_values_set& values_eligible() const;

      /// Value space associated type.
      typedef mln::value::set<
	typename mln::value::super_value<T>::ret > t_values_space;

      /// Return the value space of the image.
      const t_values_space& values_space() const;


      /// Site_Set associated type.
      typedef S domain_t;

      /// Point_Site associated type.
      typedef mln_psite(S) psite;

      /// Site associated type.
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
      bool is_valid() const;

      // FIXME: Add void init_data(..);


      /// Assignment operator (performs a shallow assignment).
      image_base& operator=(const image_base& rhs);

      /// Copy constructor (performs a shallow copy).
      image_base(const image_base& rhs);


      /// \cond INTERNAL_API

      /*!
	\brief Give an identifier of this image.

	When several image variables designate the same image, they
	share the same identifier.
      */
      const void* id_() const;

      /// \brief Hook to the image data.
      const util::tracked_ptr< internal::data<E> >& hook_data_() const;

      /// \endcond


      /// Detach data from an image (free it if nobody else hold it).
      void destroy();

    protected:

      /// Constructor without argument.
      image_base();

      /// \cond INTERNAL_API
      /// Internal data, sharable by several images.
      util::tracked_ptr< internal::data<E> > data_;
      /// \endcond
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename T, typename S, typename E>
    inline
    image_base<T, S, E>::image_base()
    {
    }

    template <typename T, typename S, typename E>
    inline
    image_base<T, S, E>::image_base(const image_base& rhs)
      : image_checked_<E>()
    {
      mln_precondition(exact(rhs).is_valid()); // FIXME: Is-it too restrictive?
      this->data_ = rhs.data_;
    }

    template <typename T, typename S, typename E>
    inline
    image_base<T, S, E>&
    image_base<T, S, E>::operator=(const image_base<T, S, E>& rhs)
    {
      mln_precondition(exact(rhs).is_valid()); // FIXME: Is-it too restrictive?
      if (& rhs == this) // || ! exact(rhs).is_valid())
	return *this;
      this->data_ = rhs.data_;
      return *this;
    }

    template <typename T, typename S, typename E>
    inline
    const void*
    image_base<T, S, E>::id_() const
    {
      return data_.ptr_;
    }

    template <typename T, typename S, typename E>
    inline
    bool
    image_base<T, S, E>::is_valid() const
    {
      return data_ != 0;
    }

    template <typename T, typename S, typename E>
    inline
    bool
    image_base<T, S, E>::has(const psite& p) const
    {
      mln_precondition(exact(this)->is_valid());
      return exact(this)->domain().has(p);
    }

    template <typename T, typename S, typename E>
    inline
    std::size_t
    image_base<T, S, E>::nsites() const
    {
      mlc_equal(mln_trait_site_set_nsites(S),
		mln::trait::site_set::nsites::known)::check();
      mln_precondition(exact(this)->is_valid());
      return exact(this)->domain().nsites();
    }

    template <typename T, typename S, typename E>
    inline
    const typename image_base<T, S, E>::t_eligible_values_set&
    image_base<T, S, E>::values_eligible() const
    {
      return t_eligible_values_set::the();
    }

    template <typename T, typename S, typename E>
    inline
    const typename image_base<T, S, E>::t_values_space&
    image_base<T, S, E>::values_space() const
    {
      return t_values_space::the();
    }

    template <typename T, typename S, typename E>
    inline
    void
    image_base<T, S, E>::destroy()
    {
      data_.clean_();
    }

    template <typename T, typename S, typename E>
    inline
    const util::tracked_ptr< internal::data<E> >&
    image_base<T, S, E>::hook_data_() const
    {
      return data_;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln

#endif // ! MLN_CORE_INTERNAL_IMAGE_BASE_HH
