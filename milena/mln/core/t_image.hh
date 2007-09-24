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

#ifndef MLN_CORE_T_IMAGE_HH
# define MLN_CORE_T_IMAGE_HH

/*! \file mln/core/t_image.hh
 *
 * \brief Definition of the "transposed" image class mln::t_image.
 */

# include <mln/core/internal/image_adaptor.hh>


namespace mln
{


  /*! \brief Transposed image class.
   *
   * Swap a couple of coordinates.
   *
   * \warning This class only works on images whose domain is a box.
   */
  template <typename I>
  struct t_image : public internal::image_adaptor_< I, t_image<I> >
  {
    /// Skeleton.
    typedef t_image< tag::image_<I> > skeleton;

    /// Test if a pixel value is accessible at \p p.
    bool owns_(const mln_point(I)& p) const;

    /// Give the definition domain.
    const box_<mln_point(I)>& domain() const;

    /// Read-only access of pixel value at point site \p p.
    mln_rvalue(I) operator()(const mln_point(I)& p) const;

    /// Type returned by the read-write pixel value operator.
    typedef typename internal::morpher_lvalue_<I>::ret lvalue;

    /// Read-write access of pixel value at point site \p p.
    lvalue operator()(const mln_point(I)& p);

    /// Constructor.
    t_image(I& ima, unsigned c1, unsigned c2);

  protected:

    typedef internal::image_adaptor_< I, t_image<I> > super_;
    const unsigned c1_, c2_; // coords to swap
    box_<mln_point(I)> b_;

    mln_point(I) transpose_(const mln_point(I)& p) const;
  };


  template <typename I>
  t_image<I> swap_coords(Image<I>& ima, unsigned c1, unsigned c2);

  template <typename I>
  t_image<const I> swap_coords(const Image<I>& ima, unsigned c1, unsigned c2);



# ifndef MLN_INCLUDE_ONLY

  template <typename I>
  t_image<I> swap_coords(Image<I>& ima, unsigned c1, unsigned c2)
  {
    typedef mln_point(I) P;
    mln_precondition(c1 != c2);
    mln_precondition(c1 <= P::dim && c2 <= P::dim);
    t_image<I> tmp(exact(ima), c1, c2);
    return tmp;
  }

  template <typename I>
  t_image<const I> swap_coords(const Image<I>& ima, unsigned c1, unsigned c2)
  {
    typedef mln_point(I) P;
    mln_precondition(c1 != c2);
    mln_precondition(c1 <= P::dim && c2 <= P::dim);
    t_image<const I> tmp(exact(ima), c1, c2);
    return tmp;
  }

  template <typename I>
  t_image<I>::t_image(I& ima, unsigned c1, unsigned c2)
    : super_(ima),
      c1_(c1),
      c2_(c2)
  {
    mln_precondition(ima.has_data());
    b_.pmin() = transpose_(this->adaptee_.bbox().pmin());
    b_.pmax() = transpose_(this->adaptee_.bbox().pmax());
  }

  template <typename I>
  mln_point(I)
  t_image<I>::transpose_(const mln_point(I)& p) const
  {
    mln_point(I) tmp(p);
    tmp[c1_] = p[c2_];
    tmp[c2_] = p[c1_];
    return tmp;
  }

  template <typename I>
  const box_<mln_point(I)>&
  t_image<I>::domain() const
  {
    mln_precondition(this->has_data());
    return b_;
  }

  template <typename I>
  bool
  t_image<I>::owns_(const mln_point(I)& p) const
  {
    mln_precondition(this->has_data());
    return this->adaptee_.owns_(transpose_(p));
  }

  template <typename I>
  mln_rvalue(I)
  t_image<I>::operator()(const mln_point(I)& p) const
  {
    mln_precondition(this->owns_(p));
    return this->adaptee_(transpose_(p));
  }

  template <typename I>
  typename internal::morpher_lvalue_<I>::ret
  t_image<I>::operator()(const mln_point(I)& p)
  {
    mln_precondition(this->owns_(p));
    return this->adaptee_(transpose_(p));
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_T_IMAGE_HH
