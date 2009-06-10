// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_VALUE_INTERNAL_LIMITS_HH
# define MLN_VALUE_INTERNAL_LIMITS_HH

/// \file
///
/// Define built-in type limits.

# if defined(__GNUC__) && __GNUC__ < 3
#  include <cfloat>
#  include <climits>
# else
#  include <limits>
# endif

namespace mln
{

  namespace value
  {

    namespace internal
    {

      template<typename T>
      struct limits
# if !defined(__GNUC__) || __GNUC__ >= 3
        : std::numeric_limits<T>
      { };
# else
      {
        static const bool is_specialized = false;
      };

      template<>
      struct limits<bool>
      {
	static bool min() throw() { return false; }
	static bool max() throw() { return false; }
	static bool epsilon() throw()
        { return false; }
      };

      template<>
      struct limits<char>
      {
	static char min() throw() { return CHAR_MIN; }
	static char max() throw() { return CHAR_MAX; }
	static char epsilon() throw()
	{ return 0; }
      };

      template<>
      struct limits<signed char>
      {
	static signed char min() throw() { return SCHAR_MIN; }
	static signed char max() throw() { return SCHAR_MAX; }
	static signed char epsilon() throw()
	{ return 0; }
      };

      template<>
      struct limits<unsigned char>
      {
	static unsigned char min() throw() { return 0; }
	static unsigned char max() throw() { return UCHAR_MAX; }
	static unsigned char epsilon() throw()
	{ return 0; }
      };

      template<>
      struct limits<short>
      {
	static short min() throw() { return SHRT_MIN; }
	static short max() throw() { return SHRT_MAX; }
	static short epsilon() throw()
	{ return 0; }
      };

      template<>
      struct limits<unsigned short>
      {
	static unsigned short min() throw() { return 0; }
	static unsigned short max() throw() { return USHRT_MAX; }
	static unsigned short epsilon() throw()
	{ return 0; }
      };

      template<>
      struct limits<int>
      {
	static int min() throw() { return INT_MIN; }
	static int max() throw() { return INT_MAX; }
	static int epsilon() throw()
	{ return 0; }
      };

      template<>
      struct limits<unsigned int>
      {
	static unsigned int min() throw() { return 0; }
	static unsigned int max() throw() { return UINT_MAX; }
	static unsigned int epsilon() throw()
	{ return 0; }
      };

      template<>
      struct limits<long>
      {
	static long min() throw() { return LONG_MIN; }
	static long max() throw() { return LONG_MAX; }
	static long epsilon() throw()
	{ return 0; }
      };

      template<>
      struct limits<unsigned long>
      {
	static unsigned long min() throw() { return 0; }
	static unsigned long max() throw() { return ULONG_MAX; }
	static unsigned long epsilon() throw()
	{ return 0; }
      };


/*
**   Some pre-ANSI-C99 systems like Linux/GCC 2.95 define
**     ULONGLONG_MAX, LONGLONG_MIN, LONGLONG_MAX; we use them if they're defined.
*/

#  if defined(HAVE_LONG_LONG) && !defined(LONGLONG_MIN)
#   define LONGLONG_MIN((long long) 0x8000000000000000LL)
#   define LONGLONG_MAX((long long) 0x7FFFFFFFFFFFFFFFLL)
#  endif


#  if defined(HAVE_LONG_LONG) && !defined(ULONGLONG_MAX)
      /* First check for ANSI C99 definition: */
#   ifdef ULLONG_MAX
#    define ULONGLONG_MAX  ULLONG_MAX
#   else
#    define ULONGLONG_MAX ((unsigned long long)(~0ULL))
#   endif

#  endif /* defined (HAVE_LONG_LONG) && !defined(ULONGLONG_MAX)*/


#  if defined(HAVE_LONG_LONG)
      template<>
      struct limits<long long>
      {
	static long long min() throw() { return LONGLONG_MIN; }
	static long long max() throw() { return LONGLONG_MAX; }
	static long long epsilon() throw()
	{ return 0; }
      };

      template<>
      struct limits<unsigned long long>
      {
	static unsigned long long min() throw() { return 0; }
	static unsigned long long max() throw() { return ULONGLONG_MAX; }
	static unsigned long long epsilon() throw()
	{ return 0; }
      };
#  endif

      template<>
      struct limits<float>
      {
	static float min() throw()   { return FLT_MIN; }
	static float max() throw() { return FLT_MAX; }
	static float epsilon() throw()
	{ return FLT_EPSILON; }
      };

      template<>
      struct limits<double>
      {
	static float min() throw()   { return DBL_MIN; }
	static float max() throw() { return DBL_MAX; }
	static float epsilon() throw()
	{ return DBL_EPSILON; }
      };

# endif
    } // end of namespace mln::value::internal

  } // end of namespace mln::value

} // end of namespace mln

#endif // ! MLN_VALUE_INTERNAL_LIMITS_HH
