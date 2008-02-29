// Copyright (C) 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_SUB_SAMPLED_IMAGE_HH
# define MLN_CORE_SUB_SAMPLED_IMAGE_HH

/*! \file mln/core/sub_sampled_image.hh
 *
 */

# include <mln/core/internal/image_morpher.hh>
# include <mln/convert/to_dpoint.hh>
# include <mln/metal/vec.hh>
# include <mln/value/set.hh>


namespace mln
{

  // Fwd decl.
  template <typename I> struct sub_sampled_image;

  namespace internal
  {
    
    /// \internal Data structure for \c mln::t_image<I>.
    template <typename I>
    struct data_< sub_sampled_image<I> >
    {
      /// \brief Build the data object held by a sub_sampled_image.
      ///
      /// \param ima  The underlying image.
      /// \param box  The bounding box (domain) of the morphed image.
      data_(I& ima, mln::box_<mln_point(I)>& box);
      
      /// Underlying image.
      I ima_;
      
      /// The bounding box of the morphed image.
      mln::box_<mln_point(I)> box_;
    };

  } // end of namespace mln::internal

  template <typename I>
  class sub_sampled_image
    : public internal::image_morpher_< I, mln_pset(I), sub_sampled_image<I> >
  {
  public:
    /// Super type.
    typedef
    internal::image_morpher_< I, mln_pset(I), sub_sampled_image<I> > super_;
    
    /// Point_Site associated type.
    typedef mln_psite(I) psite;
    
    /// Value associated type.
    typedef mln_value(I) value;

    /// Type returned by the read-write pixel value operator.
    typedef typename internal::morpher_lvalue_<I>::ret lvalue;

    /// Return type of read-only access.
    typedef mln_rvalue(I) rvalue;

    /// Value set associated type.
    typedef mln::value::set<value> vset;

    /// Skeleton.
    typedef sub_sampled_image< tag::image_<I> > skeleton;


    /// Give the definition domain.
    const box_<mln_point(I)>& domain() const;

    /// Test if this image has been initialized.
    bool has_data() const;

    /// Test if a pixel value is accessible at \p p.
    bool owns_(const mln_point(I)& p) const;

    /// Give the set of values of the image.
    const vset& values() const;
    
    /// Read-only access of pixel value at point site \p p.
    mln_rvalue(I) operator()(const mln_point(I)& p) const;
    
    /// Read-write access of pixel value at point site \p p.
    lvalue operator()(const mln_point(I)& p);
  public:
    sub_sampled_image(I& ima, const mln_point(I)& first_p, int gap);

    void set_sampling(const mln_point(I)& first_p, int gap);

  protected:
    
    /// Compute physical coordinates.
    mln_point(I) translate_coords_(const mln_point(I)& p) const;
    
    const mln_point(I)& first_p;
    int gap;
  };

    
# ifndef MLN_INCLUDE_ONLY

  namespace internal
  {

    // internal::data_< sub_sampled_image<I,S> >

    template <typename I>
    inline
    data_< sub_sampled_image<I> >::data_(I& ima, mln::box_<mln_point(I)>& box)
      : ima_(ima), box_(box)
    {
    }

  } // end of namespace mln::internal
  
  
  template <typename I>
  sub_sampled_image<I>::sub_sampled_image(I& ima, const mln_point(I)& first_p, int gap)
    : first_p(first_p), gap(gap)
  {
    box_<mln_point(I)> box(ima.bbox().pmin(),ima.bbox().pmax());
    this->data_ = new internal::data_< sub_sampled_image<I> >(ima, box);
  }

  
  template <typename I>
  inline
  const box_<mln_point(I)>&
  sub_sampled_image<I>::domain() const
  {
    mln_precondition(this->has_data());
    return this->data_->box_;
  }

  
  template <typename I>
  inline
  const typename sub_sampled_image<I>::vset&
  sub_sampled_image<I>::values() const
  {
    mln_precondition(this->delegatee_() != 0);
    return this->delegatee_()->values();
  }

  
  template <typename I>
  inline
  bool sub_sampled_image<I>::has_data() const
  {
    mln_invariant(this->delegatee_()->has_data());
    return true;
  }

  
  template <typename I>
  inline
  bool
  sub_sampled_image<I>::owns_(const mln_point(I)& p) const
  {
    mln_precondition(this->has_data());
    return this->delegatee_()->owns_(translate_coords_(p));
  }
  
  template <typename I>
  inline
  mln_rvalue(I)
  sub_sampled_image<I>::operator()(const mln_point(I)& p) const
  {
    mln_precondition(this->owns_(p));
    return (*this->delegatee_())(translate_coords_(p));
  }
  
  template <typename I>
  inline
  typename internal::morpher_lvalue_<I>::ret
  sub_sampled_image<I>::operator()(const mln_point(I)& p)
  {
    mln_precondition(this->owns_(p));
    return (*this->delegatee_())(translate_coords_(p));
  }

  template <typename I>
  inline
  mln_point(I)
  sub_sampled_image<I>::translate_coords_(const mln_point(I)& p) const
  {
   
    return mln_point(I)(metal::vec<2, int>(p) * gap + metal::vec<2, int>(first_p));
  }
  
  
# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_SUB_SAMPLED_IMAGE_HH
