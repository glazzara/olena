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

#ifndef OLENA_CORE_ABSTRACT_POINT_SET_BEING_CONNECTED_HH
# define OLENA_CORE_ABSTRACT_POINT_SET_BEING_CONNECTED_HH

# include <oln/core/abstract/point_set.hh>


namespace oln
{

  namespace abstract
  {


    template <typename E>
    class point_set_being_2d_connected : public virtual abstract::point_set<E>
    {
    public:

      unsigned nrows() const
      {
	precondition(this->is_valid());
	return this->exact().len(0);
      }

      unsigned ncols() const
      {
	precondition(this->is_valid());
	return this->exact().len(1);
      }

    protected:

      point_set_being_2d_connected()
      {}
    };


  } // end of namespace oln::abstract



  template <typename E>
  struct case_ < point_set_hierarchy_wrt_connectivity, E, 1 >
    : where_< mlc::and_list_< mlc::neq_< oln_type_of(E, bbox), mlc::none >,
			      mlc::eq_< oln_type_of(E, is_connected), mlc::true_ >,
			      mlc::eq_< oln_type_of(E, grid), grid2d > > >
  {
    typedef abstract::point_set_being_2d_connected<E> ret;
  };


} // end of namespace oln


#endif // ! OLENA_CORE_ABSTRACT_POINT_SET_BEING_CONNECTED_HH
