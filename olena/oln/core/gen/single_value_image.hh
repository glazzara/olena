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

#ifndef OLN_CORE_GEN_SINGLE_VALUE_IMAGE_HH
# define OLN_CORE_GEN_SINGLE_VALUE_IMAGE_HH

# include <oln/core/internal/image_base.hh>
# include <oln/core/internal/utils.hh>


namespace oln
{


  // Fwd decl.
  template <typename Ps, typename T> class single_value_image;


  /// Virtual types.
  template <typename Ps, typename T>
  struct vtypes< single_value_image<Ps, T> >
  {
    typedef typename Ps::point point;
    typedef point              psite;
    typedef Ps                 pset;

    typedef typename point::coord coord;

    typedef T                  value;
    typedef const T&           rvalue;

    typedef internal::pair<Ps, T> data;

    // FIXME: To be defined...
    typedef mlc::none qiter;
    typedef mlc::none fwd_qiter;
    typedef mlc::none bkd_qiter;
  };


  /// Super type.
  template <typename Ps, typename T>
  struct super_trait_< single_value_image<Ps, T> >
  {
    typedef single_value_image<Ps, T> current;
    typedef internal::primitive_image_<current> ret;
  };


  /// Class for images defined by a point set and a single value.

  template <typename Ps, typename T>
  class single_value_image : public internal::image_base_< single_value_image<Ps, T> >
  {
    typedef single_value_image<Ps, T>      current;
    typedef internal::image_base_<current> super;
  public:


    stc_using(point);
    stc_using(rvalue);
    stc_using(data);

    single_value_image();

    single_value_image(const Ps& ps, const T& val);

    bool impl_owns_(const point& p) const;
    bool impl_has  (const point& p) const;

    rvalue impl_read(const point&) const;

    const Ps& impl_points() const;

    void change_value(const T& new_value);

  }; // end of single_value_image<Ps, T>



# ifndef OLN_INCLUDE_ONLY

  template <typename Ps, typename T>
  single_value_image<Ps, T>::single_value_image()
  {
  }

  template <typename Ps, typename T>
  single_value_image<Ps, T>::single_value_image(const Ps& pts,
						const T& val)
  {
    this->data_ = new data(pts, val);
  }

  template <typename Ps, typename T>
  bool
  single_value_image<Ps, T>::impl_owns_(const typename single_value_image<Ps, T>::point& p) const
  {
    assert(this->has_data());
    return this->data_->value1.has(p);
  }

  template <typename Ps, typename T>
  bool
  single_value_image<Ps, T>::impl_has(const typename single_value_image<Ps, T>::point& p) const
  {
    assert(this->has_data());
    return this->data_->value1.has(p);
  }

  template <typename Ps, typename T>
  typename single_value_image<Ps, T>::rvalue
  single_value_image<Ps, T>::impl_read(const typename single_value_image<Ps, T>::point&) const
  {
    assert(this->has_data());
    return this->data_->value2;
  }

  template <typename Ps, typename T>
  const Ps&
  single_value_image<Ps, T>::impl_points() const
  {
    assert(this->has_data());
    return this->data_->value1;
  }

  template <typename Ps, typename T>
  void
  single_value_image<Ps, T>::change_value(const T& new_value)
  {
    assert(this->has_data());
    this->data_->value2 = new_value;
  }

# endif

} // end of namespace oln


#endif // ! OLN_CORE_GEN_SINGLE_VALUE_IMAGE_HH
