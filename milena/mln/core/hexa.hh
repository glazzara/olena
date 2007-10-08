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
 * \brief Definition of the mln::hexa class.
 */

# include <mln/core/internal/image_domain_morpher.hh>
# include <mln/core/point2d_h.hh>


namespace mln
{
  // Fwd decl.
  template <typename I> class hexa;


  namespace internal
  {

    template <typename I>
    struct data_< hexa<I> >
    {
      data_(I& ima);

      I ima_;
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
   *    -------------------
   *  XX|  |  |  |  |  |  |XX
   *    ---------------------
   *    XX|  |  |  |  |  |  |XX
   *    ---------------------
   *  XX|  |  |  |  |  |  |XX
   *    ---------------------
   *    XX|  |  |  |  |  |  |XX
   *    ---------------------
   *  XX|  |  |  |  |  |  |XX
   *    -------------------
   *
   *
   */
  template <typename I>
  struct hexa : public internal::image_domain_morpher_< I, mln_pset(I), hexa<I> >
  {
    /// Skeleton.
    typedef hexa< tag::image_<I> > skeleton;

    /// Value associated type.
    typedef mln_value(I) value;

    /// Lvalue associated type.
    typedef mln_value(I) lvalue;

    /// Return type of read-only access.
    typedef mln_rvalue(I) rvalue;

    /// Point site type
    typedef point2d_h psite;

    /// Constructor without argument.
    hexa();

    /// Constructor with an base image.
    hexa(I& ima);

    /// Initialization.
    void init_(I& ima);

    /// Give the definition domain.
    const mln_pset(I)& domain() const;

    /// Test if \p p belongs to the image domain.
    bool has(const psite& p) const;
    bool owns_(const psite& p) const;

    /// Read-only access of pixel value at hexa point site \p p.
    rvalue operator()(const point2d_h& p) const;

    /// Read-write access of pixel value at hexa point site \p p.
    value operator()(const point2d_h& p);
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
    data_< hexa<I> >::data_(I& ima)
      : ima_(ima)
    {
    }

  } // end of namespace mln::internal


  template <typename I>
  void
  hexa<I>::init_(I& ima)
  {
    mln_precondition(! this->has_data());
    this->data_ = new internal::data_< hexa<I> >(ima);
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
    return this->data_->ima_(make::point2d(p[0] / 2, p[1] / 2));
  }

  template <typename I>
  typename hexa<I>::value
  hexa<I>::operator()(const point2d_h& p)
  {
    return this->data_->ima_(make::point2d(p[0] / 2, p[1] / 2));
  }

  template <typename I>
  const mln_pset(I)&
  hexa<I>::domain() const
  {
    return this->data_->ima_.domain();
  }

  template <typename I>
  bool
  hexa<I>::has(const psite& p) const
  {
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
