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
 * \brief Definition of an image class mesh_image.
 */

# include <mln/core/internal/image_identity.hh>
# include <mln/metal/vec.hh>
# include <mln/core/mesh_p.hh>
# include <mln/core/mesh_psite.hh>
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
      data_(mesh_p<P>& mesh, std::vector<V>& val);

      std::vector<V>	val_;
      mesh_p<P>		mesh_;
    };

  } // end of namespace mln::internal

  /*! \brief FIXME
   *
   */
  template <typename P, typename V>
  struct mesh_image  : public internal::image_primary_< mesh_p<P>, mesh_image<P, V> >
  {

    typedef mln::internal::image_base_< mesh_p<P>, mesh_image<P, V> > super_;

    /// Value associated type.
    typedef V value;

    /// Return type of read-write access.
    typedef V& lvalue;

    /// Return type of read-only access.
    typedef const V& rvalue;

    /// Value set associated type.
    typedef mln::value::set<value> vset;


    /// Skeleton.
    typedef mesh_image< tag::psite_<P>, tag::value_<V> > skeleton;

    /// Constructors.
    mesh_image(mesh_p<P>& mesh, std::vector<V>& val);
    mesh_image();

    /// Read-only access of pixel value at point site \p p.
    const V& operator()(const mesh_psite<P>& p) const;
    
    /// Read-write access of pixel value at point site \p p.
    V& operator()(const mesh_psite<P>& p);

    /// Give the set of values of the image.
    const vset& values() const;

    const std::vector<V>& data_values () const;

    const mesh_p<P>& domain() const;
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
    this->data_ = new internal::data_< mesh_image<P, V> > (mesh, val);
  }

  template <typename P, typename V>
  mesh_image<P, V>::mesh_image()
  {
  }

  template <typename P, typename V>
  const V&
  mesh_image<P, V>::operator()(const mesh_psite<P>& p) const
  {
    mln_precondition(p.m_ptr_ == & this->data_->mesh_);
    mln_precondition(p.i_ < this->data_->val_.size());
    return this->data_->val_[p.i_];
  }
    
  template <typename P, typename V>
  V&
  mesh_image<P, V>::operator()(const mesh_psite<P>& p)
  {
    mln_precondition(p.m_ptr_ == & this->data_->mesh_);
    mln_precondition(p.i_ < this->data_->val_.size());
    return this->data_->val_[p.i_];
  }

  template <typename P, typename V>
  const mln::value::set<V> &
  mesh_image<P, V>::values() const
  {
    return vset::the();
  }

  template <typename P, typename V>
  const std::vector<V>&
  mesh_image<P, V>::data_values () const
  {
    return this->data_->val_;
  }

  template <typename P, typename V>
  const mesh_p<P>&
  mesh_image<P, V>::domain() const
  {
    mln_precondition(this->has_data());
    return this->data_->mesh_;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_MESH_IMAGE_HH
