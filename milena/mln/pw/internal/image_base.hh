// Copyright (C) 2009, 2010 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_PW_INTERNAL_IMAGE_BASE_HH
# define MLN_PW_INTERNAL_IMAGE_BASE_HH

/// \file
///
/// Definition of the common base class for all point-wise images.

# include <mln/core/internal/image_primary.hh>
# include <mln/core/routine/init.hh>
# include <mln/metal/unqualif.hh>
# include <mln/metal/not_equal.hh>
# include <mln/value/set.hh>


namespace mln
{


  // Forward declaration.
  namespace pw
  {

    namespace internal
    {

      template <typename F, typename S, typename E>
      class image_base;

    } // end of namespace mln::pw::internal

  } // end of namespace mln::pw

  namespace trait
  {

    // FIXME: the result type should *not* be qualified

    template <typename F, typename S, typename E>
      struct pw_image_
      : default_image_<mln_result(F),E>
    {
      private:
	typedef typename F::mutable_result mutable_result;
	typedef mlc_not_equal(mutable_result, void) is_mutable;

      public:
	// misc
	typedef trait::image::category::primary category;
	typedef trait::image::speed::fast       speed;
	typedef trait::image::size::regular     size;

	// value
	typedef trait::image::vw_io::none                    vw_io;
	typedef trait::image::vw_set::none                   vw_set;
	typedef trait::image::value_access::computed         value_access;
	typedef trait::image::value_storage::disrupted       value_storage;
	typedef trait::image::value_browsing::site_wise_only value_browsing;
	typedef trait::image::value_alignment::irrelevant    value_alignment;
	typedef mlc_if(is_mutable,
	    trait::image::value_io::read_write,
	    trait::image::value_io::read_only)    value_io;

	// site / domain
	typedef mlc_if(is_mutable,
	    trait::image::pw_io::read_write,
	    trait::image::pw_io::read) pw_io;
	typedef /* FIXME: depends on S */  undef  localization;
	typedef /* FIXME: depends on S */  undef  dimension;

	// extended domain
	typedef trait::image::ext_domain::none      ext_domain;
	typedef trait::image::ext_value::irrelevant ext_value;
	typedef trait::image::ext_io::irrelevant    ext_io;
    };

  } // end of namespace mln::trait


  namespace pw
  {

    namespace internal
    {

      /// A base class for point-wise images.
      /// Parameter \p F is the image value type.
      /// Parameter \p S is the image site set type.
      template <typename F, typename S, typename E>
      class image_base
	: public mln::internal::image_primary<mln_result(F), S, E >
      {
        typedef typename F::mutable_result mutable_result;
        typedef mlc_not_equal(mutable_result, void) is_mutable;

      public:
	/// Function associated type.
	typedef F function_t;

	/// Value associated type.
	typedef mln_result(F) value;

	/// Return type of read-only access.
	typedef mln_result(F) rvalue;

	/// Return type of read-write access.
	typedef mlc_if(is_mutable, mutable_result, mln_result(F)) lvalue;

	/// Initialize an empty image.
	void init_(const Function_v2v<F>& f, const Site_Set<S>& ps);

	/// Give the definition domain.
	const S& domain() const;

	/// Return the function which computes a site value.
	F function() const;

	/// Read-only access of pixel value at point site \p p.
	rvalue operator()(const mln_psite(S)& p) const;

	/// Read-write access returns either a temporary value (copy) or
	/// a reference in the case of a mutable function (container).
	lvalue operator()(const mln_psite(S)& p);

      protected:
	image_base();
	image_base(const Function_v2v<F>& f, const Site_Set<S>& ps);
      };

    } // end of namespace mln::pw::internal

  } // end of namespace mln::pw


  // init_

  template <typename F, typename S, typename E>
  void init_(tag::function_t, F& f, const pw::internal::image_base<F,S,E>& model);

  template <typename F1, typename F2, typename S, typename E>
  void init_(tag::function_t, F1& f, const pw::internal::image_base<F2,S,E>& model);


# ifndef MLN_INCLUDE_ONLY

  // init_

  template <typename F, typename S, typename E>
  void init_(tag::function_t, F& f, const pw::internal::image_base<F,S,E>& model)
  {
    f = model.function();
  }

  template <typename F1, typename F2, typename S, typename E>
  void init_(tag::function_t, F1& f, const pw::internal::image_base<F2,S,E>& model)
  {
    init_(tag::function, f, model.function());
  }


  namespace pw
  {

    namespace internal
    {

      template <typename F, typename S, typename E>
      inline
      image_base<F,S,E>::image_base()
      {
      }

      template <typename F, typename S, typename E>
      inline
      image_base<F,S,E>::image_base(const Function_v2v<F>& f, const Site_Set<S>& ps)
      {
        this->data_ = new mln::internal::data<E>(exact(f), exact(ps));
      }

      template <typename F, typename S, typename E>
      inline
      void
      image_base<F,S,E>::init_(const Function_v2v<F>& f, const Site_Set<S>& ps)
      {
        this->data_ = new mln::internal::data<E>(exact(f), exact(ps));
      }

      template <typename F, typename S, typename E>
      inline
      const S&
      image_base<F,S,E>::domain() const
      {
	return this->data_->domain_;
      }

      template <typename F, typename S, typename E>
      inline
      F
      image_base<F,S,E>:: function() const
      {
	return this->data_->f_;
      }


      template <typename F, typename S, typename E>
      inline
      typename image_base<F,S,E>::rvalue
      image_base<F,S,E>::operator()(const mln_psite(S)& p) const
      {
	mln_precondition(this->data_->domain_.has(p));
	return this->data_->f_(p);
      }

      template <typename F, typename S, typename E>
      inline
      typename image_base<F,S,E>::lvalue
      image_base<F,S,E>::operator()(const mln_psite(S)& p)
      {
	mln_precondition(this->data_->domain_.has(p));
	return this->data_->f_(p);
      }

    } // end of namespace mln::pw::internal

  } // end of namespace mln::pw

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_PW_INTERNAL_IMAGE_BASE_HH
