// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_FUN_META_BLUE_HH
# define MLN_FUN_META_BLUE_HH

/// \file
///
/// Meta function to retrieve/modify the blue component.

# include <mln/fun/meta/impl.hh>
# include <mln/value/rgb.hh>

namespace mln
{

  namespace meta
  {

    template <class T>
    struct blue : impl< blue<T> >
    {
      typedef T value;
    };

  } // end of namespace mln::meta


  template <unsigned n>
  struct function< meta::blue< value::rgb<n> > >
    : public Function_v2v<function< meta::blue < value::rgb<n> > > >
  {
    typedef value::rgb<n> value;

    typedef typename value::blue_t result;
    result read(const value& c);

    typedef result& lresult;
    lresult write(value& c);
  };


# ifndef MLN_INCLUDE_ONLY


  template <unsigned n>
  inline
  typename function< meta::blue< value::rgb<n> > >::result
  function< meta::blue< value::rgb<n> > >::read(const value& c)
  {
    return c.blue();
  }

  template <unsigned n>
  inline
  typename function< meta::blue< value::rgb<n> > >::lresult
  function< meta::blue< value::rgb<n> > >::write(value& c)
  {
    return c.blue();
  }


# endif // ! MLN_INCLUDE_ONLY


} // end of namespace mln

#endif // ! MLN_FUN_META_BLUE_HH
