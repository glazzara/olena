// Copyright (C) 2007, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_DEBUG_FORMAT_HH
# define MLN_DEBUG_FORMAT_HH

/*! \file
 *
 * \brief Routines that format a value to print it properly.
 */


namespace mln
{

  namespace debug
  {

    /*! \brief Default version for formatting a value is a no-op.
      \ingroup mlndebug
    */
    template <typename T>
    const T& format(const T& v);

    /*! \brief Format a Boolean to print it nicely: "|" for true and
      "-" for false.
      \ingroup mlndebug
    */
    char format(bool v);

    /*! \brief Format a signed char to print it properly, i.e., like
      an integer value.
      \ingroup mlndebug
    */
    signed short format(signed char v);

    /*! \brief Format an unsigned char to print it properly, i.e.,
      like an integer value.
      \ingroup mlndebug
    */
    unsigned short format(unsigned char v);



# ifndef MLN_INCLUDE_ONLY

    template <typename T>
    inline
    const T&
    format(const T& v)
    {
      return v;
    }

    inline
    char
    format(bool v)
    {
      return v ? '|' : '-';
    }

    inline
    signed short
    format(signed char v)
    {
      return v;
    }

    inline
    unsigned short
    format(unsigned char v)
    {
      return v;
    }

# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace mln::debug

} // end of namespace mln


#endif // ! MLN_DEBUG_FORMAT_HH
