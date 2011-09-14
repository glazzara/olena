// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
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

#ifndef MLN_CORE_IMAGE_MORPH_T_IMAGE_HH
# define MLN_CORE_IMAGE_MORPH_T_IMAGE_HH

/// \file mln/core/image/morph/t_image.hh
///
/// Definition of the "transposed" image class mln::t_image.

# include <mln/core/internal/image_morpher.hh>
# include <mln/core/site_set/box.hh>
# include <mln/value/set.hh>

namespace mln
{

  // Fwd decl.
  template <typename I> struct t_image;

  namespace internal
  {

    /// Data structure for \c mln::t_image<I>.
    template <typename I>
    struct data< t_image<I> >
    {
      /// \brief Build the data object held by a t_image.
      ///
      /// \param ima  The underlying image.
      /// \param dim1 The first dimension to be swapped.
      /// \param dim2 The second dimension to be swapped.
      /// \param box  The bounding box (domain) of the morphed image.
      data(I& ima, unsigned dim1, unsigned dim2,
	    mln::box<mln_psite(I)>& box);

      /// Underlying image.
      I ima_;
      /// The swapped dimensions.
      /// \{
      unsigned dim1_;
      unsigned dim2_;
      /// \}
      /// The bounding box of the morphed image.
      mln::box<mln_psite(I)> box_;
    };

  } // end of namespace mln::internal


  /// Transposed image.
  ///
  /// Swap a couple of coordinates.
  ///
  /// \warning This class only works on images whose domain is a box.
  ///
  /// \ingroup modimagemorpher
  //
  template <typename I>
  class t_image
    : public internal::image_morpher<I, mln_value(I), mln_domain(I), t_image<I> >
  {
  public:
    /// Super type.
    typedef internal::image_morpher< I, mln_value(I), mln_domain(I),
				     t_image<I> > super_;

    /// Point_Site associated type.
    typedef mln_psite(I) psite;

    /// Value associated type.
    typedef mln_value(I) value;

    /// Type returned by the read-write pixel value operator.
    typedef mln_morpher_lvalue(I) lvalue;

    /// Return type of read-only access.
    typedef mln_rvalue(I) rvalue;

    /// Skeleton.
    typedef t_image< tag::image_<I> > skeleton;

  public:
    /// \brief Build the generalized transpose of an image.
    ///
    /// In the 2-dimension case, \a dim1 and \a dim2 must be resp. 1 and 2
    /// (or 2 and 1), and t_image is a transpose of a matrix with the
    /// usual meaning.
    ///
    /// In dimensions greater than 2, mln::t_image represents a view
    /// of the underlying image where two dimensions (i.e., two
    /// components of each point) have been exchanged.
    ///
    /// \param ima  The underlying image.
    /// \param dim1 The first dimension to be swapped.
    /// \param dim2 The second dimension to be swapped.
    t_image(I& ima, unsigned dim1, unsigned dim2);


    /// Initialize an empty image.
    void init_(I& ima, unsigned dim1, unsigned dim2);


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
    /// Exchange components \a dim1_ and \a dim2_ of point \a p.
    mln_psite(I) transpose_(const mln_psite(I)& p) const;
    /// Exchange components \a dim1 and \a dim2 of point \a p.
    mln_psite(I) exchange_coords_(const mln_psite(I)& p,
				  unsigned dim1, unsigned dim2) const;
  };


  /* FIXME: Shouldn't we move these elsewhere?  I (Roland) think mixing
     data structure and routines is confusing, even if they are
     closely related.  What about mln/geom/swap_coords.hh?  */
  template <typename I>
  t_image<I>
  swap_coords(Image<I>& ima, unsigned dim1, unsigned dim2);

  template <typename I>
  t_image<const I>
  swap_coords(const Image<I>& ima, unsigned dim1, unsigned dim2);



# ifndef MLN_INCLUDE_ONLY

  namespace internal
  {

    // internal::data< t_image<I,S> >

    template <typename I>
    inline
    data< t_image<I> >::data(I& ima,
			       unsigned dim1, unsigned dim2,
			       mln::box<mln_psite(I)>& box)
      : ima_(ima),
	dim1_(dim1), dim2_(dim2),
	box_(box)
    {
    }

  } // end of namespace mln::internal


  template <typename I>
  inline
  t_image<I>::t_image(I& ima, unsigned dim1, unsigned dim2)
  {
    init_(ima, dim1, dim2);
  }

  template <typename I>
  inline
  void
  t_image<I>::init_(I& ima, unsigned dim1, unsigned dim2)
  {
    mln_precondition(ima.is_valid());
    /* FIXME: Add a precondition on the fact that the domain of ima is
       a box.  */
    // We don't use mln::t_image::transpose here, since one its
    // prerequisite is that data_ is initialized, which is not done
    // yet at this point.
    box<mln_psite(I)> box(exchange_coords_(ima.bbox().pmin(), dim1, dim2),
			   exchange_coords_(ima.bbox().pmax(), dim1, dim2));
    this->data_ = new internal::data< t_image<I> >(ima, dim1, dim2, box);
  }


  template <typename I>
  inline
  bool t_image<I>::is_valid() const
  {
    mln_invariant(this->delegatee_()->is_valid());
    return true;
  }

  template <typename I>
  inline
  bool
  t_image<I>::has(const mln_psite(I)& p) const
  {
    mln_precondition(this->is_valid());
    return this->delegatee_()->has(transpose_(p));
  }

  template <typename I>
  inline
  mln_psite(I)
  t_image<I>::transpose_(const mln_psite(I)& p) const
  {
    mln_precondition(this->is_valid());
    return exchange_coords_(p, this->data_->dim1_, this->data_->dim2_);
  }

  template <typename I>
  inline
  mln_psite(I)
  t_image<I>::exchange_coords_(const mln_psite(I)& p,
			       unsigned dim1, unsigned dim2) const
  {
    mln_psite(I) tmp(p);
    tmp[dim1] = p[dim2];
    tmp[dim2] = p[dim1];
    return tmp;
  }

  template <typename I>
  inline
  const box<mln_psite(I)>&
  t_image<I>::domain() const
  {
    mln_precondition(this->is_valid());
    return this->data_->box_;
  }

  template <typename I>
  inline
  mln_rvalue(I)
  t_image<I>::operator()(const mln_psite(I)& p) const
  {
    mln_precondition(this->has(p));
    return (*this->delegatee_())(transpose_(p));
  }

  template <typename I>
  inline
  mln_morpher_lvalue(I)
  t_image<I>::operator()(const mln_psite(I)& p)
  {
    mln_precondition(this->has(p));
    return (*this->delegatee_())(transpose_(p));
  }

  template <typename I>
  inline
  t_image<I>
  swap_coords(Image<I>& ima, unsigned dim1, unsigned dim2)
  {
    typedef mln_psite(I) P;
    mln_precondition(dim1 != dim2);
    mln_precondition(dim1 <= P::dim && dim2 <= P::dim);
    t_image<I> tmp(exact(ima), dim1, dim2);
    return tmp;
  }

  template <typename I>
  inline
  t_image<const I>
  swap_coords(const Image<I>& ima, unsigned dim1, unsigned dim2)
  {
    typedef mln_psite(I) P;
    mln_precondition(dim1 != dim2);
    mln_precondition(dim1 <= P::dim && dim2 <= P::dim);
    t_image<const I> tmp(exact(ima), dim1, dim2);
    return tmp;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_MORPH_T_IMAGE_HH
