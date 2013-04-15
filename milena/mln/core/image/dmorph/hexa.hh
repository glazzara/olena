// Copyright (C) 2007, 2008, 2009, 2011, 2012, 2013 EPITA Research and
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

#ifndef MLN_CORE_IMAGE_DMORPH_HEXA_HH
# define MLN_CORE_IMAGE_DMORPH_HEXA_HH


/// \file
///
/// Definition of a morpher that makes hexagonal the mesh of an
/// image.

# include <mln/core/internal/image_domain_morpher.hh>
# include <mln/core/alias/point2d_h.hh>
# include <mln/core/alias/box2d_h.hh>
# include <mln/core/image/dmorph/hexa_piter.hh>


namespace mln
{

  // Forward declaration.
  template <typename I> struct hexa;


  namespace internal
  {
    /*!
      \brief Data structure for \c mln::hexa<I>.
    */
    template <typename I>
    struct data< hexa<I> >
    {
      data(I& ima, box2d_h b);

      I ima_;
      mln::box2d_h b_;
    };

  } // end of namespace mln::internal


  namespace trait
  {
    /*!
      FIXME: use the right properties.
    */
    template <typename I>
    struct image_< hexa<I> > : default_image_morpher< I, mln_value(I),
						       hexa<I> >
    {
      //     private:
      //       typedef mln_trait_image_data(I) I_data_;
      //       typedef mlc_equal(I_data_, trait::data::linear) I_data_are_linear_;
      //     public:

      typedef trait::image::category::domain_morpher category;

      typedef mln_trait_image_pw_io(I)	    pw_io; // un-write when I const
      typedef mln_trait_image_dimension(I)  dimension;

      // Extended domain
      typedef mln_trait_image_ext_domain(I) ext_domain;  // have a border only if I does.
      typedef mln_trait_image_ext_value(I)  ext_value;
      typedef mln_trait_image_ext_io(I)	    ext_io; // un-write when I const

      //       typedef mlc_if( I_data_are_linear_,
      // 		      trait::data::stored, // if linear then just stored
      // 		      I_data_ ) data;      // otherwise like I
    };

  } // end of namespace mln::trait


  /*! \brief hexagonal image class.
   *
   * The parameter \c I is the type of the base image.  This image class
   * which handles hexagonal grid.
   *
   * Ex :
   *         1  3  5  7  9  11
   *       0  2  4  6  8  10
   *     -------------------
   * 0 XX|  |  |  |  |  |  |XX
   *     ---------------------
   * 2   XX|  |  |  |  |  |  |XX
   *     ---------------------
   * 4 XX|  |  |  |  |  |  |XX
   *     ---------------------
   * 6   XX|  |  |  |  |  |  |XX
   *     ---------------------
   * 8 XX|  |  |  |  |  |  |XX
   *     -------------------
   *
   * \ingroup modimagedomainmorpher
   */
  template <typename I>
  struct hexa :
    public internal::image_domain_morpher< I, box2d_h, hexa<I> >
  {
    /// Skeleton.
    typedef hexa< tag::image_<I> > skeleton;

    /// Value associated type.
    typedef mln_value(I) value;

    /// Lvalue associated type.
    typedef mln_lvalue(I) lvalue;

    /// Return type of read-only access.
    typedef mln_rvalue(I) rvalue;

    /// Point site type
    typedef point2d_h psite;

    /// FIXME : should it be in box2d_h?
    /// Forward Site_Iterator associated type.
    typedef hexa_fwd_piter_<box2d> fwd_piter;

    /// FIXME : should it be in box2d_h?
    /// Backward Site_Iterator associated type.
    typedef hexa_bkd_piter_<box2d> bkd_piter;


    /// Constructor without argument.
    hexa();

    /// Constructor with an base image.
    hexa(I& ima);

    /// \cond INTERNAL_API
    /// Initialization.
    void init_(I& ima);
    /// \endcond

    /// Give the definition domain.
    const box2d_h& domain() const;

    /// Test if \p p belongs to the image domain.
    bool has(const psite& p) const;

    /// Read-only access of pixel value at hexa point site \p p.
    rvalue operator()(const point2d_h& p) const;

    /// Read-write access of pixel value at hexa point site \p p.
    lvalue operator()(const point2d_h& p);
  };


  /// \cond INTERNAL_API
  template <typename I, typename J>
  void init_(tag::image_t, hexa<I>& target, const J& model);
  /// \endcond

# ifndef MLN_INCLUDE_ONLY


  // init_

  template <typename I, typename J>
  inline
  void init_(tag::image_t, hexa<I>& target, const J& model)
  {
    I ima;
    init_(tag::image, ima, model);
    target.init_(ima);
  }



  // internal::data< hexa<I> >

  namespace internal
  {

    template <typename I>
    inline
    data< hexa<I> >::data(I& ima, box2d_h b)
      : ima_(ima),
	b_(b)
    {
    }

  } // end of namespace mln::internal


  template <typename I>
  inline
  void
  hexa<I>::init_(I& ima)
  {
    mln_precondition(! this->is_valid());
    box2d b_in = ima.bbox();
    box2d_h b = make::box2d_h(b_in.pmin()[0] * 2, b_in.pmin()[1],
			      b_in.pmax()[0] * 2, (b_in.pmax()[1] + 1) * 2 - 1);
    this->data_ = new internal::data< hexa<I> >(ima, b);
  }


  template <typename I>
  inline
  hexa<I>::hexa()
  {
  }


  template <typename I>
  inline
  hexa<I>::hexa(I& ima)
  {
    this->init_(ima);
  }

  template <typename I>
  inline
  typename hexa<I>::rvalue
  hexa<I>::operator()(const point2d_h& p) const
  {
    mln_precondition(this->is_valid());
    mln_precondition(this->has(p));
    return this->data_->ima_(point2d(p[0] / 2, p[1] / 2));
  }

  template <typename I>
  inline
  typename hexa<I>::lvalue
  hexa<I>::operator()(const point2d_h& p)
  {
    mln_precondition(this->is_valid());
    mln_precondition(this->has(p));
    return this->data_->ima_(point2d(p[0] / 2, p[1] / 2));
  }

  template <typename I>
  inline
  const box2d_h&
  hexa<I>::domain() const
  {
    mln_precondition(this->is_valid());
    return this->data_->b_;
  }

  template <typename I>
  inline
  bool
  hexa<I>::has(const psite& p) const
  {
    mln_precondition(this->is_valid());
    return this->data_->ima_.has(point2d(p[0] / 2, p[1] / 2));
  }


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_DMORPH_HEXA_HH
