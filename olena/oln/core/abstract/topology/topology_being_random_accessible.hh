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

#ifndef OLN_CORE_ABSTRACT_TOPOLOGY_TOPOLOGY_BEING_RANDOM_ACCESSIBLE_HH
# define OLN_CORE_ABSTRACT_TOPOLOGY_TOPOLOGY_BEING_RANDOM_ACCESSIBLE_HH

# include <oln/core/abstract/topology.hh>
# include <oln/core/abstract/topology/hierarchies.hh>
# include <oln/core/automatic/topology_being_random_accessible.hh>


namespace oln
{

  namespace abstract
  {

    template <typename E>
    class topology_being_random_accessible
      : public virtual topology<E>,
	public automatic::get_impl<topology_being_random_accessible, E>
    {
      typedef oln_type_of(E, point) point_t;

    public:

      bool has(const point_t& p) const;

      bool has_large(const point_t& p) const;

    protected:

      topology_being_random_accessible();
    };



# ifndef OLN_INCLUDE_ONLY

    template <typename E>
    bool topology_being_random_accessible<E>::has(const oln_type_of(E, point)& p) const
    {
      return this->exact().impl_has(p);
    }

    template <typename E>
    bool topology_being_random_accessible<E>::has_large(const oln_type_of(E, point)& p) const
    {
      return this->exact().impl_has_large(p);
    }

    template <typename E>
    topology_being_random_accessible<E>::topology_being_random_accessible()
    {
    }

# endif


  } // end of namespace oln::abstract


  template <typename E>
  struct case_ < topology_hierarchy_wrt_accessibility, E, 1 >
    : where_< mlc::eq_< oln_type_of(E, is_random_accessible), mlc::true_ > >
  {
    typedef abstract::topology_being_random_accessible<E> ret;
  };


} // end of namespace oln


#endif // ! OLN_CORE_ABSTRACT_TOPOLOGY_TOPOLOGY_BEING_RANDOM_ACCESSIBLE_HH
