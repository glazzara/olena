// Copyright (C) 2001, 2003, 2004, 2005, 2006 EPITA Research and
// Development Laboratory
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

#ifndef OLN_CORE_ABSTRACT_ITERATOR_ON_POINTS_HH
# define OLN_CORE_ABSTRACT_ITERATOR_ON_POINTS_HH

# include <oln/core/abstract/iterator.hh>
# include <oln/core/abstract/point.hh>


namespace oln
{


  // Forward declaration.
  namespace abstract { template <typename E> class iterator_on_points; }


  // Super type declaration.
  template <typename E>
  struct set_super_type< abstract::iterator_on_points<E> >
  {
    typedef abstract::iterator<E> ret;
  };


  /// Virtual types associated to abstract::iterator_on_points<E>.
  template <typename E>
  struct vtypes< abstract::iterator_on_points<E> >
  {
    typedef mlc::undefined point_type;
  };

  template <typename E>
  struct single_vtype< abstract::iterator_on_points<E>, typedef_::coord_type >
  {
    typedef oln_type_of(E, point) point_t;
    typedef oln_type_of(point_t, coord) ret;
  };


  namespace abstract
  {

    /// Abstract point iterator class.
    template <typename E>
    class iterator_on_points : public abstract::iterator<E>
    {
      typedef oln_type_of(E, point) point_t;

    public:

      point_t to_point() const;

      const point_t* point_adr() const;

      // Concrete method.
      operator point_t() const;

    protected:

      iterator_on_points();

      ~iterator_on_points();

    }; // end of class oln::abstract::iterator_on_points<E>



# ifndef OLN_INCLUDE_ONLY

    template <typename E>
    typename iterator_on_points<E>::point_t
    iterator_on_points<E>::to_point() const
    {
      precondition(this->is_valid());
      return this->exact().impl_to_point();
    }

    template <typename E>
    const typename iterator_on_points<E>::point_t*
    iterator_on_points<E>::point_adr() const
    {
      return this->exact().impl_point_adr();
    }

    template <typename E>
    // Concrete method.
    iterator_on_points<E>::operator typename iterator_on_points<E>::point_t() const
    {
      precondition(this->is_valid());
      return this->to_point();
    }

    template <typename E>
    iterator_on_points<E>::iterator_on_points()
    {
    }

    template <typename E>
    iterator_on_points<E>::~iterator_on_points()
    {
      mlc::assert_defined_< typename iterator_on_points<E>::point_t >::check();
      mlc::assert_< mlc_is_a(typename iterator_on_points<E>::point_t, abstract::point) >::check();
    }


# endif

  } // end of namespace oln::abstract


} // end of namespace oln


#endif // ! OLN_CORE_ABSTRACT_ITERATOR_ON_POINTS_HH
