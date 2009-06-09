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

#ifndef MLN_FUN_META_LUM_HH
# define MLN_FUN_META_LUM_HH

/// \file
///
/// Meta function to retrieve/modify the color luminosity.

# include <mln/value/hsl.hh>
# include <mln/core/concept/meta_fun.hh>

namespace mln
{

  namespace meta
  {

    template <class H>
    struct lum : impl< lum<H> >
    {
      typedef H value;
    };

  } // end of namespace mln::meta


  template <class H, class S, class I>
  struct function< meta::lum< value::hsl_<H, S, I> > >
    : public Function_v2v<function< meta::lum < value::hsl_<H, S, I> > > >
  {
    typedef value::hsl_<H, S, I> value;

    typedef H result;
    H read(const value& h);

    typedef H& lresult;
    H& write(value& h);
  };


# ifndef MLN_INCLUDE_ONLY

  template <class H, class S, class I>
  inline
  H
  function< meta::lum< value::hsl_<H, S, I> > >::read(const value& h)
  {
    return h.lum();
  }

  template <class H, class S, class I>
  inline
  H&
  function< meta::lum< value::hsl_<H, S, I> > >::write(value& h)
  {
    return h.lum();
  }

# endif // ! MLN_INCLUDE_ONLY


} // end of namespace mln

#endif // ! MLN_FUN_META_LUM_HH
