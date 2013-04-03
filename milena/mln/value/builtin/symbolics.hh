// Copyright (C) 2007, 2008, 2009, 2013 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_VALUE_BUILTIN_SYMBOLICS_HH
# define MLN_VALUE_BUILTIN_SYMBOLICS_HH

/// \file
///
/// Some definitions about built-in symbolic types.


# include <mln/value/concept/built_in.hh>
# include <mln/value/concept/symbolic.hh>
# include <mln/trait/value_.hh>


namespace mln
{


  template <>
  struct category< bool >
  {
    typedef value::Built_In< value::Symbolic<void> > ret;
  };


  namespace trait
  {

    template <>
    struct value_<bool>
    {
      typedef value::nature::symbolic nature;
      typedef value::kind::binary     kind;

      enum {
	dim   = 1,
	nbits = 1,
	card  = 2
      };
      typedef value::quant::low       quant;

      static int min() { return 0; }
      static int max() { return 1; }

      static const char* name() { return "bool"; }
    };

  } // end of namespace mln::trait

} // end of namespace mln


#endif // ! MLN_VALUE_BUILTIN_SYMBOLICS_HH
