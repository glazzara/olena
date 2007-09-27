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

#ifndef MLN_CORE_MESH_IMAGE_HH
# define MLN_CORE_MESH_IMAGE_HH

/*! \file mln/core/mesh_image.hh
 *
 * \brief Definition of an image class FIXME
 */

# include <cmath>

# include <mln/core/internal/image_identity.hh>
# include <mln/metal/vec.hh>
# include "mesh_p.hh"
# include "mesh_psite.hh"
# include <vector>

namespace mln
{

  // Fwd decl.
  template <typename P, typename V> struct mesh_image;

  namespace internal
  {

    template <typename P, typename V>
    struct data_< mesh_image<P, V> >
    {
      data_(mesh_p<P>& ima, std::vector<V>& val_);

      mesh_p<P>		mesh_;
      std::vector<V>	val_;
    };

  } // end of namespace mln::internal

  /*! \brief FIXME
   *
   */
  template <typename P, typename V>
  struct mesh_image  : public internal::image_primary_< box2d, mesh_image<P, V> >
  {

    //    typedef mln::internal::image_primary_< box2d, mesh_image<P, V> > super_;

    /// Point_Site associated type.
    typedef P psite;

    /// Value associated type.
    typedef V value;

    /// Return type of read-write access.
    typedef V& lvalue; // FIXME: Depends on lvalue presence in I.

    /// Return type of read-only access.
    typedef V rvalue;

    /// Value set associated type.
    typedef mln::value::set<value> vset;


    /// Skeleton.
    typedef mesh_image< tag::psite_<P>, tag::value_<V> > skeleton;

    /// Constructors.
    mesh_image(mesh_p<P>& mesh, std::vector<V>& val);
    mesh_image();


    /// Test if this image has been initialized.
    bool has_data() const;

//     /// Test if a pixel value is accessible at \p p
//     using super_::owns_;

    /// Test if a pixel value is accessible at \p v.
    bool owns_(const mln::metal::vec<P::dim, float>& v) const;

//     /// Read-only access of pixel value at point site \p p.
//     /// Mutable access is only OK for reading (not writing).
//     using super_::operator();

    V& operator()(const mln::metal::vec<P::dim, float>& v) const;

//     /// Give the set of values of the image.
//     const vset& values() const;
};



# ifndef MLN_INCLUDE_ONLY

  namespace internal
  {

    template <typename P, typename V>
    data_< mesh_image<P, V> >::data_(mesh_p<P>& mesh, std::vector<V>& val)
      : val_ (val),
	mesh_ (mesh)
    {
    }

  } // end of namespace mln::internal

  template <typename P, typename V>
  mesh_image<P, V>::mesh_image(mesh_p<P>& mesh, std::vector<V>& val)
  {
    //    mln_precondition(ima.has_data());
    this->data_ = new internal::data_< mesh_image<P, V> > (mesh, val);
  }

  template <typename P, typename V>
  mesh_image<P, V>::mesh_image()
  {
  }

  template <typename P, typename V>
  bool mesh_image<P, V>::has_data() const
  {
    mln_invariant(this->data_->val_.size() != 0);
    return true;
  }

  template <typename P, typename V>
  bool mesh_image<P, V>::owns_(const mln::metal::vec<P::dim, float>& v) const
  {
    for (unsigned i = 0; i < this->data_->val_.size(); ++i)
      if (this->data_->mesh_.loc_[i] == v)
	return true;
    return false;
  }

  template <typename P, typename V>
  V&
  mesh_image<P, V>::operator()(const mln::metal::vec<P::dim, float>& v) const
  {
    unsigned i = 0;
//     for (i = 0; i < this->data_->val_.size(); ++i)
//       if (this->data_->mesh_.loc_[i] == v)
// 	break;
    return this->data_->mesh_.gr.loc_[i];
  }

//   // FIXME : Should we remove this method? (and inherit it from
//   // identity morpher)
//   template <typename P, typename V>
//   const vset &
//   mesh_image<P, V>::values() const
//   {
//     return vset::the();
//   }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_MESH_IMAGE_HH
