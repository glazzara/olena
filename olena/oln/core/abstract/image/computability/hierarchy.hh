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

#ifndef OLN_CORE_ABSTRACT_IMAGE_COMPUTABILITY_HIERARCHY_HH
# define OLN_CORE_ABSTRACT_IMAGE_COMPUTABILITY_HIERARCHY_HH

# include <oln/core/abstract/image.hh>
# include <oln/core/abstract/image/hierarchies.hh>
// # include <oln/core/automatic/image/computed_image.hh>
// # include <oln/core/automatic/image/plain_image.hh>



namespace oln
{
 
  /*--------------.
  | Abstraction.  |
  `---------------*/

  namespace abstract
  {

    /// Image whose data are computed.
    template <typename E>
    struct computed_image :
      public virtual image<E> //, public automatic::get_impl< computed_image, E>
    {
    public:

      oln_plain(E) plain() const;

    protected:

      /// Constructor (protected, empty).
      computed_image();
    };


    /// Image whose data are *not* computed so they are *plain*.
    template <typename E>
    struct plain_image :
      public virtual image<E> //, public automatic::get_impl< plain_image, E>
    {
    protected:

      /// Constructor (protected, empty).
      plain_image();
    };


# ifndef OLN_INCLUDE_ONLY

    template <typename E>
    computed_image<E>::computed_image()
    {
    }

    template <typename E>
    oln_plain(E)
    computed_image<E>::plain() const
    {
      oln_plain(E) tmp(this->topo());
      return tmp;
    }

    template <typename E>
    plain_image<E>::plain_image()
    {
    }

# endif

  } // end of namespace oln::abstract


  /*--------------------.
  | Computability switch.  |
  `--------------------*/

  /// With computability.
  template <typename E>
  struct case_< image_hierarchy_wrt_computability, E, 1 > :
    where_< mlc::eq_< oln_type_of(E, is_computed), mlc::true_ > >
  {
    typedef abstract::computed_image<E> ret;
  };

  template <typename E>
  struct case_< image_hierarchy_wrt_computability, E, 2 > :
    where_< mlc::eq_< oln_type_of(E, is_computed), mlc::false_ > >
  {
    typedef abstract::plain_image<E> ret;
  };

} // end of namespace oln


#endif // ! OLN_CORE_ABSTRACT_IMAGE_COMPUTABILITY_HIERARCHY_HH
