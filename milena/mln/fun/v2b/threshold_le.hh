// Copyright (C) 2008, 2009, 2011 EPITA Research and Development
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

#ifndef MLN_FUN_V2B_THRESHOLD_LE_HH
# define MLN_FUN_V2B_THRESHOLD_LE_HH

/// \file
///
/// FIXME.

# include <mln/core/concept/function.hh>


namespace mln
{

  namespace fun
  {

    namespace v2b
    {

      /// Threshold function.
      /// f(v) = (v <= threshold).
      template <typename V>
      struct threshold_le : public Function_v2b< threshold_le<V> >
      {
	typedef bool result;
	bool operator()(const V& v) const;

	threshold_le(const V& a);
	V a;
      };


# ifndef MLN_INCLUDE_ONLY

      template <typename V>
      inline
      threshold_le<V>::threshold_le(const V& a)
	: a(a)
      {
      }

      template <typename V>
      inline
      bool
      threshold_le<V>::operator()(const V& v) const
      {
	// Here the test seems to be inversed compared to the usual
	// use. Indeed, we want to preserve the following convention:
	// True for foreground and False for background.
	return v <= a;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::v2b

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_V2B_THRESHOLD_LE_HH
