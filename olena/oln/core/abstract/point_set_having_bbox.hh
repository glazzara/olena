// Copyright (C) 2006 EPITA Research and Development Laboratory
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

#ifndef OLN_CORE_ABSTRACT_POINT_SET_HAVING_BBOX_HH
# define OLN_CORE_ABSTRACT_POINT_SET_HAVING_BBOX_HH

# include <oln/core/abstract/point_set.hh>


namespace oln
{

  namespace abstract
  {


    template <typename E>
    class point_set_having_bbox : public virtual point_set<E>
    {
      typedef oln_type_of(E, point) point_t;
      typedef oln_type_of(E, bbox)  bbox_t;

      typedef oln_type_of(point_t, coord) coord_t;
      typedef oln_type_of(point_t, dim) dim_t;
      enum { n = mlc_value(dim_t) };
      
    public:

      const bbox_t& bbox() const;

      const point_t& pmin() const;

      coord_t pmin(unsigned i) const;

      const point_t& pmax() const;

      coord_t pmax(unsigned i) const;

      unsigned len(unsigned i) const;

    protected:

      point_set_having_bbox();

      point_t pmin_, pmax_;
    };



# ifndef OLN_INCLUDE_ONLY

    template <typename E>
    const typename point_set_having_bbox<E>::bbox_t&
    point_set_having_bbox<E>::bbox() const
    {
      return this->exact().impl_box();
    }

    template <typename E>
    const typename point_set_having_bbox<E>::point_t&
    point_set_having_bbox<E>::pmin() const
    {
      precondition(this->is_valid());
      return pmin_;
    }

    template <typename E>
    typename point_set_having_bbox<E>::coord_t
    point_set_having_bbox<E>::pmin(unsigned i) const
    {
      precondition(this->is_valid() and i < n);
      return pmin_[i];
    }

    template <typename E>
    const typename point_set_having_bbox<E>::point_t&
    point_set_having_bbox<E>::pmax() const
    {
      precondition(this->is_valid());
      return pmax_;
    }

    template <typename E>
    typename point_set_having_bbox<E>::coord_t
    point_set_having_bbox<E>::pmax(unsigned i) const
    {
      precondition(this->is_valid() and i < n);
      return pmax_[i];
    }

    template <typename E>
    unsigned
    point_set_having_bbox<E>::len(unsigned i) const
    {
      precondition(this->is_valid() and i < n);
      return pmax_[i] - pmin_[i] + 1;
    }

    template <typename E>
    point_set_having_bbox<E>::point_set_having_bbox()
    {}

# endif

  } // end of namespace oln::abstract


  // an abstract sub-class of point_set_having_bbox<E> is abstract::bbox<E>;
  // the switch-case mechanism is in oln/core/abstract/bbox.hh


} // end of namespace oln


#endif // ! OLN_CORE_ABSTRACT_POINT_SET_HAVING_BBOX_HH
