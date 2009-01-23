// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_FUN_META_RED_HH
# define MLN_FUN_META_RED_HH

/// \file mln/fun/meta/red.hh
///
/// Meta function to retrieve/modify the red component.

# include <mln/core/concept/meta_fun.hh>
# include <mln/value/rgb.hh>

namespace mln
{

  namespace meta
  {

    template <class T>
    struct red : impl< red<T> >
    {
      typedef T value;
    };

  } // end of namespace mln::meta


  template <unsigned n>
  struct function< meta::red< value::rgb<n> > >
    : public Function_v2w_w2v<function< meta::red < value::rgb<n> > > >
  {
    typedef value::rgb<n> value;

    typedef typename value::red_t result;
    result read(const value& c);

    typedef result& lresult;
    lresult write(value& c);
  };


# ifndef MLN_INCLUDE_ONLY


  template <unsigned n>
  inline
  typename function< meta::red< value::rgb<n> > >::result
  function< meta::red< value::rgb<n> > >::read(const value& c)
  {
    return c.red();
  }

  template <unsigned n>
  inline
  typename function< meta::red< value::rgb<n> > >::lresult
  function< meta::red< value::rgb<n> > >::write(value& c)
  {
    return c.red();
  }


# endif // ! MLN_INCLUDE_ONLY


} // end of namespace mln

#endif // MLN_FUN_META_RED_HH
