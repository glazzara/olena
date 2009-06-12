// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_CORE_IMAGE_TRANS_IMAGE_HH
# define MLN_CORE_IMAGE_TRANS_IMAGE_HH

/*! \file sandbox/fred/trans_image.hh
 *
 * \brief Definition of the "transformed" image class mln::trans_image.
 */

# include <mln/core/internal/image_morpher.hh>
# include <mln/core/site_set/box.hh>
# include <mln/value/set.hh>
# include <mln/core/concept/function.hh>
# include <mln/accu/shape/bbox.hh>

namespace mln
{

  // Fwd decl.
  template <typename I, typename F> struct trans_image;

  namespace internal
  {

    /// Data structure for \c mln::trans_image<I>.
    template <typename I, typename F>
    struct data< trans_image<I, F> >
    {
      /// \brief Build the data object held by a trans_image.
      ///
      /// \param ima  The underlying image.
      /// \param fun  The underlying functor.
      data(I& ima, const F& fun, const box<mln_psite(I)>& box);

      /// Underlying image.
      I ima_;
      /// Underlying transformation function.
      // FIXME: do we need to conserve original transformation or just inverse ?
      //F fun_;
      typename F::inverse funinv_;
      /// The bounding box of the morphed image.
      box<mln_psite(I)> box_;
    };

  } // end of namespace mln::internal


  /*! \brief Transformed image class.
   *
   *  Applies an fun::p2p functor to coordinates of image pixels.
   *
   *  \warning This class only works on images whose domain is a box.
   *  \warning Transformation function must transform domain to a
   *           domain with same cardinality.
   *  \warning You need to give the INVERSE of the desired transformation.
   */
  template <typename I, typename F>
  class trans_image
    : public internal::image_morpher<I, mln_value(I), mln_pset(I), trans_image<I, F> >
  {
  public:
    /// Super type.
    typedef internal::image_morpher< I, mln_value(I), mln_pset(I),
				     trans_image<I, F> > super_;

    /// Point_Site associated type.
    typedef mln_psite(I) psite;

    /// Value associated type.
    typedef mln_value(I) value;

    /// Type returned by the read-write pixel value operator.
    typedef mln_morpher_lvalue(I) lvalue;

    /// Return type of read-only access.
    typedef mln_rvalue(I) rvalue;

    /// Skeleton.
    typedef trans_image< tag::image_<I>, F > skeleton;

  public:
    /// \brief Build a transformation of an image.
    ///
    /// The trans_image morpher allows to apply a p2p functor to every cell
    /// of an image.
    /// The functor \a fun must transform points so that transformed \a ima
    /// image domain remains a box with same cardinality.
    ///
    /// \param ima  The underlying image.
    /// \param fun  p2p functor to apply.
    trans_image(I& ima, const F& fun);

    /// Initialize an empty image.
    void init_(I& ima, const F& fun);

    /// Test if this image has been initialized.
    bool is_valid() const;

    /// Test if a pixel value is accessible at \p p.
    bool has(const mln_psite(I)& p) const;

    /// Give the definition domain.
    const box<mln_psite(I)>& domain() const;

    /// Read-only access of pixel value at point site \p p.
    mln_rvalue(I) operator()(const mln_psite(I)& p) const;

    /// Read-write access of pixel value at point site \p p.
    mln_morpher_lvalue(I) operator()(const mln_psite(I)& p);


  protected:
    /// Transform \a p point through functor.
    mln_psite(I) transform_(const mln_psite(I)& p) const;
    box<mln_psite(I)> transform_(const box<mln_psite(I)>& p) const;
  };

# ifndef MLN_INCLUDE_ONLY

  namespace internal
  {

    // internal::data< trans_image<I,S> >

    template <typename I, typename F>
    inline
    data< trans_image<I, F> >::data(I& ima, const F& fun, const box<mln_psite(I)>& box)
      : ima_ (ima),
        // FIXME: do we need to conserve original transformation or just inverse ?
        // fun_ (fun),
	funinv_ (fun.inv ()),
	box_ (box)
    {
    }

  } // end of namespace mln::internal


  template <typename I, typename F>
  inline
  trans_image<I, F>::trans_image(I& ima, const F& fun)
  {
    init_(ima, fun);
  }

  template <typename I, typename F>
  inline
  void
  trans_image<I, F>::init_(I& ima, const F& fun)
  {
    typedef mln_psite(I) P;
    mln_precondition(ima.is_valid());

    /* FIXME: Add a precondition on the fact that the domain of ima is
       a box.  */

    // Create transformed bounding box
    accu::shape::bbox<P> tbox;

    tbox.take_as_init (fun (ima.domain ().pmin ()));
    tbox.take (fun (ima.domain ().pmax ()));

    this->data_ = new internal::data< trans_image<I, F> >(ima, fun, tbox);
  }


  template <typename I, typename F>
  inline
  bool trans_image<I, F>::is_valid() const
  {
    mln_invariant(this->delegatee_()->is_valid());
    return true;
  }

  template <typename I, typename F>
  inline
  bool
  trans_image<I, F>::has(const mln_psite(I)& p) const
  {
    mln_precondition(this->is_valid());
    return this->delegatee_()->has(transform_(p));
  }

  template <typename I, typename F>
  inline
  mln_psite(I)
  trans_image<I, F>::transform_(const mln_psite(I)& p) const
  {
    return this->data_->funinv_(p);
  }

  template <typename I, typename F>
  inline
  box<mln_psite(I)>
  trans_image<I, F>::transform_(const box<mln_psite(I)>& p) const
  {
    typedef mln_psite(I) P;

    // Create transformed bounding box
    accu::shape::bbox<P> tbox;

    tbox.take_as_init (this->data_->funinv_ (this->data_->ima_.domain ().pmin ()));
    tbox.take (this->data_->funinv_ (this->data_->ima_.domain ().pmax ()));

    return tbox;
  }

  template <typename I, typename F>
  inline
  const box<mln_psite(I)>&
  trans_image<I, F>::domain() const
  {
    mln_precondition(this->is_valid());
    return this->data_->box_;
  }

  template <typename I, typename F>
  inline
  mln_rvalue(I)
  trans_image<I, F>::operator()(const mln_psite(I)& p) const
  {
    mln_precondition(this->has(p));
    return (*this->delegatee_())(transform_(p));
  }

  template <typename I, typename F>
  inline
  mln_morpher_lvalue(I)
  trans_image<I, F>::operator()(const mln_psite(I)& p)
  {
    mln_precondition(this->has(p));
    return (*this->delegatee_())(transform_(p));
  }

  template <typename I, typename F>
  inline
  trans_image<I, F>
  transform_image(Image<I>& ima, const Function_v2v<F>& fun)
  {
    trans_image<I, F> tmp(exact(ima), exact(fun));
    return tmp;
  }

  template <typename I, typename F>
  inline
  trans_image<const I, F>
  transform_image(const Image<I>& ima, const Function_v2v<F>& fun)
  {
    trans_image<const I, F> tmp(exact(ima), exact(fun));
    return tmp;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_T_IMAGE_HH
