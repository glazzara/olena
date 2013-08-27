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

#ifndef MLN_CORE_INTERNAL_IMAGE_MORPHER_HH
# define MLN_CORE_INTERNAL_IMAGE_MORPHER_HH

/// \file
///
/// Definition of a base class for image morphers.
///
/// \todo Add the appropriate checks in .rw().

# include <mln/core/internal/image_base.hh>
# include <mln/metal/const.hh>
# include <mln/metal/is_const.hh>
# include <mln/metal/is_not_const.hh>


namespace mln
{

  namespace internal
  {

    /*!
      \internal
      \brief A base class for images that are morphers.

      Parameter \c I is the underlying-morphed image type.
    */
    template <typename I, typename T, typename S, typename E>
    class image_morpher : public image_base<T, S, E>
    {
    public:

      /// Delegatee associated type.
      typedef I delegatee;

      /// \cond INTERNAL_API
      /// Return the delegatee_ pointer; default code.
      mlc_const(I)* delegatee_() const;

      /// Return the delegatee_ pointer (non-const version); default code.
      I* delegatee_();
      /// \endcond


      /// Unmorph image associated type.
      typedef I unmorph;

      /// \cond INTERNAL_API
      /// Give the morphed image (mutable version).
      I& unmorph_();

      /// Give the morphed image (const version).
      mlc_const(I)& unmorph_() const;
      /// \endcond

      /* \brief Test if this image has been initialized; default impl.
       *
       * This default impl is stronger than the one inherited from
       * image_base because it also tests that the morphed image is
       * also initialized.
       */
      bool is_valid() const;


      /// State that the morpher is writable.  This allows for C++ to
      /// use it as a mutable object even if it is a temporary object.
      E& rw();


    protected:
      image_morpher();
    };

  } // end of namespace mln::internal



  namespace impl
  {

    // Default is delegation for morphers.

//     template <typename Subject, typename T,
// 	      typename I, typename S, typename E>
//     void init_(Subject s, T& target,
// 	       const internal::image_morpher<I,S,E>& model);

// FIXME: Lines above have been inactivated because they are either
// prioritary or ambiguous.
    /// \cond INTERNAL_API
    template <typename Subject, typename T,
	      typename J>
    void init_(Subject s, T& target, const Image<J>& model);
    /// \endcond

  } // end of namespace mln::impl



# ifndef MLN_INCLUDE_ONLY

  namespace internal
  {

    template <typename I, typename T, typename S, typename E>
    inline
    image_morpher<I, T, S, E>::image_morpher()
    {
    }

    template <typename I, typename T, typename S, typename E>
    inline
    mlc_const(I)*
    image_morpher<I, T, S, E>::delegatee_() const
    {
      return this->data_ == 0 ? 0 : & this->data_->ima_;
    }

    template <typename I, typename T, typename S, typename E>
    inline
    I*
    image_morpher<I, T, S, E>::delegatee_()
    {
      return this->data_ == 0 ? 0 : & this->data_->ima_;
    }

    template <typename I, typename T, typename S, typename E>
    inline
    I&
    image_morpher<I, T, S, E>::unmorph_()
    {
      I* ptr = delegatee_();
      mln_assertion(ptr != 0);
      return *ptr;
    }

    template <typename I, typename T, typename S, typename E>
    inline
    mlc_const(I)&
    image_morpher<I, T, S, E>::unmorph_() const
    {
      mlc_const(I)* ptr = delegatee_();
      mln_assertion(ptr != 0);
      return *ptr;
    }

    template <typename I, typename T, typename S, typename E>
    inline
    bool
    image_morpher<I, T, S, E>::is_valid() const
    {
      return
	this->data_ != 0 &&
	this->delegatee_() != 0 &&
	this->delegatee_()->is_valid();
    }

    template <typename I, typename T, typename S, typename E>
    inline
    E&
    image_morpher<I, T, S, E>::rw()
    {
      mlc_is_not_const(I)::check();
      mlc_equal(mln_trait_image_value_io(I),
		mln::trait::image::value_io::read_write)::check();
      // FIXME Nicolas: pw_io == read_write OR vw_io == read_write...
      return exact(*this);
    }

  } // end of namespace mln::internal



//   template <typename Subject, typename T,
// 	    typename I, typename S, typename E>
//   void init_(Subject s, T& target,
// 	     const internal::image_morpher<I,S,E>& model)
//   {
//     // FIXME: Precondition.
//     init_(s, target, * model.delegatee_());
//   }

    template <typename Subject, typename T,
	      typename J>
    inline
    void init_(Subject s, T& target, const Image<J>& model_)
    {
      mlc_is(mln_trait_image_category(J),
	     trait::image::category::morpher)::check();
      const J& model = exact(model_);
      init_(s, target, * model.delegatee_());
    }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_IMAGE_MORPHER_HH
