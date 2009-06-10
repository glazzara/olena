// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_VALUE_CAST_HH
# define MLN_VALUE_CAST_HH

/*! \file
 *
 * \brief Definition of the mln::value::cast routine.
 */

# include <mln/core/concept/value.hh>
# include <mln/value/equiv.hh>


namespace mln
{

  namespace value
  {


    /// Cast a value \p src from type \c Src to type \c Dest. 
    template <typename Dest, typename Src>
    Dest cast(const Src& src);



# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {

      template <typename S>
      inline
      const S&
      cast_(const void*, const S& src)
      {
	return src;
      }

      template <typename O, typename S>
      inline
      const S&
      cast_(const Object<O>*, const S& src)
      {
	return src;
      }

      template <typename V, typename S>
      inline
      mln_value_equiv(S)
      cast_(const Value<V>*, const S& src)
      {
	return mln::value::equiv(src);
      }

    } // end of namespace mln::value::internal


    template <typename Dest, typename Src>
    inline
    Dest cast(const Src& src)
    {
      // FIXME: Add static_cast<Dest>?
      // FIXME: Add exact()?
      return static_cast<Dest>(internal::cast_(&src, src));
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln


#endif // ! MLN_VALUE_CAST_HH
