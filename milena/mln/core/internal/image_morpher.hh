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

#ifndef MLN_CORE_INTERNAL_IMAGE_MORPHER_HH
# define MLN_CORE_INTERNAL_IMAGE_MORPHER_HH

/*! \file mln/core/internal/image_morpher.hh
 *
 * \brief Definition of a base class for image morphers.
 */

# include <mln/core/internal/image_base.hh>
# include <mln/metal/const.hh>
# include <mln/metal/is_const.hh>


namespace mln
{

  namespace internal
  {

    /*! \brief A base class for images that are morphers.
     *
     * Parameter \c I is the underlying-morphed image type.
     *
     * \internal
     */
    template <typename I, typename S, typename E>
    class image_morpher_ : public image_base_<S, E>
    {
    public:

      typedef I delegatee;

      /// Return the delegatee_ pointer; default code.
      mlc_const(I)* delegatee_() const;

      /// Return the delegatee_ pointer (non-const version); default code.
      I* delegatee_();

      /* \brief Test if this image has been initialized; default impl.
       *
       * This default impl is stronger than the one inherited from
       * image_base_.
       */
      bool has_data() const;

      /// Convertion to the underlying (morphed) image.
      operator I() const; // FIXME: Very dangerous? Remove?

    protected:
      image_morpher_();
    };

  } // end of namespace mln::internal



  namespace impl
  {

    // Default is delegation for morphers.

//     template <typename Subject, typename T,
// 	      typename I, typename S, typename E>
//     void init_(Subject s, T& target,
// 	       const internal::image_morpher_<I,S,E>& model);

// FIXME: Lines above have been inactivated because they are either
// prioritary or ambiguous.

    template <typename Subject, typename T,
	      typename J>
    void init_(Subject s, T& target, const Image<J>& model);

  } // end of namespace mln::impl



# ifndef MLN_INCLUDE_ONLY

  namespace internal
  {

    template <typename I, typename S, typename E>
    image_morpher_<I,S,E>::image_morpher_()
    {
    }

    template <typename I, typename S, typename E>
    mlc_const(I)*
    image_morpher_<I,S,E>::delegatee_() const
    {
      return this->data_ == 0 ? 0 : & this->data_->ima_;
    }

    template <typename I, typename S, typename E>
    I*
    image_morpher_<I,S,E>::delegatee_()
    {
      return this->data_ == 0 ? 0 : & this->data_->ima_;
    }

    template <typename I, typename S, typename E>
    image_morpher_<I,S,E>::operator I() const
    {
      mln_precondition(exact(this)->has_data());
      return * this->delegatee_();
    }

    template <typename I, typename S, typename E>
    bool
    image_morpher_<I,S,E>::has_data() const
    {
      return
	this->data_ != 0 &&
	this->delegatee_() != 0 &&
	this->delegatee_()->has_data();
    }

  } // end of namespace mln::internal



//   template <typename Subject, typename T,
// 	    typename I, typename S, typename E>
//   void init_(Subject s, T& target,
// 	     const internal::image_morpher_<I,S,E>& model)
//   {
//     std::cout << "deleg... ";
//     // FIXME: Precondition.
//     init_(s, target, * model.delegatee_());
//   }

    template <typename Subject, typename T,
	      typename J>
    void init_(Subject s, T& target, const Image<J>& model_)
    {
      // FIXME: Precondition.
      // FIXME: Properly check that J is an internal::image_morpher_.
      const J& model = exact(model_);
      init_(s, target, * model.delegatee_());
    }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_IMAGE_MORPHER_HH
