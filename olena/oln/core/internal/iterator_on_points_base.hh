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

#ifndef OLN_CORE_INTERNAL_ITERATOR_ON_POINTS_BASE_HH
# define OLN_CORE_INTERNAL_ITERATOR_ON_POINTS_BASE_HH

# include <oln/core/concept/iterator_on_points.hh>
# include <oln/core/internal/iterator_on_points_impl.hh>


namespace oln
{

  // Fwd decl
  namespace internal { template <typename Exact> struct iterator_on_points_base_; }

  // Super type
  template <typename Exact>
  struct super_trait_< internal::iterator_on_points_base_<Exact> >
  {
    typedef Iterator_on_Points<Exact> ret;
  };

  // Virtual types
  template <typename Exact>
  struct vtypes< internal::iterator_on_points_base_<Exact> >
  {
    typedef stc::final< stc::is<Iterator_on_Points> > category;

    typedef stc::abstract point;

    typedef stc_deferred(point) point__;
    typedef stc::final< oln_grid(point__) >   grid;
    typedef stc::final< oln_coord(point__) >  coord;
    typedef stc::final< oln_dim(point__) >    dim;
    typedef stc::final< oln_dpoint(point__) > dpoint;
  };


  namespace internal
  {

    template <typename Exact>
    class iterator_on_points_base_
      : public Iterator_on_Points<Exact>,
 	public internal::iterator_on_points_impl_<mlc_value(stc_deferred(dim)), Exact>
    {
    protected:
      iterator_on_points_base_();
    };


# ifndef OLN_INCLUDE_ONLY

    template <typename Exact>
    iterator_on_points_base_<Exact>::iterator_on_points_base_()
    {
    }

# endif // ! OLN_INCLUDE_ONLY

  } // end of namespace oln::internal

} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_ITERATOR_ON_POINTS_BASE_HH
