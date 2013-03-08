// Copyright (C) 2007, 2008, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_CORE_INTERNAL_IMAGE_IDENTITY_HH
# define MLN_CORE_INTERNAL_IMAGE_IDENTITY_HH

/// \file
///
/// Definition of a base class for image morphers w.r.t. identity.
///
/// \todo Move "fastest impl" elsewhere; it can be used by some other
/// classes.

# include <mln/core/internal/image_morpher.hh>


namespace mln
{

  namespace internal
  {


    // Fastest.

    template <typename trait_speed, typename I, typename E>
    struct image_identity_impl__fastest
    {
      // Nothing.
    };

    template <typename I, typename E>
    struct image_identity_impl__fastest< mln::trait::image::speed::fastest, I, E >
    {
    private:

      mlc_const(I)& del_() const
      {
	return * internal::force_exact<const E>(*this).delegatee_();
      }

      I& del_()
      {
	return * internal::force_exact<E>(*this).delegatee_();
      }

    public:

      int delta_offset(const mln_deduce(I, psite, delta)& dp) const
      {
	return del_().delta_offset(dp);
      }

      mln_site(I) point_at_offset(unsigned i) const
      {
	return del_().point_at_offset(i);
      }

      unsigned border() const
      {
	return del_().border();
      }

      mln_qlf_value(I)* buffer()
      {
	return del_().buffer();
      }

      const mln_value(I)* buffer() const
      {
	return del_().buffer();
      }

      mln_rvalue(I) element(unsigned i) const
      {
	return del_().element(i);
      }

      mln_lvalue(I) element(unsigned i)
      {
	return del_().element(i);
      }

      unsigned nelements() const
      {
	return del_().nelements();
      }
    };

    // Facade.

    template <typename I, typename E>
    struct image_identity_impl
      : image_identity_impl__fastest< mln_trait_image_speed(E), I, E >
    {
    };



    /*!
      \internal
      \brief A base class for image morphers w.r.t. identity.

      Parameter \p S is a point set type.

    */
    template <typename I, typename S, typename E>
    class image_identity
      : public image_identity_impl<I, E>,
	public image_morpher<I, mln_value(I), S, E>
    {
    public:

      /// Value associated type.
      typedef mln_value(I) value;

      /// Return type of read-only access.
      typedef mln_rvalue(I) rvalue;

      /// Return type of read-write access.
      typedef mln_morpher_lvalue(I) lvalue;

      /// Read-only access of pixel value at point site \p p.
      rvalue operator()(const mln_psite(S)& p) const;

      /// Read-write access of pixel value at point site \p p.
      lvalue operator()(const mln_psite(S)& p);

      // FIXME Matthieu: Doc!  Cf. core/concept/doc/image
      const mln_domain(I)& domain() const;
      bool has(const mln_psite(I)& p) const;

    protected:

      /// Constructor.
      image_identity();
    };



# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename S, typename E>
    inline
    image_identity<I, S, E>::image_identity()
    {
    }

    template <typename I, typename S, typename E>
    inline
    mln_rvalue(I)
    image_identity<I, S, E>::operator()(const mln_psite(S)& p) const
    {
      mln_precondition(this->delegatee_() != 0);
      return this->delegatee_()->operator()(p);
    }

    template <typename I, typename S, typename E>
    inline
    typename image_identity<I, S, E>::lvalue
    image_identity<I, S, E>::operator()(const mln_psite(S)& p)
    {
      mln_precondition(this->delegatee_() != 0);
      return this->delegatee_()->operator()(p);
    }

    template <typename I, typename S, typename E>
    inline
    const mln_domain(I)&
    image_identity<I, S, E>::domain() const
    {
      mln_precondition(this->delegatee_() != 0);
      return this->delegatee_()->domain();
    }

    template <typename I, typename S, typename E>
    inline
    bool
    image_identity<I, S, E>::has(const mln_psite(I)& p) const
    {
      mln_precondition(this->delegatee_() != 0);
      return this->delegatee_()->has(p);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_IMAGE_IDENTITY_HH
