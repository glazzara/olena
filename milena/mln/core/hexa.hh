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


#ifndef MLN_CORE_HEXA_HH
# define MLN_CORE_HEXA_HH


/*! \file mln/core/hexa.hh
 *
 * \brief Definition of a morpher that makes hexagonal the mesh of an
 * image.
 */

# include <mln/core/internal/image_domain_morpher.hh>
# include <mln/core/point2d_h.hh>
# include <mln/core/box2d_h.hh>
# include <mln/core/hexa_piter.hh>


namespace mln
{
  // Fwd decl.
  template <typename I> class hexa;


  namespace internal
  {

    template <typename I>
    struct data_< hexa<I> >
    {
      data_(I& ima, box2d_h b);

      I ima_;
      mln::box2d_h b_;
    };

  } // end of namespace mln::internal


  namespace trait
  {

    template <typename I>
    struct image_< hexa<I> > : default_image_morpher_< I, mln_value(I),
						       hexa<I> >
    {
      //     private:
      //       typedef mln_trait_image_data(I) I_data_;
      //       typedef mlc_equal(I_data_, trait::data::linear) I_data_are_linear_;
      //     public:

      typedef trait::image::category::domain_morpher category;


      typedef mln_trait_image_border(I) border;  // have a border only if I does.

      typedef mln_trait_image_io_from_(I) io; // un-write when I const

      typedef mln_trait_image_data_from_(I) data;

      typedef trait::image::support::regular support;
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
   *
   */
  template <typename I>
  struct hexa : public internal::image_domain_morpher_< I, box2d_h, hexa<I> >
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
    /// Forward Point_Iterator associated type.
    typedef hexa_fwd_piter_<box2d> fwd_piter;

    /// FIXME : should it be in box2d_h?
    /// Backward Point_Iterator associated type.
    typedef hexa_bkd_piter_<box2d> bkd_piter;


    /// Constructor without argument.
    hexa();

    /// Constructor with an base image.
    hexa(I& ima);

    /// Initialization.
    void init_(I& ima);

    /// Give the definition domain.
    const box2d_h& domain() const;

    /// Test if \p p belongs to the image domain.
    bool has(const psite& p) const;
    bool owns_(const psite& p) const;

    /// Read-only access of pixel value at hexa point site \p p.
    rvalue operator()(const point2d_h& p) const;

    /// Read-write access of pixel value at hexa point site \p p.
    lvalue operator()(const point2d_h& p);
  };

  template <typename I, typename J>
  void init_(tag::image_t, hexa<I>& target, const J& model);

# ifndef MLN_INCLUDE_ONLY


  // init_

  template <typename I, typename J>
  void init_(tag::image_t, hexa<I>& target, const J& model)
  {
    I ima;
    init_(tag::image, ima, model);
    target.init_(ima);
  }



  // internal::data_< hexa<I> >

  namespace internal
  {

    template <typename I>
    data_< hexa<I> >::data_(I& ima, box2d_h b)
      : ima_(ima),
	b_(b)
    {
    }

  } // end of namespace mln::internal


  template <typename I>
  void
  hexa<I>::init_(I& ima)
  {
    mln_precondition(! this->has_data());
    box2d b_in = ima.bbox();
    box2d_h b = make::box2d_h(b_in.pmin()[0] * 2, b_in.pmin()[1],
			      b_in.pmax()[0] * 2, (b_in.pmax()[1] + 1) * 2 - 1);
    this->data_ = new internal::data_< hexa<I> >(ima, b);
  }


  template <typename I>
  hexa<I>::hexa()
  {
  }


  template <typename I>
  hexa<I>::hexa(I& ima)
  {
    this->init_(ima);
  }

  template <typename I>
  typename hexa<I>::rvalue
  hexa<I>::operator()(const point2d_h& p) const
  {
    mln_precondition(this->has_data());
    mln_precondition(this->has(p));
    return this->data_->ima_(make::point2d(p[0] / 2, p[1] / 2));
  }

  template <typename I>
  typename hexa<I>::lvalue
  hexa<I>::operator()(const point2d_h& p)
  {
    mln_precondition(this->has_data());
    mln_precondition(this->has(p));
    return this->data_->ima_(make::point2d(p[0] / 2, p[1] / 2));
  }

  template <typename I>
  const box2d_h&
  hexa<I>::domain() const
  {
    mln_precondition(this->has_data());
    return this->data_->b_;
  }

  template <typename I>
  bool
  hexa<I>::has(const psite& p) const
  {
    mln_precondition(this->has_data());
    return this->data_->ima_.has(make::point2d(p[0] / 2, p[1] / 2));
  }


  template <typename I>
  bool
  hexa<I>::owns_(const psite& p) const
  {
    return this->has(p);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_HEXA_HH
