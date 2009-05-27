// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_HISTO_POINT_FROM_VALUE_HH
# define MLN_HISTO_POINT_FROM_VALUE_HH

/// \file mln/histo/point_from_value.hh
///
/// Function 'point from value'.

# include <mln/core/grids.hh>
# include <mln/core/point.hh>
# include <mln/fun/v2i/index_of_value.hh>
# include <mln/fun/v2v/component.hh>
# include <mln/trait/value/comp.hh>


# define mln_histo_point_type_from(T) typename mln::histo::point_type_from<T>::ret


namespace mln
{

  namespace histo
  {


    template <typename T>
    struct point_type_from
    {
    private:
      enum { d = mln_dim(T) };
      typedef mln_regular_grid_from_dim(d) G;

    public:
      typedef mln::point<G, def::coord> ret;
    };


    template <typename T>
    struct point_from_value : Function_v2v< point_from_value<T> >
    {
      typedef mln_histo_point_type_from(T) result;
      result operator()(const T& v) const;
    };

    template <typename T>
    mln_histo_point_type_from(T)
    meta_point_from_value(const T& v);


# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {

      template <unsigned d>
      struct point_from_value;

      template <>
      struct point_from_value< 1 >
      {
	template <typename T, typename P>
	static void run(const T& v, P& p)
	{
	  p[0] = fun::v2i::meta_index_of_value(v);
	}
      };

      template <>
      struct point_from_value< 2 >
      {
	template <typename T, typename P>
	static void run(const T& v, P& p)
	{
	  p[0] = fun::v2i::meta_index_of_value( fun::v2v::component<T,0>()(v) );
	  p[1] = fun::v2i::meta_index_of_value( fun::v2v::component<T,1>()(v) );
	}
      };

      template <>
      struct point_from_value< 3 >
      {
	template <typename T, typename P>
	static void run(const T& v, P& p)
	{
	  p[0] = fun::v2i::meta_index_of_value( fun::v2v::component<T,0>()(v) );
	  p[1] = fun::v2i::meta_index_of_value( fun::v2v::component<T,1>()(v) );
	  p[2] = fun::v2i::meta_index_of_value( fun::v2v::component<T,2>()(v) );
	}
      };

    } // end of namespace mln::histo::internal


    template <typename T>
    inline
    typename point_from_value<T>::result
    point_from_value<T>::operator()(const T& v) const
    {
      mln_histo_point_type_from(T) p;
      internal::point_from_value<mln_dim(T)>::run(v, p);
      return p;
    }

    template <typename T>
    mln_histo_point_type_from(T)
    meta_point_from_value(const T& v)
    {
      point_from_value<T> f;
      return f(v);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::histo

} // end of namespace mln


#endif // ! MLN_HISTO_POINT_FROM_VALUE_HH
