// Copyright (C) 2009 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_FUN_META_SECOND_HH
# define MLN_FUN_META_SECOND_HH

/// \file mln/fun/meta/second.hh
///
/// Meta function to retrieve/modify the second component.

# include <mln/core/concept/meta_fun.hh>
# include <mln/util/couple.hh>

namespace mln
{

  namespace meta
  {

    template <class T>
    struct second : impl< second<T> >
    {
      typedef T value;
    };

  } // end of namespace mln::meta


  template <typename T, typename U>
  struct function< meta::second< util::couple<T,U> > >
    : public Function_v2v<function< meta::second < util::couple<T,U> > > >
  {
    typedef util::couple<T,U> value;

    typedef T result;
    result read(const value& c);

    typedef result& lresult;
    lresult write(value& c);
  };


# ifndef MLN_INCLUDE_ONLY


  template <typename T, typename U>
  inline
  typename function< meta::second< util::couple<T,U> > >::result
  function< meta::second< util::couple<T,U> > >::read(const value& c)
  {
    return c.second();
  }

  template <typename T, typename U>
  inline
  typename function< meta::second< util::couple<T,U> > >::lresult
  function< meta::second< util::couple<T,U> > >::write(value& c)
  {
    return c.second();
  }


# endif // ! MLN_INCLUDE_ONLY


} // end of namespace mln

#endif // MLN_FUN_META_SECOND_HH
