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

#ifndef MLN_VALUE_INT_U8_HH
# define MLN_VALUE_INT_U8_HH

/// \file
///
/// Define the alias value::int_u8.

# include <mln/value/int_u.hh>


namespace mln
{

  namespace value
  {


    /*!
      \struct int_u8
      \headerfile <>

      \brief Alias for unsigned 8-bit integers.

      \ingroup valueuint
      \relates value::int_u
    */
    /// \cond ALIASES
    typedef mln::value::int_u<8> int_u8;
    /// \endcond


  } // end of namespace mln::value

} // end of namespace mln


#endif // ! MLN_VALUE_INT_U8_HH
