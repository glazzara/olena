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

#ifndef OLN_CORE_ABSTRACT_ITERATOR_ON_VALUES_HH
# define OLN_CORE_ABSTRACT_ITERATOR_ON_VALUES_HH

# include <oln/core/abstract/iterator.hh>


namespace oln
{


  // Forward declaration.
  namespace abstract { template <typename E> class iterator_on_values; }


  // Super type declaration.
  template <typename E>
  struct set_super_type< abstract::iterator_on_values<E> >
  {
    typedef abstract::iterator<E> ret;
  };


  /// Virtual types associated to abstract::iterator_on_values<E>.
  template <typename E>
  struct vtypes< abstract::iterator_on_values<E> >
  {
    typedef stc::abstract value_type;
  };


  namespace abstract
  {

    /// Abstract value iterator class.
    template <typename E>
    class iterator_on_values : public abstract::iterator<E>
    {
    public:
      typedef oln_vtype(E, value) value_type;

      value_type to_value() const;

      // Concrete method.
      operator value_type() const;

    protected:

      iterator_on_values();

      ~iterator_on_values();

    }; // end of class oln::abstract::iterator_on_values<E>


    template <typename E>
    std::ostream& operator<<(std::ostream& ostr,
			     const iterator_on_values<E>& vit);


# ifndef OLN_INCLUDE_ONLY

    template <typename E>
    typename iterator_on_values<E>::value_type
    iterator_on_values<E>::to_value() const
    {
      precondition(this->is_valid());
      return this->exact().impl_to_value();
    }

    template <typename E>
    // Concrete method.
    iterator_on_values<E>::operator typename iterator_on_values<E>::value_type() const
    {
      precondition(this->is_valid());
      return this->to_value();
    }

    template <typename E>
    iterator_on_values<E>::iterator_on_values()
    {
    }

    template <typename E>
    iterator_on_values<E>::~iterator_on_values()
    {
      mlc::assert_defined_< typename iterator_on_values<E>::value_type >::check();
    }


    template <typename E>
    std::ostream& operator<<(std::ostream& ostr,
			     const iterator_on_values<E>& vit)
    {
      return ostr << vit.to_value();
    }


# endif

  } // end of namespace oln::abstract


} // end of namespace oln


#endif // ! OLN_CORE_ABSTRACT_ITERATOR_ON_VALUES_HH
