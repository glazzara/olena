// Copyright (C) 2007
//  EPITA Research and Development Laboratory
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

#ifndef OLN_CORE_2D_FAST_ITERATOR_3D_HH
# define OLN_CORE_2D_FAST_ITERATOR_3D_HH

# include <cassert>
# include <oln/core/concept/fast_iterator.hh>

namespace oln
{

  // Fwd decl.
  template <typename T> class image3d;
  template <typename T, typename C> class fast_iterator_3d;

  // Super type.
  template <typename T, typename C>
  struct super_trait_< fast_iterator_3d<T, C> >
  {
    typedef fast_iterator_3d<T, C> current;
    typedef Fast_Iterator<current> ret;
  };

  // Virtual types.
  template <typename T, typename C>
  struct vtypes< fast_iterator_3d<T, C> >
  {
    typedef T value;
  };

  // fast iterator for image3d
  template <typename T, typename C>
  class fast_iterator_3d : public Fast_Iterator< fast_iterator_3d<T, C> >
  {
    typedef fast_iterator_3d<T, C> current;
    typedef Fast_Iterator<current> super;
  public:
    stc_using(value);
    fast_iterator_3d(image3d<T>& ima);

    void impl_start();
    void impl_next();
    void impl_invalidate();
    bool impl_is_valid() const;

    value& impl_dereference();
    const value& impl_dereference() const;

  protected:
    T* start_;            // buffer start
    T* current_elt_;
    T* eoi_;              // buffer end;
  };

# ifndef OLN_INCLUDE_ONLY

  // initialize the fields start_ and eoi_, to image buffer start and end
  template <typename T, typename C>
  fast_iterator_3d<T, C>::fast_iterator_3d(image3d<T>& ima)
  {
    start_ = ima.img_array().buffer() +
      ima.img_array().imin() * ima.img_array().jmin() * ima.img_array().kmin() +
      ima.img_array().jmin() * ima.img_array().kmin() +
      ima.img_array().kmin();

    current_elt_ = start_;

    eoi_ = ima.img_array().buffer() +
      (ima.img_array().imax() - ima.img_array().imin() + 1)  *
      (ima.img_array().jmax() - ima.img_array().imin() + 1) *
      (ima.img_array().kmax() - ima.img_array().kmin() + 1);
  }

  template <typename T, typename C>
  void
  fast_iterator_3d<T, C>::impl_start()
  {
    current_elt_ = start_;
  }

  template <typename T, typename C>
  void
  fast_iterator_3d<T, C>::impl_next()
  {
    assert(this->impl_is_valid());
    ++current_elt_;
  }

  template <typename T, typename C>
  void
  fast_iterator_3d<T, C>::impl_invalidate()
  {
    current_elt_ = eoi_;
  }

  template <typename T, typename C>
  bool
  fast_iterator_3d<T, C>::impl_is_valid() const
  {
    return current_elt_ != eoi_;
  }

  template <typename T, typename C>
  typename fast_iterator_3d<T, C>::value&
  fast_iterator_3d<T, C>::impl_dereference()
  {
    assert(this->impl_is_valid());
    return *current_elt_;
  }

  template <typename T, typename C>
  const typename  fast_iterator_3d<T, C>::value&
  fast_iterator_3d<T, C>::impl_dereference() const
  {
    assert(this->impl_is_valid());
    return *current_elt_;
  }

# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln


#endif // !OLN_CORE_3D_FAST_ITERATOR_3D_HH
